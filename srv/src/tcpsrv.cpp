//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifdef ISHOST
#include <iostream>
#endif

#include "tcpsrv.hpp"

#include "net.hpp"
#include "stypes.hpp"
#include "sysctrl.hpp"

// #define XDEVEL
#include "trace_macros.hpp"

#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <ctime>

#define c_TCP_RECV_BF_SIZE 1024
#define c_SRV_TIMER_LOOPS 128

//	============================================================
//	tcpsrv
	tcpsrv::tcpsrv(unsigned s, tcpproc* pp)
	: sz	(s ? s : c_TCP_SRV_DEF_CLIENTS)
	, ppr	(pp)
	, svs	(0)
	, bfr	(c_TCP_RECV_BF_SIZE)
	, cls	(new tcpclnt*[sz])
	, bf	(new char[c_TCP_RECV_BF_SIZE])
	, ttl	(0)
	, state (svs::start)
	{
	}
	tcpsrv::~tcpsrv()
	{
		postrun();
		delete cls;
		delete bf;
	}

//!	internal state setter with processor information.
	void tcpsrv::cstate(int s)
	{
		if (s != state) {
			state = s;
			TRACE_VAR(state)
			if (ppr) ppr->cstate(state);
		}
	}
//!	trigered shutdown.
	void tcpsrv::shutdown(int r)
	{
		TRACE_FLOW
		ret = r;
	//	check for send data remains
		if (sts.ncl) {
			tcpclnt** pcl = cls;
			for (unsigned i = sz; i--; ++pcl) if (*pcl && (*pcl)->sbf) {
				tl = c_SRV_TIMER_LOOPS;
				cstate(svs::finish);
				mkfds();
				return;
			}
		}
		tl = 0;
		cstate(svs::down);
	}

	void tcpsrv::postrun(void)
	{
		TRACE_FLOW
		try {
			tcpclnt** pcl = cls;
			for (unsigned i = sz; i--; ++pcl) if (*pcl) {
				close((*pcl)->s);
				delete *pcl;
				*pcl = 0;
			}
			if (svs) {
				close(svs);
				svs = 0;
			}
		}
		catch (...) {}
		sts.ncl = 0;
	}


//!	write data to tcp send buffer.
	bool tcpsrv::send(unsigned n, t_cc c, unsigned l)
	{
		TRACE_FLOW
		TRACE_VAR(n)
		tcpclnt* cl = cls[n];
		if (!cl) return false;
		cl->sbf.add(c, l);
		TRACE_VAR(cl->sbf.size())
		return true;
	}
//!	send bufferred data after client disconnect check.
	bool tcpsrv::sendbf(int ws, cbuffer& sbf)
	{
		TRACE_FLOW
		if (!sbf) return true;
		int rcs;
		try { rcs = ::send(ws, sbf, sbf.size(), 0); }
		catch(...) {}
		TRACE_VAR(rcs)
		if (rcs > 0) {
			sbf.shift(rcs);
			sts.trv << rcs;
		}
		if (!sbf) sbf.clear();
		return !sbf;
	}

//!	set time to live.
	void tcpsrv::sttl(unsigned i, unsigned t)
	{
		TRACE_FLOW
		tcpclnt* cl = cls[i];
		if (cl) sttl(cl, t);
	}
//!	set ttl of a client.
	void tcpsrv::sttl(tcpclnt* cl, unsigned t)
	{
		cl->ttl = t;
		cl->bb	= t ? sts.tt + t : 0;
	}

//	reset of all field sets
	void tcpsrv::mkfds(void)
	{
		TRACE_FLOW
		TRACE_VAR(sts.ncl)
		TRACE_VAR(svs)
		nrf = 0;
		FD_ZERO(&mfds);

		if (state > svs::finish) {
			FD_SET(svs, &mfds);
			nrf = svs + 1;
		}
		if (!sts.ncl) return;

		unsigned nc = 0;
		tcpclnt** pcl = cls;
		for (unsigned i = sz; i--; ++pcl) {
			if (*pcl) {
				int cs = (*pcl)->s;
				FD_SET(cs, &mfds);
				nrf = nrf > cs ? nrf : cs + 1;
				++nc;
				if (nc == sts.ncl) break;
			}
		}
	}
//!	remove a client.
	void tcpsrv::closecl(unsigned i)
	{
		TRACE_FLOW
		tcpclnt* cl = cls[i];
		if (cl) {
#ifdef ISHOST
			std::cerr << "--" << ' ' << i << std::endl;
#endif
			close(cl->s);
			delete cl;
			cls[i] = 0;
			--sts.ncl;
		//	rebuild field set
			mkfds();
		//	inform processor
			if (ppr) ppr->remove(i);
		}
	}

	int tcpsrv::run(t_u2 port, t_cc cip)
	{
		TRACE_FLOW
		memset(cls, 0, sz * sizeof(tcpclnt*));
		memset(&sts, 0, sizeof(tcpstats));

		port = port ? port : 80;
		TRACE_VAR(port)
		TRACE_VARQ(cip)

		bool ok = true;
	//	server socket
		TRACE_INF("server socket")
		svs = socket(PF_INET, SOCK_STREAM, 0);
		ok = (svs != -1);
		TRACE_VAR(svs)
	//	reusable
		int rsb = 1;
	//	set reusable (return must be 0)
		TRACE_INF("reusable")
		ok = ok &&
			(setsockopt(svs, SOL_SOCKET, SO_REUSEADDR, &rsb, sizeof(int)) == 0);
		if (ok) {
			TRACE_INF("server bind")
		//	server address container
			struct sockaddr svad;
		//	set port [and ip]
			set_address(svad, port, cip);

		//	bind to port [ and IP ] (return must be 0)
			ok = (bind(svs, &svad, sizeof(svad)) == 0);
		}
	//	start listening (return must be 0)
		if (ok) {
			TRACE_INF("listen")
			ok = (listen(svs, 10) == 0);
		}

		if (!ok) {
			TRACE_ERR("failure")
			close(svs);
			svs = 0;
			cstate(svs::down);
			return ctrl::error;
		}
		cstate(svs::clean);

		mkfds();
		ret  = ctrl::ok;
		time(&sts.ts);

		tl = c_SRV_TIMER_LOOPS;

	//	the loop
		TRACE_INF("starting loop")
//	loop behaviour
//	continue permanently if state greater down
//	continue for a defined count if state == finish
//	call timer after defined count if state is active
//	call timer always if state is sleep or clean
		while (state > svs::down) {
		//	very short select time (10 th of a milisecond)
			struct timeval stm = { 0, 100 };
			switch (state) {
			//	if clean or sleep: add a second
				case svs::sleep:
					TRACE_INF("sleep")
					sleep(1);
				case svs::clean:
					time(&sts.tt);
					stm.tv_sec = 1;
				break;
				case svs::active:
					if (!tl) { time(&sts.tt); tl = c_SRV_TIMER_LOOPS; }
				break;
				case svs::finish:
					if (!tl) cstate(svs::down);
				default:
				break;
			}
		//	call for time // after given number of loops
			if (state == svs::down) break;
			if (tl) --tl;

			memcpy(&cfds, &mfds, sizeof(mfds));

			int rc = select(nrf, &cfds, 0, 0, &stm);

			if (rc == -1) {
		//	TODO: no exit on select error! re-launch server socket!
				TRACE_ERR("select")
			}
		//	CLIENT CONNECT
			if (state > svs::finish && rc > 0 && FD_ISSET(svs, &cfds)) {
				struct sockaddr csa;
				socklen_t sl = sizeof(csa);
				TRACE_INF("client connect")
				int cs = accept(svs, &csa, &sl);
			//	accept error
				if (cs == -1) {
					TRACE_ERR("accept")
				}
			//	all spots occupied
				else if (sts.ncl == sz) {
					TRACE_ERR("all " << sz << " sockets in use")
					close(cs);
				}
			//	set non blocking
				else if (!unblock(cs)) {
					TRACE_ERR("unblock")
					close(cs);
				}
			//	find spot and add to select fields
				else {
				//	add to select fields
					FD_SET(cs, &mfds);
					nrf = nrf > cs ? nrf : cs + 1;
				//	find next free spot
					for (unsigned i = 0; i < sz; ++i) {
						if (!cls[i]) {
							tcpclnt* cl = new tcpclnt;
							cl->s = cs;
							cls[i] = cl;
							TRACE_INF("added " << cs << " to spot " << i);
							++sts.ncl;
							++sts.ncc;
						//	set default ttl
							sttl(cl, ttl);
						//	inform processor about new client
							if (ppr) ppr->create(i);
							break;
						}
					}
				//	update access time
					tla = sts.tt;
				}
				TRACE_VAR(sts.ncl)
			} // client connect

			bool dp = false;

			if (sts.ncl) {
			//	clients check loop activity
				for (unsigned i = 0; i < sz; ++i) {
					tcpclnt* cl = cls[i];
					if (!cl) continue;
					int cs = cl->s;
				//	========================================
				//	client activity
					if (rc > 0 && FD_ISSET(cs, &cfds)) {
#ifdef ISHOST
						std::cerr << "rq" << ' ' << i << ' ' << cs << std::endl;
#endif
					//	set new time stamp if kepp alive given
						if (cl->ttl) {
							cl->bb = sts.tt + cl->ttl;
							TRACE_VAR(cl->bb)
						}

						bfr.clear();

						for (unsigned n = 0;; ++n) {
							int rrc = recv(cs, bf, c_TCP_RECV_BF_SIZE, 0);
						//	succesful read (only reason to continue)
							if (rrc > 0) {
								TRACE_INF("read " << rrc << " at " << cs);
								if (state > svs::finish) bfr.add(bf, rrc);
								continue;
							}
						//	error
							else if (rrc == -1 && n == 0) {
								TRACE_ERR("read " << cs)
							}
						//	client closed connection
							else if (rrc == 0 && n == 0) {
								TRACE_INF("client closed: " << cs)
								closecl(i);
								cl = 0;
							}
							else if (rrc <= 0) {
								TRACE_INF("data finished " << strlen(bfr))
								if (state > svs::finish) {
								//	update access time
									tla = sts.tt;
									if (ppr) ppr->process(i, bfr);
								}
							}
							break;
						} // read parts loop
					} // client activity
				//	no activity: check for timeout
					else if (cl->ttl && cl->bb < sts.tt) {
						TRACE_INF(cs << " timed out " << sts.tt << ' ' << cl->bb)
						closecl(i);
						cl = 0;
					}
				//	========================================
				//	directly after that:
				//	process pending send to client data
					if (cl && cl->sbf && !sendbf(cs, cl->sbf)) dp = true;

				} // clients loop
			} // if clients
		//	change state depending on pending send data / last access timeout
			if (state > svs::finish) {
				if (dp) cstate(svs::active);
				else if (sts.tt - tla > c_TCP_SRV_IDLE_TO_SLEEP) cstate(svs::sleep);
				else cstate(svs::clean);
			}
			else if (!dp) cstate(svs::down);

		} // permanent loop

		postrun();
		return ret;
	}

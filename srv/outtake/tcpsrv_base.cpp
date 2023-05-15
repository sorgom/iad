//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include <tcpsrv_base.h>

#include <net.h>

#include <trace_macros.h>
#include <stypes.h>

#include <sys/socket.h>
#include <cstring>

//	============================================================
//	tcpsrv
	tcpsrv::tcpsrv(unsigned s)
	: sz( s < 1 ? 10 : s)
	, ncl(0)
	, bfr(128)
	{
		cls = new int[sz];
		memset(cls, 0, sz * sizeof(int));
		bf = new char[128];
	}
	tcpsrv::~tcpsrv()
	{
		delete cls;
		delete bf;
	}

//	reset of all field sets
	void tcpsrv::mkfds()
	{
		TRACE_FLOW
		TRACE_VAR(svs)
		FD_ZERO(&afds);
		FD_SET(svs, &afds);
		nf = svs + 1;
		if (ncl) {
			unsigned nc = 0;
			for (unsigned i = 0; i < sz; ++i) {
				int cs = cls[i];
				if (cs) {
					FD_SET(cs, &afds);
					nf = nf > cs ? nf : cs + 1;
					++nc;
					if (nc == ncl) break;
				}
			}
		}
	}

	bool tcpsrv::run(t_u2 port, t_cc cip)
	{
		TRACE_FLOW
		TRACE_VAR(port)
		TRACE_VARQ(cip)
		bool ok = true;
	//	server socket
		TRACE_INF("server socket")
		svs = socket(PF_INET, SOCK_STREAM, 0);
		TRACE_VAR(svs)
	//	reusable
		int rsb = 1;
	//	set reusable (return must be 0)
		TRACE_INF("reusable")
		ok = (setsockopt(svs, SOL_SOCKET, SO_REUSEADDR, &rsb, sizeof(int)) == 0);
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
			return false;
		}

		mkfds();

	//	the loop
		TRACE_INF("starting loop")
		while (true) {
			memcpy(&rfds, &afds, sizeof(afds));
			int rc = select(nf, &rfds, 0, 0, 0);
			if (rc == -1) {
				TRACE_ERR("select")
				return false;
			}
			if (rc > 0) {
			//	CLIENT CONNECT
				if (FD_ISSET(svs, &rfds)) {
					struct sockaddr csa;
					socklen_t sl = sizeof(csa);
					TRACE_INF("client connect")
					int cs = accept(svs, &csa, &sl);
				//	accept error
					if (cs == -1) {
						TRACE_ERR("accept")
					}
				//	all spots occupied
					else if (ncl == sz) {
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
						FD_SET(cs, &afds);
						nf = nf > cs ? nf : cs + 1;
					//	find next free spot
						for (unsigned i = 0; i < sz; ++i) {
							if (!cls[i]) {
								cls[i] = cs;
								TRACE_INF("added " << cs << " to spot " << i);
								++ncl;
								break;
							}
						}
					}
				} // client connect
			//	client activity
				if (ncl) {
					for (unsigned i = 0; i < sz; ++i) {
						int& cs = cls[i];
						if (cs && FD_ISSET(cs, &rfds)) {
							bfr.clear();
							for (unsigned n = 0;; ++n) {
								int rc = read(cs, bf, 128);
							//	succesful read (only reason to continue)
								if (rc > 0) {
									TRACE_INF("read " << rc << " at " << cs);
									bfr.add(bf, static_cast<unsigned>(rc));
									TRACE_VAR(bfr.size());
									continue;
								}
							//	error
								else if (rc == -1 && n == 0) {
									TRACE_ERR("read " << cs)
								}
							//	client closed connection
								else if (rc == 0 && n == 0) {
									TRACE_INF("client closed: " << cs)
									close(cs);
									cs = 0;
								//	rebuild field set
									mkfds();
								}
								else if (rc <= 0) {
									TRACE_VARQ(bfr.str());
								}
								break;
							} // read parts loop
						} // client activity
					} // clients loop
				} // if clients

			} // select
		} // permanent loop

		return true;
	}

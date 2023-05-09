//| ============================================================
//! @file
//!
//! A TCP server and processor example.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef TCPSRV_HPP
#define TCPSRV_HPP

#include "lnum.hpp"
#include "stypes.hpp"
#include "types.hpp"

#include <ctime>
#include <sys/select.h>

//!	tcp server send at once byte size.
#define c_TCP_SEND_BYTES 2048
//!	default number of clients
#define c_TCP_SRV_DEF_CLIENTS 10
//!	idle time until falling asleep.
#define c_TCP_SRV_IDLE_TO_SLEEP 600

//!	base tcp client representation.
	class tcpclnt
	{
	public:
	//! the socket.
		int s;
	//!	the expiry interval (keep-alive).
		unsigned ttl;
	//!	the best before.
		time_t bb;
	//!	send data buffer
		cbuffer sbf;
	//!	default constructor.
		inline tcpclnt()
		: s(0)
		, ttl(0)
		, bb(0)
		, sbf(1024)
		{}
	};

	class tcpsrv;

namespace svs
{
//!	server states enumeration.
	enum {
	//!	startup
		start = 0,
	//!	shutting down.
		down,
	//!	finish send data for shutdown.
		finish,
	//!	sleeping.
		sleep,
	//!	no outgoing data.
		clean,
	//!	active with outgoing data.
		active
	};
}

//!	tcp processor base class.
	class tcpproc
	{
	private:
		friend class tcpsrv;
	//!	install a new internal client representation.
		inline virtual void create(unsigned) {}
	//!	remove client.
		inline virtual void remove(unsigned) {}
	//! process client data.
		inline virtual void process(unsigned, t_fcc) {}
	//!	process a state change
		inline virtual void cstate(int) {}
	};

//!	the gimmik section: tcp server statistics
	struct tcpstats
	{
	//!	start time.
		time_t ts;
	//!	current time.
		time_t tt;
	//!	number of clients connected.
		unsigned ncl;
	//!	number of visits (client connects).
		unsigned ncc;
	//!	transferred data volume: large number
		lnum trv;
	};

//!	tcp server.
	class tcpsrv
	{
	public:
	//!	constructor
		tcpsrv(unsigned nc = 0, tcpproc* tp = 0);
		~tcpsrv();
	//!	start infinite server loop.
		int run(t_u2 port = 0, t_cc cip = 0);
	//!	max number of clients.
		const unsigned sz;
	//!	write data to tcp send buffer.
		bool send(unsigned n, t_cc c, unsigned l);
	//!	set time to live of a client (0 = endless).
		void sttl(unsigned n, unsigned t);
	//!	set default ttl (default: 0 = endless).
		inline void sttl(unsigned t) { ttl = t; }
	//!	trigered shutdown.
		void shutdown(int);
	//!	statistics return.
		inline const tcpstats& stats(void) { return sts; }
	private:
	//!	send bufferred data after client disconnect check.
		bool sendbf(int ws, cbuffer& sbf);
	//!	set ttl of a client.
		void sttl(tcpclnt*, unsigned t);
	//!	client representations.
		tcpclnt** const cls;
	//	server socket
		int svs;
	//!	field sets for select (master and work copy).
		fd_set mfds, cfds;
	//!	number of fields set for select.
		int nrf;
	//!	remove a client.
		void closecl(unsigned);
	//!	reset of all field sets.
		void mkfds(void);
	//!	read buffer.
		char* bf;
	//!	assembly buffer.
		cbuffer bfr;
	//!	assigned processor.
		tcpproc* ppr;
	//!	default time to live.
		time_t ttl;
	//!	last activity time stamp.
		time_t tla;
	//!	stats container.
		tcpstats sts;
	//!	continue and exit return.
		bool cont;
		unsigned tl;
		int ret;
	//!	post run procedure.
	//!	close all sockets remove and zero all clients.
		void postrun(void);
	//!	internal state.
		int state;
	//!	internal state setter with processor information.
		void cstate(int);
	};



#endif // TCPSRV_HPP

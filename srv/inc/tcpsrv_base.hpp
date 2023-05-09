//| ============================================================
//! @file
//!
//! A simple tcp server.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef TCPSRV_BASE_HPP
#define TCPSRV_BASE_HPP

#include "stypes.hpp"
#include "types.hpp"

#include <sys/select.h>

	class tcpsrv
	{
	public:
		tcpsrv(unsigned = 0);
		~tcpsrv();
		bool run(t_u2, t_cc = 0);
	private:
	//	server socket
		int svs;
	//	client sockets
		int* cls;
	//	max number of clients
		unsigned sz;
	//	current number of clients
		unsigned ncl;
	//	field sets
		fd_set afds, rfds;
	//	number of fields set for select
		int nf;
	//	reset of all field sets
		void mkfds(void);
	//	read and assembly buffer
		char* bf;
		cbuffer bfr;
	};

#endif // TCPSRV_BASE_HPP

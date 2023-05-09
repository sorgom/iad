//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include "net.hpp"

//	memset / memcpy
#include <cstring>
//	htons
#include <netinet/in.h>
//	inet_aton
#include <arpa/inet.h>
//	fcntl
#include <fcntl.h>


//!	Set a socket address with port and IP address.
//	bool set_address(struct sockaddr& addr, t_u2 port, t_cc cip)
	bool set_address(struct sockaddr& addr, t_u2 port, t_cc cip)
	{
		memset(&addr, 0, sizeof(addr));
		addr.sa_family = AF_INET;
		port = htons(port);

		unsigned char aip[4] = {0, 0, 0, 0};
		if (cip) inet_aton(cip, reinterpret_cast<in_addr*>(aip));

		memcpy(addr.sa_data, &port, 2);
		memcpy(addr.sa_data + 2, aip, 4);

		return true;
	}

//!	Make a socket non-blocking.
	bool unblock(int sd)
	{
#undef c_NONBLOCK
#if defined(O_NONBLOCK)
	    #define c_NONBLOCK O_NONBLOCK
#elif defined(O_NDELAY)
		#define c_NONBLOCK O_NDELAY
#elif defined(O_FNDELAY)
		#define c_NONBLOCK O_FNDELAY
#endif

#ifdef c_NONBLOCK
		int fl = fcntl(sd, F_GETFL, 0);
		if (fl == -1) fl = 0;
		return (fcntl(sd, F_SETFL, fl | c_NONBLOCK) != -1);
#else
		return false;
#endif
	}

//!	Make a socket non-blocking.
	bool block(int sd)
	{
#undef c_NONBLOCK
#if defined(O_NONBLOCK)
	    #define c_NONBLOCK O_NONBLOCK
#elif defined(O_NDELAY)
		#define c_NONBLOCK O_NDELAY
#elif defined(O_FNDELAY)
		#define c_NONBLOCK O_FNDELAY
#endif

#ifdef c_NONBLOCK
		int fl = fcntl(sd, F_GETFL, 0);
		if (fl == -1) fl = 0;
		return (fcntl(sd, F_SETFL, fl ^ c_NONBLOCK) != -1);
#else
		return false;
#endif
	}

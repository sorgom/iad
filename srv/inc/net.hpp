//| ============================================================
//! @file
//!
//! socket and network utilities.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef NET_HPP
#define NET_HPP

#include "types.hpp"
#include <sys/socket.h>
#include <unistd.h>

//!	Set a socket address with port and IP address.
	bool set_address(struct sockaddr&, t_u2, t_cc = 0);

//!	Make a socket non-blocking.
	bool unblock(int);

//!	Make a socket blocking.
	bool block(int);



#endif // NET_HPP

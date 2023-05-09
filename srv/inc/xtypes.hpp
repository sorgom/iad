//| ============================================================
//! @file
//!
//!  Extended data types.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef XTYPES_HPP
#define XTYPES_HPP

#include "types.hpp"

//!	a byte container as for example an ip or mac address.
	class t_bytes
	{
	public:
	//!	constructor with given number of bytes.
		t_bytes(unsigned);
		~t_bytes();
	//! number of bytes.
		const unsigned sz;
	//!	the byte container.
		unsigned char* bs;
	//!	comparism.
		bool operator< (const t_bytes&) const;
		bool operator==(const t_bytes&) const;
		bool operator> (const t_bytes&) const;
	private:
		t_bytes();
	};

//!	IPv4 address.
	class t_ip4 : public t_bytes
	{
	public:
		inline t_ip4()
		: t_bytes(4)
		{}
		inline t_ip4(t_uc u0, t_uc u1, t_uc u2, t_uc u3)
		: t_bytes(4)
		{
			bs[0] = u0;
			bs[1] = u1;
			bs[2] = u2;
			bs[3] = u3;
		}
	};
//!	mac address.
	class t_mac : public t_bytes
	{
	public:
		inline t_mac()
		: t_bytes(6)
		{}
	};

#endif // XTYPES_HPP

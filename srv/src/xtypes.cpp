//| ============================================================
//! @file
//!
//! Extended data types.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include <xtypes.h>

#include <cstring>

//	============================================================
//	t_bytes

//!	constructor with given number of bytes.
	t_bytes::t_bytes(unsigned s)
	: sz(s)
	, bs(s ? new unsigned char[s] : 0)
	{
		if (bs) memset(bs, 0, sz * sizeof(unsigned char));
	}

	t_bytes::~t_bytes()
	{
		if (bs) delete bs;
	}
//!	comparism.
	bool t_bytes::operator<(const t_bytes& o) const
	{
		if (o.sz != sz) return o.sz > sz;
		for (unsigned i = 0; i < sz; ++i) {
			if (bs[i] < o.bs[i]) return true;
			if (bs[i] > o.bs[i]) return false;
		}
		return false;
	}
	bool t_bytes::operator==(const t_bytes& o) const
	{
		return (!(o < *this || *this < o));
	}
	bool t_bytes::operator>(const t_bytes& o) const
	{
		return o < *this;
	}

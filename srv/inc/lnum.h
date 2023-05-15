//| ============================================================
//! @file
//!
//! large number storage and string.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef LNUM_H
#define LNUM_H

// #include "stypes.hpp"
#include "outstream.h"

	class lnum
	{
	public:
		inline lnum()
		: g(0)
		, n(0)
		{}
		lnum& operator << (unsigned);
		void  operator =  (unsigned);
		void  operator >> (outstream&) const;
	private:
	//! 1000,000,000.
		unsigned g;
	//!	1.
		unsigned n;
	//!	overflow check.
		void ovfl();
	};

	inline outstream& operator<<(outstream& cb, const lnum& ln)
	{
		ln >> cb;
		return cb;
	}

#endif // LNUM_H

//| ============================================================
//! @file
//!
//! large number storage and string.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef LNUM_HPP
#define LNUM_HPP

// #include "stypes.hpp"
#include "outstream.hpp"

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
		void ovfl(void);
	};

	inline outstream& operator<<(outstream& cb, const lnum& ln)
	{
		ln >> cb;
		return cb;
	}

#endif // LNUM_HPP

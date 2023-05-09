//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef TRACE_STYPES_HPP
#define TRACE_STYPES_HPP

#ifdef DEVEL

#include "stypes.hpp"
#include "ccs.hpp"
#include <iostream>

	inline std::ostream& operator<<(std::ostream& os, const xbuffer& bf)
	{
		os << (bf ? bf.str() : uc_null);
		return os;
	}

	inline std::ostream& operator<<(std::ostream& os, const s_cc& sc)
	{
		os << (sc ? sc.str() : uc_null);
		return os;
	}


#endif

#endif // TRACE_STYPES_HPP

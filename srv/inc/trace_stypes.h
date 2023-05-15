//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef TRACE_STYPES_H
#define TRACE_STYPES_H

#ifdef DEVEL

#include "stypes.h"
#include "ccs.h"
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

#endif // TRACE_STYPES_H

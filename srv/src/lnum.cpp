//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include <lnum.h>
#include <stypes.h>

#include <trace_macros.h>

#define c_LNUM_OVERFLOW 1073741824
#define c_LDUB_OVERFLOW 1073741824.0

//	============================================================
//	lnum

	lnum& lnum::operator << (unsigned i)
	{
		TRACE_FLOW
		n += i;
		ovfl();
		return *this;
	}
	void lnum::operator = (unsigned i)
	{
		TRACE_FLOW
		n = i;
		ovfl();
	}

	void lnum::operator >> (outstream& cbf) const
	{
		TRACE_FLOW
		static t_cc c1024 = "GTPXXX";
		if (g) {
			double d = g + static_cast<double>(n) / c_LDUB_OVERFLOW;
			human(cbf, d, c1024);
		}
		else human(cbf, n);
	}

	void lnum::ovfl()
	{
		TRACE_FLOW
		if (n >= c_LNUM_OVERFLOW) {
			++g;
			n %= c_LNUM_OVERFLOW;
		}
		TRACE_VAR(n)
		TRACE_VAR(g)
	}

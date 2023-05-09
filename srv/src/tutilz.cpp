//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include "tutilz.hpp"

#include "trace_macros.hpp"

//!	standard trace #cmapper traverser function.
	void trtrace(const s_cc& k, const s_cc& v)
	{
		t_cc key = k;
		t_cc val = v;
		TRACE_VARQ(key);
		TRACE_VARQ(val);
	}

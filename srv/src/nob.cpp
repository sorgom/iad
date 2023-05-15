//| ============================================================
//! @file
//!
//! Named object nob.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include <nob.h>

//  tracing off: tests passed
#undef DEVEL
#include <trace_macros.h>

	void nob::put(outstream& os) const
	{
		TRACE_FLOW
		out(os);
		if (parent) {
			os.add('_');
			parent->put(os);
		}
	}

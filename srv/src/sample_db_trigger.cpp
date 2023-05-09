//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include "sample_db_trigger.hpp"

#include "bbl.hpp"
#include "bbl_mapper.hpp"
#include "err_inf.hpp"

// #undef DEVEL
#include "trace_macros.hpp"

using namespace bbl;

//!	a sample database update trigger function: adds database update to infos.
	void info_trigger::trigger(const sbuffer& sb)
	{
		TRACE_FLOW
		for (unsigned i = 0; i < sb.size(); ++i)
			infos << trans(c_Update) << ' ' << trans(c_Successful) << " (" << sb[i] << ')' << tte;
	}

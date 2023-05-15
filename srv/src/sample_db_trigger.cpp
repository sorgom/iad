//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include <sample_db_trigger.h>

#include <bbl.h>
#include <bbl_mapper.h>
#include <err_inf.h>

// #undef DEVEL
#include <trace_macros.h>

using namespace bbl;

//!	a sample database update trigger function: adds database update to infos.
	void info_trigger::trigger(const sbuffer& sb)
	{
		TRACE_FLOW
		for (unsigned i = 0; i < sb.size(); ++i)
			infos << trans(c_Update) << ' ' << trans(c_Successful) << " (" << sb[i] << ')' << tte;
	}

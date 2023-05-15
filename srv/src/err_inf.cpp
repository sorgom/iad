//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include <err_inf.h>
#include <dml.h>

#include <trace_macros.h>

//	============================================================
//	msg_tainer

	msg_tainer::msg_tainer()
	: bm(64)
	, bms(8)
	{}

	msg_tainer& msg_tainer::operator<<(tt_end)
	{
		if (bm) bms.take(bm);
		return *this;
	}

	msg_tainer& msg_tainer::operator<<(sfunc* sf)
	{
		(*sf)(bm);
		delete sf;
		return *this;
	}
	msg_tainer infos;
	msg_tainer errors;

//	============================================================
//	msg_cproc

	void msg_cproc::put(sbuffer&) {
		TRACE_FLOW
		for (int i = 0; i < rmt.bms.size(); ++i) {
			if (i) dml << tag::br();
			dml << rmt.bms[i];
		}
		rmt.bms.clear();
		rmt.bm.clear();
	}

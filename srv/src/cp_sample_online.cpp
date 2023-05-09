//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include "cp_sample_online.hpp"

#include "cp_convenience.hpp"
#include <cstdlib>
#include <ctime>

using namespace db;

//	============================================================
//	cp_fake_online_state

	void cp_fake_online_state::put(sbuffer&)
	{
		TRACE_FLOW
		if (!db_check()) return;

		db << from(dbn::OnlineState) << dbn::Id;
		unsigned cnt = db.count();

		srand(time(0));
		unsigned id = 1 + rand() % cnt;
		unsigned rate = rand() % 1000;
		TRACE_VAR(id)
		TRACE_VAR(rate)

		db << update(dbn::OnlineState, id) << dbn::Rate;
		if (rate % 3) db << rate;
		else db << c_db_null;
		db << exec();
	}

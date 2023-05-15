//| ============================================================
//! @file
//!
//!	Database stress test binary.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include <db.h>
#include <dbn.h>
#include <ini.h>
#include <vars.h>

#include <trace_macros.h>

#include <cstdlib>
#include <ctime>
// #include <unistd.h>
#include <windows.h>


int main(int argc, char * argv[])
{
	TRACE_FLOW
	load_ini();
	db::proc& db = db::proc::instance();
	if (!db.connect(dbdir, dbnames)) {
		TRACE_ERR("db.connect")
		return 1;
	}

	db << db::from(dbn::OnlineState) << dbn::Id;
	db::data* pd = db.get();
	if (!pd) {
		TRACE_ERR("db.get")
		return 1;
	}
	unsigned rows = pd->rows();
	if (pd) {
		for (unsigned r = rows; r--;) { TRACE_VAR(pd->next()); }
	}

	srand(time(0));
	db << db::prepare(dbn::OnlineState);

	while (true) {

		TRACE_INF("...")
		for (int i = 1000; i--;) {
			pd->rewind();
			for (unsigned r = rows; r--;) {
				unsigned rate = rand() % 1000 + 100;
				db << db::update(dbn::OnlineState, pd->next()) << dbn::Rate << rate << db::pile();
			}
			db << db::exec();
			Sleep(1000);
		}
		TRACE_INF(".")
		Sleep(2000);
	}
	return 0;
}

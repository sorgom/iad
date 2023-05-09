//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include "bbl_mapper.hpp"
#include "ccs.hpp"
#include "db.hpp"
#include "hdn_vars.hpp"
#include "httpsrv.hpp"
#include "ini.hpp"
#include "sample_db_trigger.hpp"
#include "sample_sysctrl.hpp"
#include "sysctrl.hpp"
#include "vars.hpp"

#define XDEVEL
#include "trace_macros.hpp"

#ifdef DEVEL
#include "tutilz.hpp"
#endif

	int main(void)
	{
		TRACE_FLOW
	//	runtime constants
		hdn::srv = true;

		db::proc& db = db::proc::instance();

		info_trigger trg;
		db.set_trigger(&trg);

		simplectrl sctrl;
		ctrl::set(&sctrl);

		bool cont;
		int  ret = ctrl::ok;
		do {
			TRACE_INF(c_iadsrv_ini)
			cont = load_ini(c_iadsrv_ini);
#ifdef DEVEL
			if (cont) ini.traverse(&trtrace);
#endif
			if (cont) cont = bbl::bblinit();

			if (cont) {
				TRACE_VARQ(dbdir)
				TRACE_VAR(dbnames.size())
				bool dbok = db.connect(dbdir, dbnames);
				TRACE_VARB(dbok)
				httpsrv srv;
				ret = srv.run();
				TRACE_VAR(ret)
				switch (ret) {
					case ctrl::error    :
					case ctrl::shutdown :
						cont = false;
					default:
					break;
				}
				db.disconnect();
			}

			sctrl.process(ret);

		}	while (cont);

		return 0;
	}

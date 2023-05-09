//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include "cp_date_time.hpp"
#include "cp_convenience.hpp"

#include <cstdlib>

//	if no conflicting names: shorten up the typing
using namespace ta;
using namespace tag;
using namespace tbl;
using namespace bbl;

	time_setter::time_setter(time_processor* pt)
	: rnd(60)
	, ptpr(pt)
	{}

	void time_setter::put(sbuffer& sb)
	{
		TRACE_FLOW

		if (!split(cpnm, sb)) return;

	//	get current time
		time_t ct;
		if (ptpr) ct = ptpr->get_time();
		else ct = curr_time;

		if (rnd) ct = (ct / rnd) * rnd + rnd;

		struct tm* ptm;
		ptm = localtime(&ct);

	//	if this is a form return
		if (bissue) {
			t_ch cd;
			if (cd = cgi.getn(c_day)) 	ptm->tm_mday	= atoi(cd);
			if (cd = cgi.getn(c_month)) 	ptm->tm_mon 	= atoi(cd);
			if (cd = cgi.getn(c_year)) 	ptm->tm_year 	= atoi(cd) - 1900;
			if (cd = cgi.getn(c_hour)) 	ptm->tm_hour 	= atoi(cd);
			if (cd = cgi.getn(c_minute)) ptm->tm_min 	= atoi(cd);
			if (cd = cgi.getn(c_second)) ptm->tm_sec 	= atoi(cd);

			ct = mktime(ptm);
			if (ptpr) ptpr->set_time(ct);
		}

	//	html output?
		if (!bout) return;

	//	declare key word as issue
		set_issue(cpnm);

		unsigned n;

	//	start table
		dml << tb();

	//	date row, label
		dml << tr() << td() << label() << trans(c_Date);

	//	day of month selector
		dml << td() << select() << name(c_day) << title(trans(c_Day)) << cnl;
		digisel(32, ptm->tm_mday, 1, 1);
		dml << tte;

	//	month selector
		dml << td() << select() << name(c_month) << title(trans(c_Month)) << cnl;
		for (n = 0; n <= 11; ++n) {
			dml << '\t' << option() << value(n);
			if (n == ptm->tm_mon) dml << selected();
			dml << trans(c_MonthShort[n]) << tte << cnl;
		}
		dml << tte;

	//	year selector
		dml << td() << select() << name(c_year) << title(trans(c_Year)) << cnl;
		for (int j = ptm->tm_year - 2; j <= ptm->tm_year + 2; ++j) {
			n = j + 1900;
			dml << '\t' << option();
			if (j == ptm->tm_year) dml << selected();
			dml << n << tte << cnl;
		}
		dml << tte;

	//	time row, label
		dml << tr() << td() << label() << trans(c_Time);

	//	hour selector
		dml << td() << select() << name(c_hour) << title(trans(c_Hour)) << cnl;
		digisel(24, ptm->tm_hour);
		dml << tte << ' ' << ':';

	//	minute selector
		dml << td() << select() << name(c_minute) << title(trans(c_Minute)) << cnl;
		digisel(60, ptm->tm_min);
		dml << tte << ' ' << ':';

	//	second selector
		dml << td() << select() << name(c_second) << title(trans(c_Second)) << cnl;
		unsigned stp = rnd;
		if 		(stp < 5 ) stp = 5;
		else if (stp > 15) stp = 15;
		digisel(60, ptm->tm_sec, stp);
		dml << tte;

	//	table end
		dml << tbe();
	}

	void time_setter::digisel(unsigned mx, unsigned ic, unsigned st, unsigned mn)
	{
		bool sd = true;
		for (unsigned n = mn; n < mx; n += st) {
			dml << '\t' << option();
			if (sd && n >= ic) { dml << selected(); sd = false; }
			dml << u22d(n) << tte << cnl;
		}
	}

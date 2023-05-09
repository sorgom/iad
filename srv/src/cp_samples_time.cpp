//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include "cp_samples_time.hpp"

#include "cmapper.hpp"
#include "ini_io.hpp"
#include "f_io.hpp"

//	#undef DEVEL
//	#define XDEVEL
#include "cp_convenience.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace tag;
using namespace ta;
using namespace tbl;
using namespace bbl;

//	============================================================
//	fake time
	fake_time* fake_time::instance(void)
	{
		static fake_time* p = new fake_time;
		return p;
	}

	fake_time::fake_time()
	: loaded(false)
	, idiff(0)
	{}

	time_t fake_time::get_time(void)
	{
		TRACE_FLOW
		if (!loaded) {
			if (db_check()) idiff = get_svi(db, dbn::TimeDiff);
			loaded = true;
		}
		TRACE_VAR(idiff)
		return curr_time + idiff;
	}
	void fake_time::set_time(time_t tc)
	{
		TRACE_FLOW
		int i = tc - curr_time;
		bool bdiff = (i != idiff);
		idiff = i;
		loaded = true;

	//	see if we can store that
		if (!db_check()) return;

	//	check if any data sets of time servers are set active
		db << db::from(dbn::TimeSrv) << db::where() << dbn::Active << db::nn();

	//	if given reset all to null
		if (db.count()) db << db::update(dbn::TimeSrv, c_db_wildcard) << dbn::Active << c_db_null << db::exec(true);

		if (bdiff) {
			TRACE_VAR(idiff)
			set_svi(db, dbn::TimeDiff, idiff, true);
		}
	}

//	============================================================
//	fake time cproc

	cp_fake_time::cp_fake_time()
	: time_setter(fake_time::instance())
	{
		rnd = 15;
	}


//	============================================================
//	cp_time_server
	cp_time_server::cp_time_server()
	: name(cdef)
	{}


	bool cp_time_server::my_check(sbuffer&)
	{
		TRACE_FLOW
		if (!db_check()) return false;
		db << db::from(dbn::TimeSrv) << dbn::Name << db::where() << dbn::Active << db::nn();
		t_cc v = db.getv();
		name.copy(v);
		return name.val();
	}

//!	puts out current time server name.
	void cp_time_server::put(sbuffer& sb)
	{
		check_once(sb);
		dml << name;
	}

//	============================================================
//	cp_time_servers
	void cp_time_servers::put(sbuffer& sb)
	{
		TRACE_FLOW
	//	frequently used: database table name
		t_cc cts =  dbn::TimeSrv;

	//	check for split & db connection
		if (!(split(cts, sb) && db_check())) return;

	//	reversed: show mac, title: name
		bool rev = false;
		if (sb && !strcmp(sb[0], c_reversed)) {
			rev = true;
			sb.shift();
		}

	//	select data:
	//	In this case we decide to select just all data:
	//	which are in order of appearence: Id, Name, Fixed, Acive
		db << db::from(cts) << db::order(rev) << dbn::Fixed; // << db::order() << dbn::Name;
		db::data* pd = db.get();

	//	if no data return undone
		if (!pd) return;


	//	required name objects
		cnb1 = cts;
		cnb3 = "r";

	//	active radio button id
		t_ch c_act = 0;
	//	active radio button done tracker
		bool bdone = false;

	//	get current radio buttons value
		if (bissue) c_act = cgi.getn(cts);

		if (bout) {
		//	begin output
			set_issue(cts);
			dml << tb() << id(cts)
				<< tr()
				<< th() << trans(c_Server)
				<< th() << trans(c_Address)
				<< th() << trans(c_Active)
				<< th() << trans(c_Remove)
			;
		}

		for (unsigned r = 1; r <= pd->rows(); ++r) {

		//	formally correct data indicator
			bool okf = true;

		//	add a table row and 1st td: just label with number
			if (bout) dml << tr() << td() << label() << r;

		//	2nd td: input (potentially read only)
			t_cc cid = pd->next();
			t_ch cnm = pd->next();
			bool bfx = pd->next();
			bool bac = pd->next();

		//	set id nob to id of row
			cnb2 = cid;

		//	if my issue and not fixed
			if (bissue && !bfx) {
			//	check for remove checkbox
				const s_cc& sr = cgi.getn(cnb3);
				if (sr.val()) {
				//	only interesting if value exists or active was set
					if (cnm || bac) {
						db << db::update(cts, cid) << dbn::Name << c_db_null << dbn::Active << c_db_null << db::pile();
						cnm = 0;
					}
				}
			//	check for value / active change
				else {
				//	value
					{
						const s_cc& sv = cgi.getn(cnb2);
						if (sv.val()) {
						//	change database value if valid and different
							okf = val::in(sv, val::host);
							if (okf && val::dif(cnm, sv, val::none)) db << db::update(cts, cid) << dbn::Name << sv << db::pile();
							cnm = sv;
						//	set input error if not ok format
							if (!okf) input_error = true;
						}
					}
				}
			}
		//	genrally if my issue
			if (bissue) {
			//	active
				{
					bool rac = okf && c_act && !bdone && cnm && !strcmp(c_act, cid);
					if (rac != bac) {
						db << db::update(cts, cid) << dbn::Active << (rac ? "1" : c_db_null) << db::pile();
						bac = rac;
					}
				}
			}
			if (bac) bdone = true;
			if (bout) {
				dml << td()
				//	formatter span
					<< span() << cl(bfx ? c_readonly : okf ? c_ok : c_nok)
				//	begin the input
					<< input() << type(c_text) << cl(c_host) << value(cnm)
				;
			//	if fixed add readonly mode
				if (bfx) dml << readonly();
			//	otherwise: give it a name
				else dml << name(cnb2);
			//	close formatter span
				dml << "" << tte;

			//	active radio button
				dml << td() << input() << name(cts) << type(c_radio) << value(cid);

			//	... and checked value
				if (bac) dml << checked();

			//	remove option
				if (!bfx) dml << td() << input() << name(cnb3) << type(c_checkbox); // << cl(c_remove);
			}
		} // table rows

	//	none active row
		if (bout) {
			dml << tr()
				<< td()
				<< td()
				<< td() << input() << name(cts) << type(c_radio) << value("X");
		//	set checked if no others were checked yet
			if (!bdone) dml << checked();
		}

	//	if an other was checked we check the current time diff setting and reset it potentially
		if (bdone && bissue && db.updates()) set_svi(db, dbn::TimeDiff, 0, false, false);

	//	end of table
		if (bout) dml << tbe();

	//	if changes were made: commit with trigger
		if (bissue) db.commit(true);

		delete pd;
	}

//	============================================================
//	cp_clock
	void cp_clock::put(sbuffer&)
	{
		time_t t = fake_time::instance()->get_time();
		struct tm* plt = localtime(&t);
		char* cb = new char[strlen(c_hms) + 1];
		strftime(cb, 9, c_hms, plt);
		dml << span() << id(cpnm) << cb << tte;
		delete cb;
	}

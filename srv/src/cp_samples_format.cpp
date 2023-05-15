//| ============================================================
//! @file
//!
//! A cproc sample.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================


#include <cp_samples_format.h>

// #undef DEVEL
#include <cp_convenience.h>

//	if no conflicting names: shorten up the typing
using namespace ta;
using namespace tag;
using namespace tbl;
using namespace bbl;
using namespace val;
using namespace db;


//	============================================================
//	cp_sample_format
	cp_sample_format::cp_sample_format()
	{
		strcpy(tbl, dbn::Sp);
	}

//!	check once: any data other than NULL.
	bool cp_sample_format::ok(sbuffer& sb)
	{
		TRACE_FLOW
		if (!sb) return false;
		t_cc cf = sb.shift();
		if (!db_check()) return false;
		strcpy(tbl + 2, cf);
		db << from(tbl) << dbn::Id << where() << dbn::Val << nn();
		return db.count();
	}

//!	Sample format class: PUT.
	void cp_sample_format::put(sbuffer& sb)
	{
		TRACE_FLOW
		if (!(sb && db_check())) return;

	//	set table name from call parameter
		t_cc cfrm = sb.shift();
		strcpy(tbl + 2, cfrm);

	//	select data:
	//	In this case we decide to select expected data: Id, Val
		db << from(tbl) << dbn::Id << dbn::Val;
		data* pd = db.get();

	//	if no or empty data return undone
		if (!pd) return;
		unsigned rows = pd->rows();

	//	check format
		int ifrm = key2frm(cfrm);

	//	check issue
		bool my_issue = is_issue(tbl);

	//	name object chain
	//	using frequently usable cnob chain provided by vars component
	//	update : "1_MAC"
	//	removal: "X_1_MAC"
		cnb1 = cfrm;
		cnb3 = "X";

	//	declare table name as issue
		set_issue(tbl);

	//	start with table and headings
		dml << tb() << id(tbl)
		//	setup: no caption
			<< th()
		//	Val: check for translation
			<< th() << cfrm
		//	Remove: check for translation
			<< th() << trans(c_Remove)
		;
	//	database and html table rows loop
		for (unsigned r = 0; r < rows; ++r) {
			dml << tr();

		//	with  each table row: set id nob to current id
			t_cc cid = pd->next();
			s_cc sv  = pd->next();
			cnb2 = cid;

		//	indicator: correct format received
			bool okf = true;

		//	1st td: just label with number 1 ... rows
			dml << td() << label() << (r + 1);

		//	if this is comming from a form:
		//	check for removal / data change
			if (my_issue) {
			//	check for non empty remove input (which should be "checked")
			//	X_1_MAC
				const s_cc& sr = cgi.getn(cnb3);
				if (sr.val()) {
				//	udate if value was given
					if (sv) db << update(tbl, cid) << dbn::Val << c_db_null << pile();
					sv = cdef;
				}
			//	no remove: check incomming data for change
				else {
				//	1_MAC
					const s_cc& sc = cgi.getn(cnb2);
				//	if given and different from given (unformatted)
					if (sc.val() && dif(sc, sv, none)) {
					//	check input correctness and format in data
						okf = in(sc, ifrm);
					//	valid input
					//	and: still different after formatting?
					//	(important check: saves useless triggers)
						if (okf && dif(sv, sc, ifrm)) db << update(tbl, cid) << dbn::Val << sc << pile();
					//	set new value anyhow
						sv = sc;
					}
				}
			}
			if (!sv) sv = cdef;
		//	value td
			dml << td()
				<< span() << cl(okf ? c_ok : c_nok)
				<< input() << name(cnb2) << type(c_text) << cl(cfrm) << size(17)
				<< value(out(sv, okf ? ifrm : none))
				<< tte;

		//	"remove" checkbox td (if value given)
			if (sv.val() && okf) dml << td() << input() << name(cnb3) << type(c_checkbox) << cl(c_remove);
		}
	//	end of table
		dml << tbe();

	//	if changes were made: commit with trigger
		if (my_issue) db.commit(true);

	//	clean up database data container
		delete pd;
	}

//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include <cp_samples_dbx.h>

#include <cp_convenience.h>

using namespace bbl;
using namespace db;
using namespace ta;
using namespace tag;

//	============================================================
//	cp_SVB

	cp_SVB::cp_SVB()
	: cm(8)
	{}

	bool cp_SVB::ok(sbuffer& sb)
	{
		return sb && ok(sb.shift());
	}

	bool cp_SVB::ok(t_cc k)
	{
	//	search key already buffered "" / "1"
		t_cc v = cm.getn(k);
		if (v) return v[0] == '1';
	//	if not: ask database
		if (!db_check()) return false;
		db << from(dbn::SVB) << dbn::Ok << where() << dbn::Name << k;
		bool okc = db.getv();
		cm.xset(k, (okc ? "1" : ""), true, true);
		return okc;
	}

	void cp_SVB::put(sbuffer& sb)
	{
		if (!sb) return;
		t_cc k = sb.shift();
		set_issue(dbn::SVB, k);

	//	current value
		bool okc = ok(k);

		cnb2 = k;
		cnb3 = dbn::SVB;

	//	returning value must opposite of current for change / update
		if (is_issue(dbn::SVB, k)) {
			cnb1 = okc ? c_Off : c_On;
			if (cgi.getn(cnb3)) {
				okc = !okc;
				db << update(dbn::SVB, dbn::Name, k) << dbn::Ok << (okc ? "1" : c_db_null) << exec(true);
				cm.xset(k, (okc ? "1" : ""), true, true);
			}
		}
	//	dml output
		t_cc con = okc ? c_On : c_Off;
		t_cc cof = okc ? c_Off : c_On;
		cnb1 = cof;
	//	outer divs class SVB, class submit
		dml << div() << cl(dbn::SVB) << div() << cl(c_submit)
		//	encapsulating span eneaysies css
		//	span with On / Off text
			<< span() << cl(con) << span() << trans(con) << tte << tte << cnl
		//	submit button
			<< input() << type(c_submit) << cl(c_submit) << name(cnb3) << value(trans(cof))
		//	end of divs
			<< tte << tte
		;
	}

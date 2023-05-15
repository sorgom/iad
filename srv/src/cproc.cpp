//| ============================================================
//! @file
//!
//!	Content processor class -
//!	the core class of modular content processing.
//!	And subsidiary features.
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================


#include <cproc.h>

#include <bbl.h>
#include <ccs.h>
#include <db.h>
#include <dml.h>
#include <cgi.h>
#include <val.h>
#include <vars.h>

//#undef DEVEL
#include <trace_macros.h>

#include <cstdlib>
#include <cstring>

//	============================================================
//	bswitch

//!	switch by keyword / no keyword
	void bswitch::set(sbuffer& sb)
	{
		if (sb) {
			t_cc v = sb[0];
			for (int i = 6; i--;) {
				if (strcmp(v, bswitch_vals[i]) == 0) {
					sb.shift();
					rb = (i < 3);
					return;
				}
			}
		}
		rb = true;
	}
//	============================================================
//	binfo
	void binfo::put(sbuffer&)
	{
		dml << trans(rb ? bbl::c_Yes : bbl::c_No);
	}

//	============================================================
//	cswitch
	void cswitch::set(sbuffer& sb)
	{
		if (sb) {
			t_cc v = sb[0];
			if (v[0] && !v[1]) {
				rc = v[0];
				sb.shift();
			}
		}
	}

//	============================================================
//	iswitch
	void iswitch::set(sbuffer& sb)
	{
		if (sb) {
			s_cc v;
			v.copy(sb[0]);
			if (val::in_num(v)) {
				ri = atoi(v);
				sb.shift();
			}
		};
	}
//	============================================================
//	uswitch
	void uswitch::set(sbuffer& sb)
	{
		if (sb) {
			s_cc v;
			v.copy(sb[0]);
			if (val::in_num(v)) {
				int i = atoi(v);
				if (i >= 0) {
					ri = i;
					sb.shift();
				}
			}
		};
	}
//	============================================================
//	sswitch
	void sswitch::set(sbuffer& sb)
	{
		if (sb) rs.copy(sb.shift());
		else rs = sd;
	}


//	============================================================
//	db_cproc
	bool db_cproc::db_chkd	= false;
	bool db_cproc::db_ok	= false;

	bool db_cproc::db_check()
	{
		TRACE_FLOW
		if (!db_chkd) {
			db_chkd = true;
			db_ok = db.connect(dbdir, dbnames);
		}
		TRACE_VARB(db_ok)
		return db_ok;
	}

//	============================================================
//	cp_splitter

	bool cp_splitter::split(t_cc c, sbuffer& sb)
	{
		TRACE_FLOW

	//	issue check
		bissue = is_issue(c);

	//	do we want html output
		bout = true;

	//	check for split indicating parameters
		if (sb && strlen(sb[0]) == 1) {
			bool r = true;
			switch (sb[0][0]) {
				case '0': bout   = false; break;
				case '1': bissue = false; break;
				default : r = false;
			}
			if (r) sb.shift();
		}
		TRACE_VARB(bissue)
		TRACE_VARB(bout)

		return bissue || bout;
	}
//	============================================================
//	cp_once
	bool cp_once::check_once(sbuffer& sb)
	{
		return once_chkd ? once_ok :
			(once_chkd = true) &&
			(once_ok = my_check(sb));
	}

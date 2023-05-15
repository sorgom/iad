//| ============================================================
//! @file
//!
//! Customized database handling extensions
//!	for a given database setup.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include <sample_dbx.h>

#include <dbn.h>
#include <cmapper.h>
#include <val.h>
#include <trace_macros.h>


#include <cstdlib>
#include <cstring>

//!	get a single int value by name.
	int get_svi(db::proc& db, t_cc n)
	{
		TRACE_FLOW
		db << db::from(dbn::SVI) << dbn::Num << db::where() << dbn::Name << n;
		t_cc cv = db.getv();
		return cv ? atoi(cv) : 0;
	}

//!	set a single int value by name.
	void set_svi(db::proc& db, t_cc n, int v, bool t, bool x)
	{
		TRACE_FLOW
		int vv = get_svi(db, n);
		if (vv != v) {
			db	<< db::update(dbn::SVI, dbn::Name, n)
				<< dbn::Num << v;
			if (x) db.commit(t);
			else db.pile();
		}
	}

	cmapper cmf(10);

//!	collect column name - format information for a table.
	void init_frm(db::proc& db, t_cc ct)
	{
		TRACE_FLOW
	//	standard translations
		cmf.clear();
		cmf.set(dbn::Id, 		dbn::NUM);
		cmf.set(dbn::Active, 	dbn::BOOL);
		cmf.set(dbn::Fixed, 	dbn::BOOL);
		cmf.set(dbn::Ok,	 	dbn::BOOL);
		cmf.set(dbn::Num,		dbn::NUM);
		cmf.set(dbn::Mac,		dbn::MAC);
	//	load explcit formats from S_FRM
		db << db::from(dbn::S_FRM) << dbn::Col << dbn::Frm << db::where() << dbn::Name << ct;
		db::data* pd = db.get();
		if (!pd) return;
		for (unsigned r = pd->cols(); r--;) cmf.set(pd->val(r, 0), pd->val(r, 1));
		delete pd;
	}

//! retrieve a format key (string) of a column name.
	t_cc col2key(t_cc cc)
	{
		t_ch ck = cmf.getn(cc);
		if (!ck) ck = cmf.getn("*");
		return ck ? ck : dbn::TEXT;
	}

//!	retrieve a format number of a format key.
	int key2frm(t_cc ck)
	{
			 if (!strcmp(ck, dbn::NUM )) return val::num;
		else if (!strcmp(ck, dbn::UNUM)) return val::unum;
		else if (!strcmp(ck, dbn::REAL)) return val::real;
		else if (!strcmp(ck, dbn::IP4 )) return val::ip4;
		else if (!strcmp(ck, dbn::MAC )) return val::mac;
		else if (!strcmp(ck, dbn::HOST)) return val::host;
		else if (!strcmp(ck, dbn::BOOL)) return val::num;

//		else if	(!strcmp(ck, dbn::TEXT)) return val::text;
		else return val::text;
	}

//! determination of boolean columns by name
	bool key2bool(t_cc ck)
	{
		return (!strcmp(ck, dbn::BOOL));
	}

//!	determination of editable columns by format key.
	bool key2edit(t_cc ck)
	{
		return (strcmp(ck, dbn::NOED));
	}

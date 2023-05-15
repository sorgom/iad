//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include <cp_samples_db.h>

#include <cp_convenience.h>

//	atoi
#include <cstdlib>

//	if no conflicting names: shorten up the typing
using namespace ta;
using namespace tag;
using namespace tbl;
using namespace val;

//!	general table viewer setting: use column names as table cell (td) classes.
	bool vtac = true;
//!	general table viewer setting: display column names as table headings (th).
	bool vtch = true;
//!	general table viewer setting: instant contents translation.
	bool vttr = false;

//!	NULL display string (default nothing).
	s_cc s_db_null = cdef;


//	============================================================
//	cp_view_column

	void cp_view_column::put(sbuffer& sb)
	{
		TRACE_FLOW
		if (sb.size() >= 2 && db_check()) {

		//	select data:
			db << db::from(sb.shift());
			db << sb.shift();
			db::data* pd = db.get();
		//	if no or empty data return undone
			if (pd) {
				for (unsigned i = pd->rows(); i--;) {
				//	database value
					const s_cc& sv = pd->next();
					if (sv) {
						if (vttr) dml << wtrans(sv);
						else dml << sv;
					}
					else dml << s_db_null;
				//	line break
					if (i) dml << br();
				}
				delete pd;
			}
		}
		vttr = false;
	}

//	============================================================
//	cp_view_table

	void cp_view_table::put(sbuffer& sb)
	{
		TRACE_FLOW
		if (sb && db_check()) {

		//	select data:
		//	- table name
			t_ch table_name = sb.shift();
			db << db::from(table_name);
		//	- specified columns
			while (sb) db << sb.shift();
			db::data* pd = db.get();

		//	if no or empty data return undone
			if (pd) {

				unsigned cols = pd->cols();

			//	OUR TABLE
				dml << tb() << cl(cpnm) << id(table_name);
			//	start with table headings: column names
				if (vtch) for (unsigned c = 0; c < cols; ++c) dml << th() << pd->col(c);

			//	cnobs for "dbc_Column" class formatting
				cnob cnc;
				cnob cnp(cnc, c_dbc);

			//	database and html table rows loop
				for (unsigned i = pd->rows(); i--;) {
					dml << tr();
				//	table columns loop
					for (unsigned c = 0; c < cols; ++c) {
					//	table cell class acc. to table column name
						if (vtac) {
							cnc = pd->col(c);
							dml << tda() << cl(cnp);
						}
					//	or just standard column
						else dml << td();
					//	database value
						const s_cc& sv = pd->next();
						if (sv) {
							if (vttr) dml << wtrans(sv);
							else dml << sv;
						}
						else dml << s_db_null;
					}
				}
			//	end of table
				dml << tbe();
			//	clean up database data container
				delete pd;
			}
		}
		vttr = false;
	}

//	============================================================
//	cp_tables

	void tblnk(t_cc ct, bool bc = false)
	{
		cmapper cmq(1);
		cbf1.clean() << c_table << ':' << ct;
		cmq.set(c_params, cbf1.str());
		if (bc) {
			cmapper cma(1);
			cma.set(c_class, c_current);
			link(ct, cmq, &cma);
		}
		else link(ct, cmq);
	}

	void cp_tables::put(sbuffer&)
	{
		TRACE_FLOW
		if (!db_check()) return;
		csbuffer sbt;
		if (!db.tables(sbt)) return;
	//	current table
		t_cc ctb = cgi.getn(c_table);
		TRACE_VARQ(ctb)
		dml << div() << cl(cpnm);
		bool b = false;
		for (unsigned i = 0; i < sbt.size(); ++i) {
			t_cc ct = sbt[i];
		//	all tables but system ones
			if (strncmp(ct, dbn::S_prefix, dbn::S_length)) {
				if (b) dml << cnl;
				tblnk(ct, ctb && !strcmp(ct, ctb));
				b = true;
			}
		}
		dml << tte;
	}

//	============================================================
//	cp_edit_table

//!	ID selector maker.
	void idsel(db::data* pd, const nob& nb, t_cc ci)
	{
		TRACE_FLOW
	//	select start and NULL option
		dml << select() << name(nb) << cnl
			<< option() << '0' << tte << cnl
		;
	//	Id options
	//	title display?
		bool bt = (pd->cols() == 2);
		pd->rewind();
		for (unsigned r = pd->rows(); r--;) {
			t_cc cr = pd->next();
			dml << option();
		//	id given and matching?
			if (ci && !strcmp(cr, ci)) dml << selected();
		//	title?
			if (bt) {
				t_cc ctt = pd->next();
				if (ctt) dml << title(format(ctt));
				else dml << title("?");
			}
		//	number to sxelect
			dml << cr << tte << cnl;
		}
		dml << tte;
	}

//!	boolean selector maker.
	void boolsel(const nob& nb, t_cc cb)
	{
		TRACE_FLOW
		dml << input() << type(c_checkbox) << name(nb);
		if(cb) dml << checked();
		dml << cnl;
	}

//!	the call.
	void cp_edit_table::put(sbuffer& sb)
	{
		if (!(sb && db_check())) return;
	//	get all table names (needed for reference decisions)
		csbuffer sbt;
		if (!db.tables(sbt)) return;
	//	table name
		t_cc ct = sb.shift();
	//	check if table is not resizeable.
		db << db::from(dbn::S_NRS) << db::where() << dbn::Name << ct;
		bool okrz = !db.count();

	//	check for issue
		bool my_issue = is_issue(cpnm);
		TRACE_VARB(my_issue)

	//	process add row / removal
		if (my_issue) {
		//	Add
			if (cgi.getn(bbl::c_Add)) {
				db.addset(ct);
				my_issue = false;
			}
		//	check for remove
			else {
				db << db::from(ct) << dbn::Id;
				db::data* pd = db.get();
				if (pd) {
					for (unsigned i = pd->rows(); i--;) {
						t_cc cid = pd->next();
						if (cgi.getn(cid)) {
							db << db::remove(ct, cid) << db::exec(true);
							my_issue = false;
							break;
						}
					}
					delete pd;
				}
			}
		}

	//	get all data (with forced column names option set)
		db << db::from(ct);
		db::data* pd = db.get(true);
		if (!pd) return;

		unsigned cols = pd->cols();
		unsigned rows = pd->rows();

	//	column wise preparation of format, format key and Id referrer preselections
		init_frm(db, ct);

	//	indicators whether a column is a reference
		bool*		rr		= new bool[cols];
	//	reference selection data containers
		db::data**	rcols	= new db::data*[cols];
	//	format keys
		t_ch*		rkeys	= new t_ch[cols];
	//	format numbers
		int*		rfrms	= new int[cols];
	//	number of column containing Id
		int			nid		= -1;
	//	indicators whether a column is boolean
		bool*		cbools	= new bool[cols];
	//	indicators whether a column is editable
		bool*		cedit	= new bool[cols];

	//	get all these values together
		for (unsigned i = 0; i < cols; ++i) {
			t_cc cc 	= pd->col(i);
			rr[i] 		= false;
			rcols[i] 	= 0;
		//	column containing Id
			if (!strcmp(cc, dbn::Id)) {
				cedit[i] = false;
				nid = i;
				continue;
			}
		//	format key and number
			t_cc ck 	= col2key(cc);
			rkeys[i] 	= ck;
			rfrms[i] 	= key2frm(ck);
			cbools[i]	= key2bool(ck);
			cedit[i]	= key2edit(ck);

		//	if a field is not editable then the table is not resizeable
			if (!cedit[i]) okrz = false;

		//	if boolean or not editable: no further examination
			if (cbools[i] || !cedit[i]) continue;

		//	refrences to other tables
			rr[i] = sbt.find(cc);
			if (!rr[i]) rcols [i] = 0;
			else {
				db::data* pt;
			//	try with names
				db << db::from(cc) << dbn::Id << dbn::Name;
				pt = db.get(true);
			//	if not then without names
				if (!pt) {
					db << db::from(cc) << dbn::Id;
					pt = db.get(true);
				}
				rcols [i] = pt;
			}
		}

	//	dml ouput
	//	declare cproc name as issue
		set_issue(cpnm);

	//	table and headings
		dml << tb() << cl(cpnm) << id(ct) << tr();
	//	start with table headings: column names
		for (unsigned c = 0; c < cols; ++c) {
			dml << th();
			if (rr[c]) tblnk(pd->col(c));
			else dml << pd->col(c);
		}
	//	if table is resizeable: add new data set button
		if (okrz) {
			TRACE_INF("adding add")
			dml << td()
				<< input() << type(c_submit) << name(bbl::c_Add) << cl(bbl::c_Add) << value("+") << title(trans(bbl::c_Add));
		}
	//	name objects for column / row count
	//	column: e.g. "Name"
		cnob nbc;
	//	id + column: e.g. 2_Name
		cnob nbid(nbc, cdef);
	//	cnobs for "dbc_Column" class formatting
		cnob nbd(nbc, c_dbc);

	//	row wise contents
		for (unsigned r = 0; r < rows; ++r) {
			TRACE_INF("row: " << r)
			dml << tr();
			t_cc cid = nid > -1 ? pd->val(r, nid).str() : 0;
			nbid = cid;

		//	column wise ..
			for (unsigned c = 0; c < cols; ++c) {
				t_cc cc = pd->col(c);
				TRACE_INF("col: " << c << " name: " << cc)
				s_cc cv = pd->val(r, c);
				nbc = cc;
				int format = rfrms[c];
				TRACE_VAR(format)

			//	check for incomming data
			//	indicator: correct format received
				bool okf = true;
				if (my_issue && cid && cedit[c]) {
					const s_cc& sc = cgi.getn(nbid);
					if (dif(sc, cv, none)) {
						TRACE_INF("value difference: " << cc)
					//	boolean evaluation (doesn't have to be format checked)
						if (cbools[c]) {
							TRACE_INF("updating bool")
							if		(sc && !cv) db << db::update(ct, cid) << cc << 1 << db::pile();
							else if (!sc && cv) db << db::update(ct, cid) << cc << c_db_null << db::pile();
						}
					//	referrers (don't have to be format checked)
						else if (rcols[c]) {
							TRACE_INF("updating reference")
							if		(sc[0] == '0' && cv) db << db::update(ct, cid) << cc << c_db_null << db::pile();
							else if (sc[0] != '0') db << db::update(ct, cid) << cc << sc << db::pile();
						}
					//	others have to
						else if (sc.val()) {
							TRACE_INF("updating value")
							okf = in(sc, format);
						//	valid input
						//	still different after formatting?
						//	(important check: saves useless triggers)
							if (okf && dif(cv, sc, format)) db << db::update(ct, cid) << cc << sc << db::pile();
						}
					//	null input
						else if (cv) {
							TRACE_INF("clearing value")
							db << db::update(ct, cid) << cc << c_db_null << db::pile();
						}
					//	set new value
						cv = sc;
					}
				} // incomming data

				if (!cv && !cbools[c] && !rcols[c]) cv = cdef;


				TRACE_INF("output")

			//	Id field?
				if (c == nid) {
					TRACE_INF("- Id field")
					dml << tda() << cl(dbn::Id) << cid;
				}
			//	not editable / Id field?
				else if (!cedit[c]) {
					TRACE_INF("- not editable")
					dml << td()
						<< input() << type(c_text) << size(10) << readonly() << cl(c_readonly) << value(cv);
				}
			//	a boolean?
				else if (cbools[c]) {
					TRACE_INF("- boolean")
					dml << td();
					boolsel(nbid, cv);
				}
			//	a referrer to Id?
				else if (rcols[c]) {
					TRACE_INF("- referrer")
					dml << td();
					idsel(rcols[c], nbid, cv);
				}
				else {
					TRACE_INF("- formatted")
					if (!cv) cv = cdef;
					dml << tda() << cl(rkeys[c]) << span() << cl(okf ? c_ok : c_nok)
						<< input() << type(c_text) << size(10) << cl(nbd) << value(out(cv, okf ? format : none));
					if (cid) dml << name(nbid);
					dml	<< tte;
				}

				TRACE_INF("end column " << c)
			} // columns

		//	if table is resizeable: add remove button
			if (okrz && cid) {
				TRACE_INF("adding remove")
				dml << td() << input() << type(c_submit) << name(cid) << cl(c_remove) << value("X") << title(trans(bbl::c_Remove));
			}

		} // rows
	//	if table is resizeable: add new data set button
//		if (okrz) {
//			TRACE_INF("adding add")
//			dml << tr() << tda() << colspan(cols + 1) << align(c_right)
//				<< input() << type(c_submit) << name(bbl::c_Add) << cl(bbl::c_Add) << value("+") << title(trans(bbl::c_Add));
//		}
	//	end of table
		TRACE_INF("end of table")
		dml << tbe();

		TRACE_INF("db.commit")
		if (my_issue) db.commit(true);

	//	remove heap vars
		for (unsigned i = 0; i < cols; ++i) if (rcols[i]) delete rcols[i];
		delete rcols;
		delete rr;
		delete rkeys;
		delete rfrms;
		delete cbools;
		delete cedit;
	//	and data container
		delete pd;
	}

//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include <cp_sample_macfilter.h>

#include <cp_convenience.h>

//	if no conflicting names: shorten up the typing
using namespace ta;
using namespace tag;
using namespace tbl;
using namespace bbl;
using namespace val;
using namespace db;

//	============================================================
//	cp_macfilter

	cp_macfilter::cp_macfilter()
	: tbl(dbn::MacFlt)
	{}

	bool cp_macfilter::my_check(sbuffer&)
	{
		TRACE_FLOW
		if (is_issue(tbl)) {
			if (cgi.getn(c_Off)) return false;
			if (cgi.getn(c_On )) return true;
		}
		if (!db_check()) return false;
		db << from(dbn::SVB) << dbn::Ok << where() << dbn::Name << tbl;
		return db.getv();
	}

//!	Sample base class: PUT.
	void cp_macfilter::put(sbuffer& sb)
	{
		TRACE_FLOW
		if (!db_check()) return;

	//	reversed: show mac, title: name
		bool rev = false;
		if (sb && !strcmp(sb[0], c_reversed)) {
			rev = true;
			sb.shift();
		}

	//	(check if active)
		bool bon = check_once(sb);

	//	check for issue
		bool my_issue = is_issue(tbl);

	//	preset values for editing
		s_cc sname 	= cdef;
		s_cc smac	= cdef;
		s_cc sid	= cdef;
		bool bname 	= false;
		bool bmac	= false;
		bool okname = true;
		bool okmac	= true;
		bool bsid 	= false;
	//	multi select string buffer
		sbuffer sbms;
	//	anything selected
		bool bsel = false;
	//	analyze incomming data
		if (my_issue) {
		//	anything selected
			bsel	= cmvals(sbms, cgi, c_select);
		//	action mode by button pressed
			enum {
				m_none,
			//	activation button pressed
				m_acti,
			//	deactivation button pressed
				m_deact,
			//	edit button pressed
				m_edit,
			//	remove button pressed
				m_remove,
			//	add button pressed
				m_add,
			//	save button pressed
				m_save
			} amode = m_none;
			amode = (
				cgi.getn(c_Activate		) ? m_acti	:
				cgi.getn(c_Deactivate	) ? m_deact	:
				cgi.getn(c_Edit			) ? m_edit	:
				cgi.getn(c_Remove		) ? m_remove:
				cgi.getn(c_Add			) ? m_add	:
				cgi.getn(c_Save			) ? m_save	:
				m_none
			);
		//	action by mode
			switch (amode) {
			//	useless trigger prevention:
			//	if activation / deactivation button pressed
			//	retieve active states of entries
				case m_acti:
				case m_deact: {
					if (!bsel) break;
					bool bsa = (amode == m_acti);
					db << from(tbl) << dbn::Id << where() << dbn::Active << nn();
					data* pd = db.get();
					while (sbms) {
						t_cc cid = sbms.shift();
						bool bia = pd && pd->find(cid);
						if (bsa != bia) db << update(tbl, cid) << dbn::Active << (bsa ? "1" : c_db_null) << pile();
					}
					db.commit(true);
					if (pd) delete pd;
				}
				break;
				case m_remove:
					if (!bsel) break;
					while (sbms) db << remove(tbl, sbms.shift()) << pile();
					db.commit(true);
				break;
				case m_edit:
					if (!bsel) amode = m_none;
					else {
						db << from(tbl) << dbn::Name << dbn::Mac << where() << dbn::Id << sbms[0];
						data* pdt = db.get();
						if (!pdt) amode = m_none;
						else {
							sid.	copy(sbms[0]);
							sname.	copy(pdt->next());
							smac.	copy(pdt->next());
							bname 	= sname.val();
							bmac	= smac.val();
							delete pdt;
						}
					}
				break;
			//	user value input: add / save
				case m_save:
					sid = cgi.getn(dbn::Id);
				case m_add:
				{
					sname	= cgi.getn(dbn::Name);
					smac	= cgi.getn(dbn::Mac);
					bname 	= sname.val();
					bmac	= smac.val();
					okname	= bname && in(sname, text);
					okmac	= bmac	&& in(smac, mac);
					bool ok = false;
				//	if both passsed valid: update or insert
					if (okmac && okname) {
					//	save existing entry
						if (amode == m_save) {
							ok = sid.val();
							if (ok) {
							//	check if changes where made (save triggers).
								db << from(tbl) << dbn::Name << dbn::Mac << where() << dbn::Id << sid;
								data* pdt = db.get();
								if (pdt) {
									ok = dif(sname, pdt->next(), text) || dif(smac, pdt->next(), mac);
									delete pdt;
								}
								if (ok) {
									db << update(tbl, sid) << dbn::Name << sname << dbn::Mac << smac;
									ok = db.commit(true);
								}
							}
						}
					//	add new entry
						else {
							TRACE_INF("launch of insert")
							db << insert(tbl) << dbn::Name << sname << dbn::Mac << smac << dbn::Active << 1;
							ok = db.commit(true);
							if (ok) {
								t_cc nid = db.lid(tbl);
								TRACE_INF("NEW ID: " << nid)
								sid.copy(nid);
							}
						}
					}
				}
				break;
				default:
				break;
			} // action by mode
			bsid = sid.val();
		} // incomming data

	//	select data:
	//	In this case we decide to select data explicitly
	//	to make sure we have the right order.
	//	which are in order of appearence:
	//	0 Id
	//	1 Name
	//	2 Mac
	//	3 Active
		db	<< from(tbl)
			<< dbn::Id << dbn::Name << dbn::Mac << dbn::Active
			<< order() << (rev ? dbn::Mac : dbn::Name)
		;
		data* pd = db.get();
		unsigned rows = (pd? pd->rows() : 0);

//	============================================================
//	dml output
	//	create a section div for each section to give more ability to css formatting:
	//	id=MacFlt_1, id=MacFlt_2 ...
		inob nbdc(1);
		cnob nbdiv(nbdc, tbl);

	//	usage of commonly accesable nobs for button class
		cnb2 = c_btn;

	//	the very outer div: id table name
		dml << div() << id(tbl);

	//	section div close and open: selection of MAC addresses
		++nbdc;
		dml << div() << id(nbdiv) << cnl
	//	select headline
			<< p() << trans(c_Trusted_Nodes) << tte
	//	start multiple select
			<< select() << name(c_select) << multiple() << size(8) << cl(tbl) << cnl;

	//	create options if any data available
		bool oks = true;
		for (unsigned r = 0; r < rows; ++r) {

		//	check for missing values (possible if database editor was used)
			if (!(pd->val(r, 1) && pd->val(r, 2))) continue;

			bool ba = pd->val(r, 3);
			char ca = ba ? '+' : '-';
			dml << option()
			//	option value is database Id
				<< value(pd->val(r, 0))
			//	option class defined by Filter Active and database Active
				<< cl(bon && !ba ? c_nok : c_ok);
			//	selected if editing
				if (oks && bsid && !strcmp(pd->val(r, 0), sid)) {
					dml << selected();
					oks = false;
				}
		//	display and title
			if (rev)	dml << title(format(pd->val(r, 1))) <<  ca << ' ' << pd->val(r, 2);
			else 		dml << title(pd->val(r, 2)) << ca << ' ' << format(pd->val(r, 1));
		//	option end
			dml	<< tte << cnl;
		}
	//	end of multiple select
		dml << tte << cnl;


	//	if any data selectable: create process options: aktivate, deaktivate, edit, remove
	//	submit div
		dml << div() << cl(c_submit) << cnl;
	//	option buttons
		{
			sbuffer sbb;
			sbb << c_Activate << c_Deactivate << c_Edit << c_Remove;
		//	2 buttons line break
			unsigned ibr = 0;
			while (sbb) {
				t_cc c = sbb.shift();
				cnb1 = c;
				dml << input() << type(c_submit) << name(c) << value(trans(c)) << cl(cnb2);
			//	2 buttons line break
				if (++ibr % 2) dml << cnl;
				else dml << br();
			}
		}
	//	close submit div
	 	dml	<< tte << cnl;

	//	section div close and open
		++nbdc;
		dml << tte << div() << id(nbdiv) << cnl;

/*	paragraph and line break version
	//	name and mac address edit fields
		dml << p() << trans(dbn::Name) << br()
			<< span() << cl(okname ? c_ok : c_nok)
			<< input() << type(c_text) << name(dbn::Name) << value(format(sname)) << cl(c_text)
			<< tte << tte
			<< p() << trans(dbn::Mac) << br()
			<< span() << cl(okmac ? c_ok : c_nok)
			<< input() << type(c_text) << name(dbn::Mac) << value(smac) << cl(c_mac)
			<< tte << tte;
*/
	//	table version
		dml << tb()
			<< td()  << trans(dbn::Name)
			<< tda() << cl(okname ? c_ok : c_nok)
			<< input() << type(c_text) << name(dbn::Name) << value(format(sname)) << cl(c_text)
			<< tr()
			<< td()  << trans(dbn::Mac)
			<< tda() << cl(okmac ? c_ok : c_nok)
			<< input() << type(c_text) << name(dbn::Mac) << value(smac) << cl(c_mac)
			<< tbe()
		;


	//	add / save button
	//	submit div
		dml << cnl << div() << cl(c_submit) << cnl;
	//	button: Clear
		if (bname || bmac) {
			cnb1 = c_Clear;
			dml << input() << type(c_submit) << name(c_Clear) << value(trans(c_Clear)) << cl(cnb2) << cnl;
		}
	//	button: Add / Save
		t_cc cap = (bsid ? c_Save : c_Add);
		cnb1 = cap;
		dml << input() << type(c_submit) << name(cap) << value(trans(cap)) << cl(cnb2) << cnl;

	//	close submit div, section div, very outer div
		dml	<< tte << tte << tte << cnl;

	//	if id given: save for next round
		if (bsid) dml << input() << type(c_hidden) << name(dbn::Id) << value(sid);

	//	declare table name as issue
		set_issue(tbl);

	//	delete data pointer
		if (pd) delete pd;
	}

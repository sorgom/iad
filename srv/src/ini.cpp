//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include "ini.hpp"

#include "ccs.hpp"
#include "f_io.hpp"
#include "finders.hpp"
#include "hdn_vars.hpp"
#include "ini_io.hpp"
#include "sfunc.hpp"
#include "utilz.hpp"
#include "val.hpp"
#include "vars.hpp"

// #undef DEVEL
// #define XDEVEL
#include "trace_macros.hpp"

#include <cstdio>

	t_cc cerrf = "%s%s(%s) %s\n";

//	directory check and error output
	bool chkdir(t_cc cd, t_cc ci)
	{
		if (isdir(cd)) return true;
		fprintf(stderr, cerrf, msg_error, msg_no_directory, ci, cd);
		return false;
	}

	bool load_ini(t_cc fn)
	{
		TRACE_FLOW
		hdn::ini.clear();
		load_ini_file(hdn::ini, fn, false);
		hdn::ini.sort();

	//	complete setup with defaults
		hdn::ini.xadd(c_layout,	c_layout_default);
		hdn::ini.xadd(c_lang,		c_lang_default);

		setcc(common, ini.getk(c_common));

	//	set frequently accessed directories
		setcc(tmpdir,	ini.getd(c_tmpdir,	c_tmpdir_default	));
		setcc(dbdir,	ini.getd(c_dbdir,	c_dbdir_default		));
		setcc(htmdir,	ini.getd(c_htmdir,	c_htmdir_default	));
		setcc(langdir,	ini.getd(c_langdir,	c_langdir_default	));

		bool ok = true;
	//	when server: check all diretories for correctnes
#ifdef ISSRV
		{
			ok &= chkdir(tmpdir,	c_tmpdir);
			ok &= chkdir(dbdir,		c_dbdir);
			ok &= chkdir(htmdir,	c_htmdir);
			ok &= chkdir(langdir,	c_langdir);
			if (!ok) return false;
		}
		hdn::ini.set(c_droot, cdef);
#endif
	//	html document path extender
		setcc(droot, ini.get(c_droot));

	//	database names
		hdn::dbnames.clear();
		t_ch dbns = ini.getd(c_dbnames, c_dbnames_default);
		split(dbns, hdn::dbnames);

	//	standard content type for parsed content
		setcc(content_type, ini.getd(c_content_type, c_text_html));

	//	poly layout selection allowed
		hdn::poly = ini.getn(c_poly);

	//	number of floating point digits
		if (!val::is_num(ini.getn(c_digits), true, true)) hdn::ini.set(c_digits, "2");

	//	languages
		{
			hdn::langs.clear();
			languagefinder* plf = new languagefinder(true);
			for (unsigned i = 0; i < plf->size(); ++i) {
			//	short sign
				t_cc cs = plf->at(i);
			//	native long name
				t_cc cl = ini.getn(cs);
				if (cl) hdn::langs.copyv(cl, cs, false);
			}
			delete plf;
			hdn::langs.sort();
		}
	//	layouts
		{
			hdn::layouts.clear();
			layoutfinder* plf = new layoutfinder(true);
			for (unsigned i = 0; i < plf->size(); ++i) {
			//	short name
				t_cc cs = plf->at(i);
			//	long desc
				t_cc cl = ini.getn(cs);
				if (cl) {
					cbf1.clean();
				//	untag entry e.g. "Geier & Habicht" -> "Geier &amp; Habicht"
					frmout(cl, cbf1);
					hdn::layouts.copy(cs, cbf1.str(), false);
				//	remove layout descriptions from ini
					hdn::ini.remove(cs);
				}
			}
			delete plf;
			hdn::layouts.sort();
		}
	//	other finders
		htmfinder* const pf = const_cast<htmfinder* const>(htmfinder::instance());
		ok &= pf->init();
		return ok;
	}

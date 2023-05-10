//| ============================================================
//! @file
//!
//! The balel key value translator class.
//! (#cmapper class extended by file load)
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include <iostream>

#include "bbl_mapper.hpp"

#include "ccs.hpp"
#include "defs.hpp"
#include "ini_io.hpp"
#include "vars.hpp"

// #undef DEVEL
// #define XDEVEL
#include "trace_macros.hpp"

#include <cstdlib>
#include <cstring>

//	langauge data expiry time
#define c_LANG_DATA_EXPIRY 15

namespace bbl
{
//	============================================================
//	common
//	============================================================
	dirman lman;

//!	bbl directory manager init call.
	bool bblinit(void)
	{
		return lman.scan(langdir, c_ext_dat, true);
	}

//!	language / layout order dependent path and file assembly.
	void ll2b(cbuffer& buf, t_cc lang, t_cc lay)
	{
#ifdef c_BBL_LAY_LANG
		buf.clean() << lay  << c_sep << lang;
#else
		buf.clean() << lang << c_sep << lay;
#endif
	}

//!	common cmapper language / layout literals load.
	bool bload(cmapper& cm, t_cc lang, t_cc lay, t_cc csec = 0)
	{
		TRACE_FLOW
		TRACE_VARQ(lang)
		TRACE_VARQ(lay)

		cm.clear();
		cbuffer bfl(c_BUF_STEP_FILE_NAME);
		bool ok = false;
		t_ch cfl;

	//	common part
		ll2b(bfl, lang, common);
		TRACE_VARQ(bfl)
		ok = (cfl = lman.ff(bfl)) && load_ini_file(cm, cfl, ok, csec);
		cm.sort();

	//	layout dependent part
		ll2b(bfl, lang, lay);
		TRACE_VARQ(bfl)
		ok |= (cfl = lman.ff(bfl)) && load_ini_file(cm, cfl, ok, csec);
		cm.sort();

	//	if not "complete": load "common".dat
		ok |= (
			cm.getn(c_complete) ||
			((cfl = lman.ff(common)) && load_ini_file(cm, cfl, ok, csec))
		);
		cm.sort();

		TRACE_VAR(cm.size())
		return ok;
	}

//	============================================================
//	CGI interface
//	============================================================
#ifndef ISSRV

	void load(cmapper& cm)
	{
		TRACE_FLOW
		bload(cm, curr_lang, curr_lay);
	}

//!	language instance of cmapper
	cmapper mapper(c_BUF_STEP_BBL, &load);

//	============================================================
//	HTTP server multi language manager
//	============================================================
#else

//	============================================================
//	bpair
//	extended language and layout search map representation.
	class bpair
	{
	public:
		s_cc lang;
		s_cc lay;
		bool main;
		cmapper cm;
		time_t tst;
	};

//	============================================================
	typedef const bpair* const t_sbp;


//!	compare function for qsort / bsearch
	int bcomp(const void* p1, const void* p2)
	{
		t_sbp s1 = *reinterpret_cast<t_sbp*>(p1);
		t_sbp s2 = *reinterpret_cast<t_sbp*>(p2);

		if (s1 && s2) {
			if (int i = strcmp(s1->lang, s2->lang)) return i;
			return strcmp(s1->lay, s2->lay);
		}
		return s1 ? -1 : s2 ? 1 : 0;
	}

//	============================================================
//	bmapper
	bmapper::bmapper()
	: lcm(0)
	, cmm(256)
	, cml(10)
	, smlg(cdef)
	, smly(cdef)
	, sllg(cdef)
	, slly(cdef)
	{}

//!	set current language and layout.
	bool bmapper::set(t_cc lang, t_cc lay, bool main)
	{
		TRACE_FLOW
		TRACE_VARQ(lang)
		TRACE_VARQ(lay)
		TRACE_VARB(main)
	//	set and find attempts
	//	a	if internal main language / layout matched:
	//		direct to main mapper (regarless of request main)
	//	b	if request is not main and internal login matched:
	//		set login
	//	c	otherwise create and set main / login

	//	a
		if (!strcmp(lang, smlg) && !strcmp(lay, smly))
			lcm = &cmm;
	//	b
		else if (!main && !strcmp(lang, sllg) && !strcmp(lay, slly))
			lcm = &cml;
	//	c
		else if (main) {
			lcm = &cmm;
			smlg.copy(lang);
			smly.copy(lay);
			bload(cmm, lang, lay);
		}
		else {
			lcm = &cml;
			sllg.copy(lang);
			slly.copy(lay);
			bload(cml, lang, lay, c_login);
		}
		return true;
	}

//!	retreive a translation
//!	(with key / cdef return in case of not found).
	t_cc bmapper::getk(t_cc key)
	{
		TRACE_FLOW
		TRACE_ADDR(lcm)
		TRACE_VARQ(key)
		return lcm ? lcm->getk(key) : key ? key : cdef;
	}

//!	retreive a translation
//!	(with NULL return in case of not found).
	t_cc bmapper::getn(t_cc key)
	{
		TRACE_FLOW
		return lcm ? lcm->getn(key) : 0;
	}

//!	the instance as bbl::mapper
//!	which is exchangealbe with bbl::mapper defined in bbl_mapper.hpp.
	bmapper mapper;

#endif // CGI / HTTP server

} // namespace bbl

//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include <finders.h>

#include <ccs.h>
#include <defs.h>
#include <ini.h>
#include <utilz.h>
#include <vars.h>

#undef DEVEL
// #define XDEVEL
#include <trace_macros.h>


#include <cstring>

//	============================================================
//	languagefinder

	languagefinder::languagefinder(bool f)
	: csbuffer(c_BUF_STEP_LANGS)
	, lc(strlen(common))
	{
		TRACE_FLOW
		TRACE_VAR(lc)
		if (f) operator()();
	}

	void languagefinder::operator()(t_cc cd)
	{
		TRACE_FLOW
#ifdef c_BBL_LAY_LANG
		if (strncmp(cd, common, lc)) return;
		t_ch c1 = cd + lc;
		if (*c1 != c_sep) return;
		++c1;
		t_ch c2 = c1;
		while (is_alpha(*c2)) ++c2;
		if (c2 == c1 || strcmp(c2, c_ext_dat)) return;
		copy(c1, c2 - c1);
#else
		t_ch c1 = cd;
		while (is_alpha(*c1)) ++c1;
		if (c1 == cd || *c1 != c_sep) return;
		t_ch c2 = c1 + 1;
		if (
			strncmp(c2, common, lc) ||
			strmp(c2 + lc, c_ext_dat)
		) return;
		copy(cd, c1 - cd);
#endif
	}

	void languagefinder::operator()()
	{
		TRACE_FLOW
		clear();
		lsdir(langdir, tp_file, *this);
	}

//	============================================================
//	layoutfinder

	layoutfinder::layoutfinder(bool f)
	: csbuffer(c_BUF_STEP_LAYOUTS)
	{
		if (f) operator()();
	}

	void layoutfinder::operator()(t_cc cd)
	{
		if (
			bc_all(&is_alphanum, cd) &&
			strcmp(cd, common)
		)	copy(cd);
	}

	void layoutfinder::operator()()
	{
		TRACE_FLOW
		clear();
		lsdir(htmdir, tp_dir, *this);
	}

//	============================================================
//	htmfinder

	const htmfinder* const htmfinder::instance()
	{
		static htmfinder* pf = new htmfinder;
		return pf;
	}

	bool htmfinder::init()
	{
		return scan(htmdir, c_ext_htm, true);
	}

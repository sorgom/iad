//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include <cpreg.h>

//	cproc inclusions (generated).
#include <cp_sample_ilanguage.h>
#include <cp_sample_macfilter.h>
#include <cp_sample_online.h>
#include <cp_samples_ajax.h>
#include <cp_samples_db.h>
#include <cp_samples_dbx.h>
#include <cp_samples_format.h>
#include <cp_samples_htm.h>
#include <cp_samples_time.h>
#include <cp_vars.h>
#include <err_inf.h>
#include <system_cprocs.h>

#include <trace_macros.h>
#include <cstdlib>

#define c_NR_CPROCS 56

//!	#cproc instance creater function type.
	typedef cproc* (ccproc)(t_cc);

//!	a cprocs regististry entry.
	struct cprg
	{
		t_cc k;
		cproc* p;
		ccproc* cc;
	};

//!	standard template for creater function definition.
	template <class T>
	T* mkcp(t_cc n)
	{
		TRACE_FLOW
		T* cp = new T;
		t_ch& ch = const_cast<t_ch&>(cp->cpnm);
		ch = n;
		TRACE_ADDR(cp)
		TRACE_VARQ(cp->cpnm)
		return cp;
	}

//!	cproc creator functions (generated).
	cproc* mkcp0001(t_cc n) { return mkcp< cp_DEF                  >(n); }
	cproc* mkcp0002(t_cc n) { return mkcp< cp_DEVEL                >(n); }
	cproc* mkcp0003(t_cc n) { return mkcp< cp_HOST                 >(n); }
	cproc* mkcp0004(t_cc n) { return mkcp< cp_JE                   >(n); }
	cproc* mkcp0005(t_cc n) { return mkcp< cp_JS                   >(n); }
	cproc* mkcp0006(t_cc n) { return mkcp< cp_JWE                  >(n); }
	cproc* mkcp0007(t_cc n) { return mkcp< cp_JWS                  >(n); }
	cproc* mkcp0008(t_cc n) { return mkcp< cp_LL                   >(n); }
	cproc* mkcp0009(t_cc n) { return mkcp< cp_NULL                 >(n); }
	cproc* mkcp0010(t_cc n) { return mkcp< cp_SRV                  >(n); }
	cproc* mkcp0011(t_cc n) { return mkcp< cp_SVB                  >(n); }
	cproc* mkcp0012(t_cc n) { return mkcp< cp_ajax_able            >(n); }
	cproc* mkcp0013(t_cc n) { return mkcp< cp_ajax_updater         >(n); }
	cproc* mkcp0014(t_cc n) { return mkcp< cp_autoclass            >(n); }
	cproc* mkcp0015(t_cc n) { return mkcp< cp_begin_ajax           >(n); }
	cproc* mkcp0016(t_cc n) { return mkcp< cp_clock                >(n); }
	cproc* mkcp0017(t_cc n) { return mkcp< cp_current              >(n); }
	cproc* mkcp0018(t_cc n) { return mkcp< cp_edit_table           >(n); }
	cproc* mkcp0019(t_cc n) { return mkcp< cp_end_ajax             >(n); }
	cproc* mkcp0020(t_cc n) { return mkcp< cp_errors               >(n); }
	cproc* mkcp0021(t_cc n) { return mkcp< cp_fake_online_state    >(n); }
	cproc* mkcp0022(t_cc n) { return mkcp< cp_fake_time            >(n); }
	cproc* mkcp0023(t_cc n) { return mkcp< cp_false                >(n); }
	cproc* mkcp0024(t_cc n) { return mkcp< cp_fdigits              >(n); }
	cproc* mkcp0025(t_cc n) { return mkcp< cp_fpoint               >(n); }
	cproc* mkcp0026(t_cc n) { return mkcp< cp_ilanguages           >(n); }
	cproc* mkcp0027(t_cc n) { return mkcp< cp_infos                >(n); }
	cproc* mkcp0028(t_cc n) { return mkcp< cp_is_ajax              >(n); }
	cproc* mkcp0029(t_cc n) { return mkcp< cp_language             >(n); }
	cproc* mkcp0030(t_cc n) { return mkcp< cp_layout               >(n); }
	cproc* mkcp0031(t_cc n) { return mkcp< cp_login                >(n); }
	cproc* mkcp0032(t_cc n) { return mkcp< cp_macfilter            >(n); }
	cproc* mkcp0033(t_cc n) { return mkcp< cp_page_help            >(n); }
	cproc* mkcp0034(t_cc n) { return mkcp< cp_page_help_default    >(n); }
	cproc* mkcp0035(t_cc n) { return mkcp< cp_page_path            >(n); }
	cproc* mkcp0036(t_cc n) { return mkcp< cp_poly                 >(n); }
	cproc* mkcp0037(t_cc n) { return mkcp< cp_prompt               >(n); }
	cproc* mkcp0038(t_cc n) { return mkcp< cp_quick_language       >(n); }
	cproc* mkcp0039(t_cc n) { return mkcp< cp_quick_layout         >(n); }
	cproc* mkcp0040(t_cc n) { return mkcp< cp_quotes               >(n); }
	cproc* mkcp0041(t_cc n) { return mkcp< cp_rtv                  >(n); }
	cproc* mkcp0042(t_cc n) { return mkcp< cp_rtv1                 >(n); }
	cproc* mkcp0043(t_cc n) { return mkcp< cp_rtv_ico              >(n); }
	cproc* mkcp0044(t_cc n) { return mkcp< cp_rtv_info             >(n); }
	cproc* mkcp0045(t_cc n) { return mkcp< cp_sample_format        >(n); }
	cproc* mkcp0046(t_cc n) { return mkcp< cp_sysreset             >(n); }
	cproc* mkcp0047(t_cc n) { return mkcp< cp_tables               >(n); }
	cproc* mkcp0048(t_cc n) { return mkcp< cp_time_server          >(n); }
	cproc* mkcp0049(t_cc n) { return mkcp< cp_time_servers         >(n); }
	cproc* mkcp0050(t_cc n) { return mkcp< cp_true                 >(n); }
	cproc* mkcp0051(t_cc n) { return mkcp< cp_view_column          >(n); }
	cproc* mkcp0052(t_cc n) { return mkcp< cp_view_table           >(n); }
	cproc* mkcp0053(t_cc n) { return mkcp< cp_view_table_autoclass >(n); }
	cproc* mkcp0054(t_cc n) { return mkcp< cp_view_table_columns   >(n); }
	cproc* mkcp0055(t_cc n) { return mkcp< cp_view_table_trans     >(n); }
	cproc* mkcp0056(t_cc n) { return mkcp< cp_xml                  >(n); }

//!	cproc registry (generated).
	cprg cprgs[c_NR_CPROCS] = {
		{ "DEF"                  , 0, &mkcp0001 },
		{ "DEVEL"                , 0, &mkcp0002 },
		{ "HOST"                 , 0, &mkcp0003 },
		{ "JE"                   , 0, &mkcp0004 },
		{ "JS"                   , 0, &mkcp0005 },
		{ "JWE"                  , 0, &mkcp0006 },
		{ "JWS"                  , 0, &mkcp0007 },
		{ "LL"                   , 0, &mkcp0008 },
		{ "NULL"                 , 0, &mkcp0009 },
		{ "SRV"                  , 0, &mkcp0010 },
		{ "SVB"                  , 0, &mkcp0011 },
		{ "ajax_able"            , 0, &mkcp0012 },
		{ "ajax_updater"         , 0, &mkcp0013 },
		{ "autoclass"            , 0, &mkcp0014 },
		{ "begin_ajax"           , 0, &mkcp0015 },
		{ "clock"                , 0, &mkcp0016 },
		{ "current"              , 0, &mkcp0017 },
		{ "edit_table"           , 0, &mkcp0018 },
		{ "end_ajax"             , 0, &mkcp0019 },
		{ "errors"               , 0, &mkcp0020 },
		{ "fake_online_state"    , 0, &mkcp0021 },
		{ "fake_time"            , 0, &mkcp0022 },
		{ "false"                , 0, &mkcp0023 },
		{ "fdigits"              , 0, &mkcp0024 },
		{ "fpoint"               , 0, &mkcp0025 },
		{ "ilanguages"           , 0, &mkcp0026 },
		{ "infos"                , 0, &mkcp0027 },
		{ "is_ajax"              , 0, &mkcp0028 },
		{ "language"             , 0, &mkcp0029 },
		{ "layout"               , 0, &mkcp0030 },
		{ "login"                , 0, &mkcp0031 },
		{ "macfilter"            , 0, &mkcp0032 },
		{ "page_help"            , 0, &mkcp0033 },
		{ "page_help_default"    , 0, &mkcp0034 },
		{ "page_path"            , 0, &mkcp0035 },
		{ "poly"                 , 0, &mkcp0036 },
		{ "prompt"               , 0, &mkcp0037 },
		{ "quick_language"       , 0, &mkcp0038 },
		{ "quick_layout"         , 0, &mkcp0039 },
		{ "quotes"               , 0, &mkcp0040 },
		{ "rtv"                  , 0, &mkcp0041 },
		{ "rtv1"                 , 0, &mkcp0042 },
		{ "rtv_ico"              , 0, &mkcp0043 },
		{ "rtv_info"             , 0, &mkcp0044 },
		{ "sample_format"        , 0, &mkcp0045 },
		{ "sysreset"             , 0, &mkcp0046 },
		{ "tables"               , 0, &mkcp0047 },
		{ "time_server"          , 0, &mkcp0048 },
		{ "time_servers"         , 0, &mkcp0049 },
		{ "true"                 , 0, &mkcp0050 },
		{ "view_column"          , 0, &mkcp0051 },
		{ "view_table"           , 0, &mkcp0052 },
		{ "view_table_autoclass" , 0, &mkcp0053 },
		{ "view_table_columns"   , 0, &mkcp0054 },
		{ "view_table_trans"     , 0, &mkcp0055 },
		{ "xml"                  , 0, &mkcp0056 }
	};

//!	cproc find compare function
//!	p1, p2 are const pointers to cprg struct.
	typedef const cprg* pc_cprg;

	int cprcomp(const void* p1, const void* p2)
	{
		pc_cprg pc1 = reinterpret_cast<pc_cprg>(p1);
		pc_cprg pc2 = reinterpret_cast<pc_cprg>(p2);
		return strcmp(pc1->k, pc2->k);
	}

//!	find function.
	cproc* findcp(t_cc k)
	{
		TRACE_FLOW
		TRACE_VARQ(k)
		if (!k) return 0;
		cprg rf = { k, 0 };
		void* pv = bsearch(&rf, cprgs, c_NR_CPROCS, sizeof(cprg), &cprcomp);
		if (!pv) return 0;
		cprg* pr = reinterpret_cast<cprg*>(pv);
	//	return existing cproc or cproc creation
		return pr->p ? pr->p : (pr->p = (*pr->cc)(pr->k));
	}

//!	reset function.
	void resetcps()
	{
		cprg* pcp = cprgs;
		for (unsigned i = c_NR_CPROCS; i--; ++pcp) if (pcp->p) pcp->p->reset();
	}
//!	remove all cproc instances from memory
	void removecps()
	{
		cprg* pcp = cprgs;
		for (unsigned i = c_NR_CPROCS; i--; ++pcp) if (pcp->p) {
			delete pcp->p;
			pcp->p = 0;
		}
	}

//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include "cproc_register.hpp"

#include "trace_macros.hpp"

#include "cp_sample_ilanguage.hpp"
#include "cp_sample_macfilter.hpp"
#include "cp_sample_online.hpp"
#include "cp_samples_ajax.hpp"
#include "cp_samples_db.hpp"
#include "cp_samples_dbx.hpp"
#include "cp_samples_format.hpp"
#include "cp_samples_htm.hpp"
#include "cp_samples_time.hpp"
#include "cp_vars.hpp"
#include "err_inf.hpp"
#include "system_cprocs.hpp"

#include <cstdlib>

#define c_NR_CPROCS 56

//!	a cprocs regististry entry.
	struct cprg
	{
		t_cc k;
		cproc* p;
	};

//!	instances of all cprocs.
	cp_DEF                  cp0001;
	cp_DEVEL                cp0002;
	cp_HOST                 cp0003;
	cp_JE                   cp0004;
	cp_JS                   cp0005;
	cp_JWE                  cp0006;
	cp_JWS                  cp0007;
	cp_LL                   cp0008;
	cp_NULL                 cp0009;
	cp_SRV                  cp0010;
	cp_SVB                  cp0011;
	cp_ajax_able            cp0012;
	cp_ajax_updater         cp0013;
	cp_autoclass            cp0014;
	cp_begin_ajax           cp0015;
	cp_clock                cp0016;
	cp_current              cp0017;
	cp_edit_table           cp0018;
	cp_end_ajax             cp0019;
	cp_errors               cp0020;
	cp_fake_online_state    cp0021;
	cp_fake_time            cp0022;
	cp_false                cp0023;
	cp_fdigits              cp0024;
	cp_fpoint               cp0025;
	cp_ilanguages           cp0026;
	cp_infos                cp0027;
	cp_is_ajax              cp0028;
	cp_language             cp0029;
	cp_layout               cp0030;
	cp_login                cp0031;
	cp_macfilter            cp0032;
	cp_page_help            cp0033;
	cp_page_help_default    cp0034;
	cp_page_path            cp0035;
	cp_poly                 cp0036;
	cp_prompt               cp0037;
	cp_quick_language       cp0038;
	cp_quick_layout         cp0039;
	cp_quotes               cp0040;
	cp_rtv                  cp0041;
	cp_rtv1                 cp0042;
	cp_rtv_ico              cp0043;
	cp_rtv_info             cp0044;
	cp_sample_format        cp0045;
	cp_sysreset             cp0046;
	cp_tables               cp0047;
	cp_time_server          cp0048;
	cp_time_servers         cp0049;
	cp_true                 cp0050;
	cp_view_column          cp0051;
	cp_view_table           cp0052;
	cp_view_table_autoclass cp0053;
	cp_view_table_columns   cp0054;
	cp_view_table_trans     cp0055;
	cp_xml                  cp0056;


//!	the cprocs regististry.
	cprg cprgs[c_NR_CPROCS] = {
		{ "DEF"                  , &cp0001 },
		{ "DEVEL"                , &cp0002 },
		{ "HOST"                 , &cp0003 },
		{ "JE"                   , &cp0004 },
		{ "JS"                   , &cp0005 },
		{ "JWE"                  , &cp0006 },
		{ "JWS"                  , &cp0007 },
		{ "LL"                   , &cp0008 },
		{ "NULL"                 , &cp0009 },
		{ "SRV"                  , &cp0010 },
		{ "SVB"                  , &cp0011 },
		{ "ajax_able"            , &cp0012 },
		{ "ajax_updater"         , &cp0013 },
		{ "autoclass"            , &cp0014 },
		{ "begin_ajax"           , &cp0015 },
		{ "clock"                , &cp0016 },
		{ "current"              , &cp0017 },
		{ "edit_table"           , &cp0018 },
		{ "end_ajax"             , &cp0019 },
		{ "errors"               , &cp0020 },
		{ "fake_online_state"    , &cp0021 },
		{ "fake_time"            , &cp0022 },
		{ "false"                , &cp0023 },
		{ "fdigits"              , &cp0024 },
		{ "fpoint"               , &cp0025 },
		{ "ilanguages"           , &cp0026 },
		{ "infos"                , &cp0027 },
		{ "is_ajax"              , &cp0028 },
		{ "language"             , &cp0029 },
		{ "layout"               , &cp0030 },
		{ "login"                , &cp0031 },
		{ "macfilter"            , &cp0032 },
		{ "page_help"            , &cp0033 },
		{ "page_help_default"    , &cp0034 },
		{ "page_path"            , &cp0035 },
		{ "poly"                 , &cp0036 },
		{ "prompt"               , &cp0037 },
		{ "quick_language"       , &cp0038 },
		{ "quick_layout"         , &cp0039 },
		{ "quotes"               , &cp0040 },
		{ "rtv"                  , &cp0041 },
		{ "rtv1"                 , &cp0042 },
		{ "rtv_ico"              , &cp0043 },
		{ "rtv_info"             , &cp0044 },
		{ "sample_format"        , &cp0045 },
		{ "sysreset"             , &cp0046 },
		{ "tables"               , &cp0047 },
		{ "time_server"          , &cp0048 },
		{ "time_servers"         , &cp0049 },
		{ "true"                 , &cp0050 },
		{ "view_column"          , &cp0051 },
		{ "view_table"           , &cp0052 },
		{ "view_table_autoclass" , &cp0053 },
		{ "view_table_columns"   , &cp0054 },
		{ "view_table_trans"     , &cp0055 },
		{ "xml"                  , &cp0056 }
	};

//!	cproc names setter.
	class cpsnm
	{
	public:
		cpsnm()
		{
			TRACE_FLOW
			cprg* pcp = cprgs;
			for (unsigned i = c_NR_CPROCS; i--; ++pcp) {
				t_ch& ch = const_cast<t_ch&>(pcp->p->cpnm);
				ch = pcp->k;
			}
			delete this;
		}
		~cpsnm()
		{
			TRACE_FLOW
		}
	};

	cpsnm* snm = new cpsnm;

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
		return pv ? (reinterpret_cast<cprg*>(pv))->p : 0;
	}

//!	reset function.
	void resetcps(void)
	{
		cprg* pcp = cprgs;
		for (unsigned i = c_NR_CPROCS; i--; ++pcp) pcp->p->reset();
	}

//!	cproc count info.
	unsigned cpcount(void) { return c_NR_CPROCS; }

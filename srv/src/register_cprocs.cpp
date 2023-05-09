//| ============================================================
//! @file
//!
//! Registration of all available cproc units.
//!
//!	Content of definiton is script generated.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include "register_cprocs.hpp"
#include "fcpcgi.hpp"

#undef DEVEL
#include "trace_macros.hpp"


//	header files containing cproc declarations
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

//	generated keys from cproc derivates' names
	t_cc cpk_DEF                  = "DEF";
	t_cc cpk_DEVEL                = "DEVEL";
	t_cc cpk_HOST                 = "HOST";
	t_cc cpk_JE                   = "JE";
	t_cc cpk_JS                   = "JS";
	t_cc cpk_JWE                  = "JWE";
	t_cc cpk_JWS                  = "JWS";
	t_cc cpk_LL                   = "LL";
	t_cc cpk_NULL                 = "NULL";
	t_cc cpk_SRV                  = "SRV";
	t_cc cpk_SVB                  = "SVB";
	t_cc cpk_ajax_able            = "ajax_able";
	t_cc cpk_ajax_updater         = "ajax_updater";
	t_cc cpk_autoclass            = "autoclass";
	t_cc cpk_begin_ajax           = "begin_ajax";
	t_cc cpk_clock                = "clock";
	t_cc cpk_current              = "current";
	t_cc cpk_edit_table           = "edit_table";
	t_cc cpk_end_ajax             = "end_ajax";
	t_cc cpk_errors               = "errors";
	t_cc cpk_fake_online_state    = "fake_online_state";
	t_cc cpk_fake_time            = "fake_time";
	t_cc cpk_false                = "false";
	t_cc cpk_fdigits              = "fdigits";
	t_cc cpk_fpoint               = "fpoint";
	t_cc cpk_ilanguages           = "ilanguages";
	t_cc cpk_infos                = "infos";
	t_cc cpk_is_ajax              = "is_ajax";
	t_cc cpk_language             = "language";
	t_cc cpk_layout               = "layout";
	t_cc cpk_login                = "login";
	t_cc cpk_macfilter            = "macfilter";
	t_cc cpk_page_help            = "page_help";
	t_cc cpk_page_help_default    = "page_help_default";
	t_cc cpk_page_path            = "page_path";
	t_cc cpk_poly                 = "poly";
	t_cc cpk_prompt               = "prompt";
	t_cc cpk_quick_language       = "quick_language";
	t_cc cpk_quick_layout         = "quick_layout";
	t_cc cpk_quotes               = "quotes";
	t_cc cpk_sample_format        = "sample_format";
	t_cc cpk_sysreset             = "sysreset";
	t_cc cpk_tables               = "tables";
	t_cc cpk_time_server          = "time_server";
	t_cc cpk_time_servers         = "time_servers";
	t_cc cpk_true                 = "true";
	t_cc cpk_view_column          = "view_column";
	t_cc cpk_view_table           = "view_table";
	t_cc cpk_view_table_autoclass = "view_table_autoclass";
	t_cc cpk_view_table_columns   = "view_table_columns";
	t_cc cpk_view_table_trans     = "view_table_trans";
	t_cc cpk_xml                  = "xml";


	template <class T>
	T* tpl_mkcp(t_cc n)
	{
		TRACE_FLOW
		static T* cp = new T;
		t_ch& ch = const_cast<t_ch&>(cp->cpnm);
		ch = n;
		TRACE_ADDR(cp)
		TRACE_VARQ(cp->cpnm)
		return cp;
	}

//	cproc creator functions
	cproc* mkcp_DEF                  (void) { return tpl_mkcp< cp_DEF                  >(cpk_DEF                 ); }
	cproc* mkcp_DEVEL                (void) { return tpl_mkcp< cp_DEVEL                >(cpk_DEVEL               ); }
	cproc* mkcp_HOST                 (void) { return tpl_mkcp< cp_HOST                 >(cpk_HOST                ); }
	cproc* mkcp_JE                   (void) { return tpl_mkcp< cp_JE                   >(cpk_JE                  ); }
	cproc* mkcp_JS                   (void) { return tpl_mkcp< cp_JS                   >(cpk_JS                  ); }
	cproc* mkcp_JWE                  (void) { return tpl_mkcp< cp_JWE                  >(cpk_JWE                 ); }
	cproc* mkcp_JWS                  (void) { return tpl_mkcp< cp_JWS                  >(cpk_JWS                 ); }
	cproc* mkcp_LL                   (void) { return tpl_mkcp< cp_LL                   >(cpk_LL                  ); }
	cproc* mkcp_NULL                 (void) { return tpl_mkcp< cp_NULL                 >(cpk_NULL                ); }
	cproc* mkcp_SRV                  (void) { return tpl_mkcp< cp_SRV                  >(cpk_SRV                 ); }
	cproc* mkcp_SVB                  (void) { return tpl_mkcp< cp_SVB                  >(cpk_SVB                 ); }
	cproc* mkcp_ajax_able            (void) { return tpl_mkcp< cp_ajax_able            >(cpk_ajax_able           ); }
	cproc* mkcp_ajax_updater         (void) { return tpl_mkcp< cp_ajax_updater         >(cpk_ajax_updater        ); }
	cproc* mkcp_autoclass            (void) { return tpl_mkcp< cp_autoclass            >(cpk_autoclass           ); }
	cproc* mkcp_begin_ajax           (void) { return tpl_mkcp< cp_begin_ajax           >(cpk_begin_ajax          ); }
	cproc* mkcp_clock                (void) { return tpl_mkcp< cp_clock                >(cpk_clock               ); }
	cproc* mkcp_current              (void) { return tpl_mkcp< cp_current              >(cpk_current             ); }
	cproc* mkcp_edit_table           (void) { return tpl_mkcp< cp_edit_table           >(cpk_edit_table          ); }
	cproc* mkcp_end_ajax             (void) { return tpl_mkcp< cp_end_ajax             >(cpk_end_ajax            ); }
	cproc* mkcp_errors               (void) { return tpl_mkcp< cp_errors               >(cpk_errors              ); }
	cproc* mkcp_fake_online_state    (void) { return tpl_mkcp< cp_fake_online_state    >(cpk_fake_online_state   ); }
	cproc* mkcp_fake_time            (void) { return tpl_mkcp< cp_fake_time            >(cpk_fake_time           ); }
	cproc* mkcp_false                (void) { return tpl_mkcp< cp_false                >(cpk_false               ); }
	cproc* mkcp_fdigits              (void) { return tpl_mkcp< cp_fdigits              >(cpk_fdigits             ); }
	cproc* mkcp_fpoint               (void) { return tpl_mkcp< cp_fpoint               >(cpk_fpoint              ); }
	cproc* mkcp_ilanguages           (void) { return tpl_mkcp< cp_ilanguages           >(cpk_ilanguages          ); }
	cproc* mkcp_infos                (void) { return tpl_mkcp< cp_infos                >(cpk_infos               ); }
	cproc* mkcp_is_ajax              (void) { return tpl_mkcp< cp_is_ajax              >(cpk_is_ajax             ); }
	cproc* mkcp_language             (void) { return tpl_mkcp< cp_language             >(cpk_language            ); }
	cproc* mkcp_layout               (void) { return tpl_mkcp< cp_layout               >(cpk_layout              ); }
	cproc* mkcp_login                (void) { return tpl_mkcp< cp_login                >(cpk_login               ); }
	cproc* mkcp_macfilter            (void) { return tpl_mkcp< cp_macfilter            >(cpk_macfilter           ); }
	cproc* mkcp_page_help            (void) { return tpl_mkcp< cp_page_help            >(cpk_page_help           ); }
	cproc* mkcp_page_help_default    (void) { return tpl_mkcp< cp_page_help_default    >(cpk_page_help_default   ); }
	cproc* mkcp_page_path            (void) { return tpl_mkcp< cp_page_path            >(cpk_page_path           ); }
	cproc* mkcp_poly                 (void) { return tpl_mkcp< cp_poly                 >(cpk_poly                ); }
	cproc* mkcp_prompt               (void) { return tpl_mkcp< cp_prompt               >(cpk_prompt              ); }
	cproc* mkcp_quick_language       (void) { return tpl_mkcp< cp_quick_language       >(cpk_quick_language      ); }
	cproc* mkcp_quick_layout         (void) { return tpl_mkcp< cp_quick_layout         >(cpk_quick_layout        ); }
	cproc* mkcp_quotes               (void) { return tpl_mkcp< cp_quotes               >(cpk_quotes              ); }
	cproc* mkcp_sample_format        (void) { return tpl_mkcp< cp_sample_format        >(cpk_sample_format       ); }
	cproc* mkcp_sysreset             (void) { return tpl_mkcp< cp_sysreset             >(cpk_sysreset            ); }
	cproc* mkcp_tables               (void) { return tpl_mkcp< cp_tables               >(cpk_tables              ); }
	cproc* mkcp_time_server          (void) { return tpl_mkcp< cp_time_server          >(cpk_time_server         ); }
	cproc* mkcp_time_servers         (void) { return tpl_mkcp< cp_time_servers         >(cpk_time_servers        ); }
	cproc* mkcp_true                 (void) { return tpl_mkcp< cp_true                 >(cpk_true                ); }
	cproc* mkcp_view_column          (void) { return tpl_mkcp< cp_view_column          >(cpk_view_column         ); }
	cproc* mkcp_view_table           (void) { return tpl_mkcp< cp_view_table           >(cpk_view_table          ); }
	cproc* mkcp_view_table_autoclass (void) { return tpl_mkcp< cp_view_table_autoclass >(cpk_view_table_autoclass); }
	cproc* mkcp_view_table_columns   (void) { return tpl_mkcp< cp_view_table_columns   >(cpk_view_table_columns  ); }
	cproc* mkcp_view_table_trans     (void) { return tpl_mkcp< cp_view_table_trans     >(cpk_view_table_trans    ); }
	cproc* mkcp_xml                  (void) { return tpl_mkcp< cp_xml                  >(cpk_xml                 ); }

//	registration: assignment of creator functions to keys
	void register_cprocs(void)
	{
	//	unconst access
		fcpcgi& mfcp = const_cast<fcpcgi&>(fcp);

		mfcp.set( cpk_DEF                  , &mkcp_DEF                  );
		mfcp.set( cpk_DEVEL                , &mkcp_DEVEL                );
		mfcp.set( cpk_HOST                 , &mkcp_HOST                 );
		mfcp.set( cpk_JE                   , &mkcp_JE                   );
		mfcp.set( cpk_JS                   , &mkcp_JS                   );
		mfcp.set( cpk_JWE                  , &mkcp_JWE                  );
		mfcp.set( cpk_JWS                  , &mkcp_JWS                  );
		mfcp.set( cpk_LL                   , &mkcp_LL                   );
		mfcp.set( cpk_NULL                 , &mkcp_NULL                 );
		mfcp.set( cpk_SRV                  , &mkcp_SRV                  );
		mfcp.set( cpk_SVB                  , &mkcp_SVB                  );
		mfcp.set( cpk_ajax_able            , &mkcp_ajax_able            );
		mfcp.set( cpk_ajax_updater         , &mkcp_ajax_updater         );
		mfcp.set( cpk_autoclass            , &mkcp_autoclass            );
		mfcp.set( cpk_begin_ajax           , &mkcp_begin_ajax           );
		mfcp.set( cpk_clock                , &mkcp_clock                );
		mfcp.set( cpk_current              , &mkcp_current              );
		mfcp.set( cpk_edit_table           , &mkcp_edit_table           );
		mfcp.set( cpk_end_ajax             , &mkcp_end_ajax             );
		mfcp.set( cpk_errors               , &mkcp_errors               );
		mfcp.set( cpk_fake_online_state    , &mkcp_fake_online_state    );
		mfcp.set( cpk_fake_time            , &mkcp_fake_time            );
		mfcp.set( cpk_false                , &mkcp_false                );
		mfcp.set( cpk_fdigits              , &mkcp_fdigits              );
		mfcp.set( cpk_fpoint               , &mkcp_fpoint               );
		mfcp.set( cpk_ilanguages           , &mkcp_ilanguages           );
		mfcp.set( cpk_infos                , &mkcp_infos                );
		mfcp.set( cpk_is_ajax              , &mkcp_is_ajax              );
		mfcp.set( cpk_language             , &mkcp_language             );
		mfcp.set( cpk_layout               , &mkcp_layout               );
		mfcp.set( cpk_login                , &mkcp_login                );
		mfcp.set( cpk_macfilter            , &mkcp_macfilter            );
		mfcp.set( cpk_page_help            , &mkcp_page_help            );
		mfcp.set( cpk_page_help_default    , &mkcp_page_help_default    );
		mfcp.set( cpk_page_path            , &mkcp_page_path            );
		mfcp.set( cpk_poly                 , &mkcp_poly                 );
		mfcp.set( cpk_prompt               , &mkcp_prompt               );
		mfcp.set( cpk_quick_language       , &mkcp_quick_language       );
		mfcp.set( cpk_quick_layout         , &mkcp_quick_layout         );
		mfcp.set( cpk_quotes               , &mkcp_quotes               );
		mfcp.set( cpk_sample_format        , &mkcp_sample_format        );
		mfcp.set( cpk_sysreset             , &mkcp_sysreset             );
		mfcp.set( cpk_tables               , &mkcp_tables               );
		mfcp.set( cpk_time_server          , &mkcp_time_server          );
		mfcp.set( cpk_time_servers         , &mkcp_time_servers         );
		mfcp.set( cpk_true                 , &mkcp_true                 );
		mfcp.set( cpk_view_column          , &mkcp_view_column          );
		mfcp.set( cpk_view_table           , &mkcp_view_table           );
		mfcp.set( cpk_view_table_autoclass , &mkcp_view_table_autoclass );
		mfcp.set( cpk_view_table_columns   , &mkcp_view_table_columns   );
		mfcp.set( cpk_view_table_trans     , &mkcp_view_table_trans     );
		mfcp.set( cpk_xml                  , &mkcp_xml                  );

	//	finalize
		mfcp.sort();
	}

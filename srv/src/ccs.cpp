//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include <ccs.h>

//	static setup part
	t_cc c_iadcgi_ini		= "iadcgi.ini";
	t_cc c_layout_default	= "std";
	t_cc c_common			= "common";
	t_cc c_lang_default		= "en";
	t_cc c_brand			= "brand";
	t_cc c_htmdir			= "htmdir";
	t_cc c_htmdir_default	= "../htm";
	t_cc c_langdir			= "langdir";
	t_cc c_langdir_default	= "../bbl";
	t_cc c_dbdir			= "dbdir";
	t_cc c_dbdir_default 	= "../db";
	t_cc c_tmpdir			= "tmpdir";
	t_cc c_tmpdir_default  = "../tmp";
	t_cc c_dbnames			= "dbnames";
	t_cc c_dbnames_default	= "user runtime";
	t_cc c_poly				= "poly";
	t_cc c_droot			= "droot";
	t_cc c_fpt				= "fpt.";
	t_cc c_digits			= "digits";
	t_cc c_sysreset			= "sysreset";

//	dynamic setup part
	t_cc c_call			= "call";
	t_cc c_layout		= "layout";
	t_cc c_language		= "language";
	t_cc c_lang			= "lang";
	t_cc c_sid			= "sid";

	t_cc c_ext_htm 		= ".htm";
	t_cc c_ext_dat 		= ".dat";
	t_cc c_ext_ini		= ".ini";
	t_cc c_ext_js		= ".js";
	t_cc c_ext_db		= ".db";

	const char c_sep	= '-';

//	login
	t_cc c_uid = "uid";
	t_cc c_qed = "qed";

//	special keys
	t_cc c_ajax			= "ajax";
	t_cc uc_begin		= "BEGIN";
	t_cc c_complete		= "complete";
	t_cc c_default 		= "default";
	t_cc c_end			= "end";
	t_cc uc_end			= "END";
	t_cc c_help			= "help";
	t_cc c_hpg			= "hpg";
	t_cc c_link_atts	= "link_atts";
	t_cc c_no_help		= "no_help";
	t_cc c_none 		= "none";
	t_cc uc_not			= "NOT";
	t_cc c_onerror		= "onerror";
	t_cc c_plg			= "plg";
	t_cc c_ply			= "ply";
	t_cc c_reversed		= "reversed";
	t_cc c_rtvs			= "rtvs";
	t_cc c_rtv_			= "rtv_";
	t_cc uc_rtv_n		= "RTV_N_";
	t_cc uc_rtv_s		= "RTV_S_";
	t_cc uc_rtv_y		= "RTV_Y_";
	t_cc c_subject		= "subject";
	t_cc c_this			= "this";
	t_cc c_unique		= "unique";
	t_cc c_xml			= "xml";
	t_cc c_pr_prompted	= "_PROMPTED_";
	t_cc c_pr_yes		= "_YES_";
	t_cc bswitch_vals[6] = { "1", "on", "ON", "0", "off", "OFF" };

//	SQL
	t_cc c_db_and 			= " and ";
	t_cc c_db_as			= " as ";
	t_cc c_db_attach		= " attach ";
	t_cc c_db_begin			= " begin ";
	t_cc c_db_commit		= " commit ";
	t_cc c_db_default		= " default ";
	t_cc c_db_delete		= " delete ";
	t_cc c_db_desc			= " desc ";
	t_cc c_db_exclusive		= " exclusive ";
	t_cc c_db_from			= " from ";
	t_cc c_db_insert		= " insert ";
	t_cc c_db_into			= " into ";
	t_cc c_db_not			= " not ";
	t_cc c_db_null			= " null ";
	t_cc c_db_or			= " or ";
	t_cc c_db_order_by		= " order by ";
	t_cc c_db_select		= " select ";
	t_cc c_db_set			= " set ";
	t_cc c_db_transaction	= " transaction ";
	t_cc c_db_update		= " update ";
	t_cc c_db_values		= " values ";
	t_cc c_db_where 		= " where ";
	t_cc c_db_wildcard		= "*";

//	tag types
	t_cc c_div		= "div";
	t_cc c_form 	= "form";
	t_cc c_img		= "img";
	t_cc c_input 	= "input";
	t_cc c_label 	= "label";
	t_cc c_option 	= "option";
	t_cc c_script	= "script";
	t_cc c_select 	= "select";
	t_cc c_span		= "span";

//	attribute specifiers
	t_cc c_action 		= "action";
	t_cc c_align		= "align";
	t_cc c_alt			= "alt";
	t_cc c_cellpadding 	= "cellpadding";
	t_cc c_cellspacing 	= "cellspacing";
	t_cc c_checked 		= "checked";
	t_cc c_class 		= "class";
	t_cc c_colspan		= "colspan";
	t_cc c_for   		= "for";
	t_cc c_height		= "height";
	t_cc c_href 		= "href";
	t_cc c_id    		= "id";
	t_cc c_maxlength	= "maxlength";
	t_cc c_multiple		= "multiple";
	t_cc c_name  		= "name";
	t_cc c_readonly		= "readonly";
	t_cc c_selected 	= "selected";
	t_cc c_size 		= "size";
	t_cc c_src			= "src";
	t_cc c_title 		= "title";
	t_cc c_type  		= "type";
	t_cc c_value  		= "value";
	t_cc c_width		= "width";

//	attribute values
//	t_cc c_center		= "center";
	t_cc c_right		= "right";

//	type values
	t_cc c_checkbox 	= "checkbox";
	t_cc c_hidden 		= "hidden";
	t_cc c_javascript 	= "javascript";
	t_cc c_password 	= "password";
	t_cc c_radio		= "radio";
	t_cc c_submit 		= "submit";
	t_cc c_text 		= "text";

//	predefined class values
	t_cc c_current 	= "current";
	t_cc c_host		= "host";
	t_cc c_ip4 		= "ip4";
	t_cc c_mac		= "mac";
	t_cc c_nok 		= "nok";
	t_cc c_num 		= "num";
	t_cc c_ok 		= "ok";
	t_cc c_real		= "real";
	t_cc c_remove	= "remove";
//	class prefixes
	t_cc c_btn		= "btn";
	t_cc c_dbc		= "dbc";

//	predefined name values
	t_cc c_form_issue = "form_issue";
//	date and time names
	t_cc c_hour 	= "hour";
	t_cc c_minute 	= "minute";
	t_cc c_second 	= "second";
	t_cc c_day 		= "day";
	t_cc c_month 	= "month";
	t_cc c_year 	= "year";

//! table specials
	t_cc c_table 	= "table";
	t_cc c_tr 		= "tr";
	t_cc c_th 		= "th";
	t_cc c_td 		= "td";

//	misc
	t_cc c_com_open		= "<!--";
	t_cc c_com_close	= "-->";
	t_cc c_tcl			= "</";
	t_cc c_tclm			= "<\\/";
	t_cc c_nbsp			= "&nbsp;";

//	request environment settings
	t_cc uc_query_string   	= "QUERY_STRING";
//	cgi only
#ifndef ISSRV
	t_cc uc_request_method 	= "REQUEST_METHOD";
	t_cc uc_content_length	= "CONTENT_LENGTH";
	t_cc uc_script_name 	= "SCRIPT_NAME";
	t_cc uc_remote_addr 	= "REMOTE_ADDR";
	t_cc uc_remote_port 	= "REMOTE_PORT";
#endif

//	response constants
	t_cc c_method 			= "method";
	t_cc uc_get 		 	= "GET";
	t_cc uc_post 			= "POST";
	t_cc c_content_type		= "Content-Type";
	t_cc c_text_html		= "text/html";
	t_cc c_text_plain		= "text/plain";
	t_cc c_page 			= "page";
	t_cc c_next 			= "next";
	t_cc c_params			= "params";
	t_cc c_start 			= "start";
	t_cc c_login 			= "login";
	t_cc c_logout 			= "logout";

//	xtags
	t_cc c_amp	= "&amp;";
	t_cc c_quot	= "&quot;";
	t_cc c_gt	= "&gt;";
	t_cc c_lt	= "&lt;";

//	============================================================
//	specials
	t_cc c_hms			= "%H:%M:%S";
	t_cc c_java_write	= "window.document.write('";
	t_cc uc_null		= "NULL";

//	============================================================
//	log and error messages

	t_cc msg_error			= "error: ";
	t_cc msg_info			= ": ";
	t_cc msg_trc_enter      = "ENTER ";
	t_cc msg_trc_exit       = "EXIT  ";
	t_cc msg_trc_address    = " address: ";
	t_cc msg_exceeded		= " exceeded.";
	t_cc msg_negative		= " negative.";
	t_cc msg_loading		= "loading ";
	t_cc msg_file_not_found = "file not found: \"";
	t_cc msg_case_level 	= "CASE level";
	t_cc msg_no_page		= "no page specified";
	t_cc msg_no_directory	= "not a directory: ";

//	============================================================
//	some server specials
#ifdef ISSRV

	t_cc c_iadsrv_ini		= "iadsrv.ini";
	t_cc c_port				= "port";
	t_cc c_ip				= "ip";
	t_cc c_deflate			= "deflate";
	t_cc c_dfldir			= "dfldir";
	t_cc c_dflkeep			= "dflkeep";

	t_cc uc_ctrl			= "CTRL";
	t_cc uc_file			= "FILE";

//	HTTP header
	t_cc c_accept_encoding	= "Accept-Encoding";
	t_cc c_cache_control	= "Cache-Control";
	t_cc c_connection		= "Connection";
	t_cc c_content_encoding	= "Content-Encoding";
	t_cc c_content_length	= "Content-Length";
	t_cc c_date				= "Date";
	t_cc c_etag				= "ETag";
	t_cc c_http1x			= "HTTP/1.x";
	t_cc c_iadsrv			= "IADSRV/1.0";
	t_cc c_if_none_match	= "If-None-Match";
	t_cc c_internal_error	= "Internal Server Error";
	t_cc c_keep_alive		= "Keep-Alive";
	t_cc c_last_modified	= "Last-Modified";
	t_cc c_max				= "max";
	t_cc uc_msie			= "MSIE";
	t_cc c_no_store			= "no-store";
	t_cc c_not_found		= "Not Found";
	t_cc c_not_modified		= "Not Modified";
	t_cc uc_ok				= "OK";
	t_cc c_server			= "Server";
	t_cc c_timeout			= "timeout";
	t_cc c_unsupported_type	= "Unsupported Media Type";
	t_cc c_user_agent		= "User-Agent";
	t_cc c_vary				= "Vary";

#endif

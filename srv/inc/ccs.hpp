//| ============================================================
//! @file
//!
//!	Central storage of common constant character strings.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef CCS_HPP
#define CCS_HPP

#include "types.hpp"

//	============================================================
//	setup

//	static setup part
//!	name of ini file: "iad.ini".
	extern t_cc c_iadcgi_ini;
//!	ini key for default layout: "layout".
	extern t_cc c_layout;
//!	default layout if not set in ini: "std".
	extern t_cc c_layout_default;
//!	ini key for default language short : "lang".
	extern t_cc c_lang;
//!	default lang if not set in ini: "en".
	extern t_cc c_lang_default;
//!	ini key for htm top directory : "htmdir".
	extern t_cc c_htmdir;
//!	default htmdir if not set in ini: "../htm".
	extern t_cc c_htmdir_default;
//!	ini key for language directory : "langdir".
	extern t_cc c_langdir;
//!	default langdir if not set in ini: "../bbl".
	extern t_cc c_langdir_default;
//!	ini key for (temp) data directory : "tmpdir".
	extern t_cc c_tmpdir;
//!	default tmpdir if not set in ini: "../tmp".
	extern t_cc c_tmpdir_default;
//!	ini key for database directory : "dbdir".
	extern t_cc c_dbdir;
//!	default dbdir if not set in ini: "../db".
	extern t_cc c_dbdir_default;
//!	ini key for databases to connect / attach : "dbnames".
	extern t_cc c_dbnames;
//!	default dbnames if not set in ini: "user runtime".
	extern t_cc c_dbnames_default;
//!	ini key (and default) for common: "common".
//!	defines common subdirectory name for htm documents
//!	and common part of language literals files
//!	(e.g. common-de.dat).
	extern t_cc c_common;
//!	multi layout allowed key "poly".
	extern t_cc c_poly;

//!	document root for css / javascript calls: "droot".
//!	only valid for CGI version.
	extern t_cc c_droot;

//!	floating point representation: "fpt.".
//!	sample "fpt. = ," "fpt.en = ."
	extern t_cc c_fpt;
//!	number of floating point "digits".
	extern t_cc c_digits;

//!	reset system call key "sysreset".
	extern t_cc c_sysreset;

//!	no ini key: stores long description
//!	assigned to a layout.
//!	ini sample: "arcor = Vodkafone Pro Mille AG"
	extern t_cc c_brand;
//!	no ini key: stores long description
//!	assigned to a language short.
//!	ini sample:	"de = Deutsch"
	extern t_cc c_language;

//	dynamic setup part

//!	"call" internal usage.
	extern t_cc c_call;
//!	"sid" session id parameter key, internal usage.
	extern t_cc c_sid;

//	standard extensions

//!	htm file extension.
	extern t_cc c_ext_htm;
//!	data file extension.
	extern t_cc c_ext_dat;
//!	ini file extension.
	extern t_cc c_ext_ini;
//!	javascript file extension.
	extern t_cc c_ext_js;
//!	database file extension.
	extern t_cc c_ext_db;

//!	layout language separator: '-'.
	extern const char c_sep;

//	============================================================
//	login
//!	"uid" (user name).
	extern t_cc c_uid;
//!	"qed" (password).
	extern t_cc c_qed;

//	============================================================
//	special keys
//!	ajax request indication "ajax".
	extern t_cc c_ajax;
//!	ini reader section "BEGIN".
	extern t_cc uc_begin;
//!	"complete" key of language dat files.
//!	inicates that anonymous common file does not have to be loaded.
	extern t_cc c_complete;
//!	default indicator: "default".
	extern t_cc c_default;
//!	"end".
	extern t_cc c_end;
//!	ini reader section "END".
	extern t_cc uc_end;
//!	page help first phrase "help".
	extern t_cc c_help;
//!	page help redirection KEY "hpg".
	extern t_cc c_hpg;
//!	standard link attributes "link_atts".
	extern t_cc c_link_atts;
//!	standard phrase KEY for no help available "no_help".
	extern t_cc c_no_help;
//!	variable undefiner: "none".
	extern t_cc c_none;
//!	dcp negator "NOT".
	extern t_cc uc_not;
//!	form on error specifier "onerror".
	extern t_cc c_onerror;
//!	poly mode lang "plg".
	extern t_cc c_plg;
//!	poly mode layout "ply".
	extern t_cc c_ply;
//!	reversed output indicator for some cprocs: "reversed".
	extern t_cc c_reversed;
//!	run time vars "rtvs".
	extern t_cc c_rtvs;
//!	run time vars "rtv_" prefix.
	extern t_cc c_rtv_;
//!	run time vars no / off "RTV_N_".
	extern t_cc uc_rtv_n;
//!	run time vars setter "RTV_S_".
	extern t_cc uc_rtv_s;
//!	run time vars yes / on "RTV_Y_".
	extern t_cc uc_rtv_y;
//!	"subject" key word.
	extern t_cc c_subject;
//!	this page specifier "this".
	extern t_cc c_this;
//!	special var "unique" indicates that page cannot be switched to other layouts.
	extern t_cc c_unique;
//!	"xml" ini key to switch xml behaviour generally on.
	extern t_cc c_xml;
//!	the "_PROMPTED_" indicator.
	extern t_cc c_pr_prompted;
//!	the "_YES_" submit button name for "are you sure?" prompts.
	extern t_cc c_pr_yes;
//!	switch keywords:
//!	0 .. 2 on : "1", "on",  "ON"
//! 3 .. 5 off: "0", "off", "OFF"
	extern t_cc bswitch_vals[6];

//	============================================================
//	SQL
//!	"and".
	extern t_cc c_db_and;
//!	" as ".
	extern t_cc c_db_as;
//!	" attach ".
	extern t_cc c_db_attach;
//!	" begin ".
	extern t_cc c_db_begin;
//!	" commit ".
	extern t_cc c_db_commit;
//!	" default ".
	extern t_cc c_db_default;
//!	" delete ".
	extern t_cc c_db_delete;
//!	" desc ".
	extern t_cc c_db_desc;
//!	" exclusive ".
	extern t_cc c_db_exclusive;
//!	" from ".
	extern t_cc c_db_from;
//!	" insert ".
	extern t_cc c_db_insert;
//!	" into ".
	extern t_cc c_db_into;
//!	" max(".
//	extern t_cc c_db_max;
//!	" not ".
	extern t_cc c_db_not;
//!	" null ".
	extern t_cc c_db_null;
//!	" or ".
	extern t_cc c_db_or;
//!	" order by ".
	 extern t_cc c_db_order_by;
//!	" select ".
	extern t_cc c_db_select;
//!	" set ".
	extern t_cc c_db_set;
//!	" transaction ".
	extern t_cc c_db_transaction;
//!	" update ".
	extern t_cc c_db_update;
//!	" values ".
	extern t_cc c_db_values;
//!	" where ".
	extern t_cc c_db_where;
//! "*" update id wildcard
	extern t_cc c_db_wildcard;


//	============================================================
//	HTML

//	tag types

//!	"div".
	extern t_cc c_div;
//!	"form".
	extern t_cc c_form;
//!	"img".
	extern t_cc c_img;
//!	"input".
	extern t_cc c_input;
//!	"label".
	extern t_cc c_label;
//!	"option".
	extern t_cc c_option;
//!	"script".
	extern t_cc c_script;
//!	"select".
	extern t_cc c_select;
//!	"span".
	extern t_cc c_span;

//	table specials
//!	"table".
	extern t_cc c_table;
//!	"tr".
	extern t_cc c_tr;
//!	"th".
	extern t_cc c_th;
//!	"td".
	extern t_cc c_td;

//	attribute specifiers

//!	"action".
	extern t_cc c_action;
//!	"align".
	extern t_cc c_align;
//!	"alt".
	extern t_cc c_alt;
//!	"cellpadding".
	extern t_cc c_cellpadding;
//!	"cellspacing".
	extern t_cc c_cellspacing;
//!	"checked".
	extern t_cc c_checked;
//!	"class".
	extern t_cc c_class;
//!	"colspan".
	extern t_cc c_colspan;
//!	"for".
	extern t_cc c_for;
//!	"height".
	extern t_cc c_height;
//!	"href".
	extern t_cc c_href;
//!	"id".
	extern t_cc c_id;
//!	"maxlength".
	extern t_cc c_maxlength;
//!	"multiple".
	extern t_cc c_multiple;
//!	"name".
	extern t_cc c_name;
//!	"readonly".
	extern t_cc c_readonly;
//!	"selected".
	extern t_cc c_selected;
//!	"size".
	extern t_cc c_size;
//!	"src".
	extern t_cc c_src;
//!	"title".
	extern t_cc c_title;
//!	"type".
	extern t_cc c_type;
//!	"value".
	extern t_cc c_value;
//!	"width".
	extern t_cc c_width;

//	attribute values
//!	"center".
//	extern t_cc c_center;
//!	"right".
	extern t_cc c_right;

//	- type values
//!	"checkbox".
	extern t_cc c_checkbox;
//!	"hidden".
	extern t_cc c_hidden;
//!	"javascript".
	extern t_cc c_javascript;
//!	"password".
	extern t_cc c_password;
//!	"radio".
	extern t_cc c_radio;
//!	"submit".
	extern t_cc c_submit;
//!	"text".
	extern t_cc c_text;

//	predefined class values

//!	the class value for current links: "current".
	extern t_cc c_current;
//!	a server name "host".
	extern t_cc c_host;
//!	"ip4".
	extern t_cc c_ip4;
//!	a mac address "mac".
	extern t_cc c_mac;
//!	"nok".
	extern t_cc c_nok;
//!	"num".
	extern t_cc c_num;
//!	"ok".
	extern t_cc c_ok;
//!	"real".
	extern t_cc c_real;
//! "remove" checkboxes
	extern t_cc c_remove;

//	class prefixes
//!	button class prefix.
	extern t_cc c_btn;
//!	database column class prefix.
	extern t_cc c_dbc;


//	predefined name attribute values

//!	form issue recognizer
	extern t_cc c_form_issue;

//	date and time names
//!	"hour".
	extern t_cc c_hour;
//!	"minute".
	extern t_cc c_minute;
//!	"second".
	extern t_cc c_second;
//!	"day".
	extern t_cc c_day;
//!	"month".
	extern t_cc c_month;
//!	"year".
	extern t_cc c_year;

//	misc
//!	comment opener "<!--".
	extern t_cc c_com_open;
//!	comment closer "-->".
	extern t_cc c_com_close;
//!	tag closer "</".
	extern t_cc c_tcl;
//!	masked tag closer "<\/".
	extern t_cc c_tclm;
//!	non break space "&nbsp;".
	extern t_cc c_nbsp;

//	cgi only request environment settings

//!	"QUERY_STRING".
	extern t_cc uc_query_string;

//	cgi only
#ifndef ISSRV
//!	"REQUEST_METHOD".
	extern t_cc uc_request_method;
//!	"CONTENT_LENGTH".
	extern t_cc uc_content_length;
//!	"SCRIPT_NAME".
	extern t_cc uc_script_name;
//!	"REMOTE_ADDR".
	extern t_cc uc_remote_addr;
//	"REMOTE_PORT".
	extern t_cc uc_remote_port;
#endif

//	response constants
//!	"method".
	extern t_cc c_method;
//!	"GET".
	extern t_cc uc_get;
//!	"POST".
	extern t_cc uc_post;
//!	"Content-Type".
	extern t_cc c_content_type;
//!	default content type: "text/html".
//!	can be modified by "Content-type = ... " in iad.ini
	extern t_cc c_text_html;
//!	for error panic output: "text/plain".
	extern t_cc c_text_plain;
//!	"page".
	extern t_cc c_page;
//!	next page directive: "next".
	extern t_cc c_next;
//!	additional link parameters key "params".
	extern t_cc c_params;
//! default page "start".
	extern t_cc c_start;
//!	login page "login".
	extern t_cc c_login;
//!	"logout" pseudo page.
	extern t_cc c_logout;


//	xtags

//!	the ampersant (&amp;)for call value chains.
	extern t_cc c_amp;
//!	quotation mark (&uot;)for text output formatting.
	extern t_cc c_quot;
//!	greater than sign (&gt;).
	extern t_cc c_gt;
//!	less than sign (&lt).
	extern t_cc c_lt;


//	============================================================
//	specials
//!	hh:mm:ss formatter: "%H:%M:%S".
	extern t_cc c_hms;

//! "window.document.write('".
	extern t_cc c_java_write;
//!	end of window.document.write.
	#define c_java_write_end "');"

//!	"NULL" literal e.g. for tracing.
	extern t_cc uc_null;

//	============================================================
//	log and error messages

	extern t_cc msg_error;
	extern t_cc msg_info;
	extern t_cc msg_trc_enter;
	extern t_cc msg_trc_exit;
	extern t_cc msg_trc_address;
	extern t_cc msg_exceeded;
	extern t_cc msg_negative;
	extern t_cc msg_loading;
	extern t_cc msg_file_not_found;
	extern t_cc msg_case_level;
	extern t_cc msg_no_page;
	extern t_cc msg_no_directory;

//	============================================================
//	some server specials
#ifdef ISSRV
//	INI
//!	name of ini file: "iadsrv.ini".
	extern t_cc c_iadsrv_ini;
//!	"port".
	extern t_cc c_port;
//!	"ip".
	extern t_cc c_ip;
//!	file types to deflate key "deflate".
	extern t_cc c_deflate;
//!	directotory to deflate to ini key "dfldir".
	extern t_cc c_dfldir;
//!	ini key: keep deflated files for future requests "dflkeep".
	extern t_cc c_dflkeep;

//!	"_FILE" key word of request tokenizer.
	extern t_cc uc_file;
//!	"_CTRL" control message tag.
	extern t_cc uc_ctrl;

//	HTTP header

//!	"Accept-Encoding"
	extern t_cc c_accept_encoding;
//!	"Cache-Control: ".
	extern t_cc c_cache_control;
//!	"Connection".
	extern t_cc c_connection;
//!	"Content-Encoding".
//	leave out if no special encoding (identity)
	extern t_cc c_content_encoding;
//!	"Content-Length".
	extern t_cc c_content_length;
//!	"Date: ".
	extern t_cc c_date;
//!	"Etag: ".
	extern t_cc c_etag;
//!	"HTTP/1.x"
	extern t_cc c_http1x;
//!	our server's name: "IADSRV/1.0".
	extern t_cc c_iadsrv;
//!	"If-None-Match" (check for update).
	extern t_cc c_if_none_match;
//!	"Internal Server Error" (500).
	extern t_cc c_internal_error;
//!	"Keep-Alive".
	extern t_cc c_keep_alive;
//!	"Last-Modified".
	extern t_cc c_last_modified;
//!	"max".
	extern t_cc c_max;
//!	"MSIE".
	extern t_cc uc_msie;
//!	"no-store".
	extern t_cc c_no_store;
//!	"Not Found"
	extern t_cc c_not_found;
//!	"Not Modified".
	extern t_cc c_not_modified;
//!	"OK".
	extern t_cc uc_ok;
//!	"Server".
	extern t_cc c_server;
//!	"timeout".
	extern t_cc c_timeout;
//!	"Unsupported Media Type" (415).
	extern t_cc c_unsupported_type;
//!	"User-Agent".
	extern t_cc c_user_agent;
//!	"Vary".
	extern t_cc c_vary;

#endif

#endif // CCS_HPP

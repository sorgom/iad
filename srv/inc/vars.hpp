//| ============================================================
//! @file
//!
//! Centralization of component behaviour switches
//! and runtime variables.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef VARS_HPP
#define VARS_HPP

#include "cmapper.hpp"
#include "stypes.hpp"
#include "nob.hpp"

#include <ctime>

//	============================================================
//	component behaviour switches

//!	new line depending on mode.
	extern char cnl;

//!	htm xml behaviour.
//!	default is off.
//!	ini: xml = 1
	extern bool htm_xml;

//!	html script compatibility: end tags must have <\/tag> style.
//!	default is off.
	extern bool script_quote;

//!	htm tags attribute quoting.
//!	default is off.
	extern bool htm_quotes;

//!	htm table auto td / th class by column.
//!	default is on.
	extern bool table_autoclass;

//!	dcp auto current class for links to same page.
//!	default is on.
	extern bool dcp_current;

//!	representation of real: floating point sign.
	extern char real_fpoint;

//!	representation of real: number of digits.
//!	TODO: ini var for real_digits
	extern int real_digits;

//	============================================================
//	runtime variables

//	time
	extern time_t curr_time;
	extern unsigned currtime;

//!	wrong user input indicator, can be set by any #cproc.
	extern bool input_error;

//!	ajax request info, set by #load_params().
	extern bool is_ajax;

//!	page is login.
	extern bool is_login;

//!	posted form info.
	extern bool is_post;

//!	multi layout selection allowed.
	extern const bool& poly;

//!	server mode as opposed to CGI mode.
	extern const bool& srv;

//!	system control directives.
	extern const unsigned& sctrl;

//	character string links

//!	name of currently called page.
	extern s_cc call_page;

//!	the "common" part.
	extern t_cc common;

//!	standard content type for parsed content.
//!	can be modified by "Content-type = ... " in iad.ini
	extern t_cc content_type;

//!	current lang.
	extern t_ch curr_lang;

//!	current layout.
	extern t_ch curr_lay;

//!	name of currently parsed page (set by #dcp).
	extern t_ch curr_page;

//!	current sid.
	extern t_ch curr_sid;

//!	data directory.
	extern t_cc tmpdir;

//!	database access: database directory, set by #load_ini().
	extern t_cc dbdir;

//!	html files call expansion root used as $droot.
	extern t_cc droot;

//!	html file search root, set by #load_ini().
	extern t_cc htmdir;

//!	language directory.
	extern t_cc langdir;

//!	standard link attributes (set by #dcp).
	extern t_ch link_atts;

//!	link "params" paramter set by #load_params().
	extern t_ch params;

//!	script name environment setting, set by #load_params().
//!	(saves multiple calls of getenv).
	extern t_ch script_name;

//	buffers

//!	the ini storage.
	extern const cmapper& ini;

//!	names of standard cgi variables.
//!	these names must not be used as field names etc.
//!	values are set with #dml instance creation.
	extern const sbuffer& form_vars;

//!	incomming request parameter storage.
	extern const cmapper& cgi;

//!	"current = ... " declaration by htm designer container.
	extern csbuffer bf_current;

//!	dcp pages list.
	extern ncsbuffer bf_pages;

//!	database access: database names, set by #load_ini().
	extern const ncsbuffer& dbnames;

//!	currently set document vars (set by #dcp).
	extern cmapper dcp_vars;

//!	link "params" value listing set by #load_params().
	extern sbuffer paramvs;

//!	layouts: short name -> long desc.
//!	e.g. "actrice" -> "HansiCom Actrice GmbH"
//!	this is handy for quick layout.
//!	if you desire alphabetical order of long descriptions
//!	just #cswap the thing.
	extern const cmapper& layouts;

//!	languages: native long name -> short sign.
//!	e.g. "Francais" -> "fr"
//!	This enables outputters to keep alphabetical order.
	extern const cmapper& langs;

//!	runtime switch vars search array.
	extern const csbuffer& rtvs;

//!	runtime switch vars string "key1,key2,key3"
	extern const s_cc& s_rtvs;

//!	standard link parameters string
//!	assembled by cgi module.
	extern const s_cc& s_lnk;

//	============================================================
//	one time usage vars

//!	one time usage character buffer.
	extern cbuffer cbf1;

//!	one time usage or pseudo string reference buffer.
	extern csbuffer csbf1;

//!	a frequently usable cnob chain.
	extern cnob cnb1, cnb2, cnb3, cnb4;

//!	save length string memory by length.
	fbuffer& rfb(unsigned);
//!	save length string memory by string including copy.
	fbuffer& rfb(t_cc);

//	============================================================
//	var functions

//!	check if a variable value is a $$var.
	bool isvar(t_cc);

//! check if a string contains a $$var.
	bool hasvar(t_cc);

//!	retrieve a value from #dcp_vars with $$var recursion.
	t_cc fvar(t_cc);

//! check if a string contains language literal function "LL()"
//! or a $$var.
//!	(redundant hasvar() functionality for performance reasons).
	bool hasllorvar(t_cc);

//	============================================================
//	misc functions accessing vars

//!	general directory + file + extension concatenator.
	t_fcc dir_file(t_cc dn, t_cc fn, t_cc ext = 0);

//!	standard temp IO file name.
	t_fcc tmp_file(t_cc fn, t_cc ext = 0);

#endif // VARS_HPP

//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include "vars.hpp"
#include "hdn_vars.hpp"

#undef DEVEL
#include "trace_macros.hpp"

#include "ccs.hpp"
#include "defs.hpp"
#include "ini.hpp"
#include "utilz.hpp"

#include <cstdlib>

//	============================================================
//	component behaviour switches

	char cnl 				= '\n';
	bool htm_xml			= false;
	bool script_quote		= false;
	bool htm_quotes			= false;
	bool table_autoclass 	= true;
	bool dcp_current		= true;
	char real_fpoint		= '.';
	int  real_digits		= 2;

//	============================================================
//	runtime variables

	time_t curr_time;
	unsigned currtime;

	bool input_error		= false;
	bool is_ajax			= false;
	bool is_login			= false;
	bool is_post			= false;
	const bool& poly		= hdn::poly;
	const bool& srv			= hdn::srv;

	const unsigned& sctrl	= hdn::sctrl;

	s_cc call_page		= cdef;
	t_cc common			= c_common;
	t_cc content_type	= cdef;
	t_ch curr_lang		= cdef;
	t_ch curr_lay		= cdef;
	t_ch curr_page		= cdef;
	t_ch curr_sid		= 0;
	t_cc tmpdir			= cdef;
	t_cc dbdir			= cdef;
	t_cc langdir		= cdef;
	t_cc droot			= cdef;
	t_cc htmdir			= cdef;
	t_ch link_atts		= 0;
	t_ch params			= 0;
	t_ch script_name	= cdef;

	csbuffer	bf_current		(5);
	ncsbuffer	bf_pages		(5);
	cmapper		dcp_vars		(c_BUF_STEP_DCP_VARS);
	sbuffer		paramvs			(5);

	const cmapper& 		ini 		= hdn::ini;
	const cmapper&		cgi			= hdn::cgi;
	const sbuffer&		form_vars	= hdn::form_vars;
	const ncsbuffer&	dbnames		= hdn::dbnames;
	const cmapper&		layouts		= hdn::layouts;
	const cmapper&		langs		= hdn::langs;
	const csbuffer& 	rtvs		= hdn::rtvs;
	const s_cc& 		s_rtvs		= hdn::s_rtvs;
	const s_cc& 		s_lnk		= hdn::s_lnk;

//	============================================================
//	one time usage vars

	cbuffer		cbf1;
	csbuffer	csbf1;

	cnob cnb1(0);
	cnob cnb2(cnb1);
	cnob cnb3(cnb2);
	cnob cnb4(cnb3);

	fbuffer* pfb = 0;
//!	save length string memory.
	fbuffer& rfb(unsigned l)
	{
		if (pfb && pfb->mx < l) { delete pfb; pfb = 0; }
		if (!pfb) pfb = new fbuffer(l);
		else pfb->clean();
		return *pfb;
	}
//!	save length string memory by string including copy.
	fbuffer& rfb(t_cc cc)
	{
		unsigned l = strlen(cc);
		fbuffer& r = rfb(l);
		memcpy(r.ms(), cc, l + 1);
		return r;
	}

//	============================================================
//	var functions

//!	check if a variable value is a $var.
	bool isvar(t_cc cc)
	{
		if (!( cc &&
			*cc == '$' &&
			is_alphanum(*(cc + 1))
		)) return false;
		t_ch c = cc + 2;
		while (*c) {
			if (!is_alphanum(*c)) return false;
			++c;
		}
		return true;
	}
//! check if a string contains a $var.
	bool hasvar(t_cc cc)
	{
		if (!cc) return false;
		t_ch c = cc;
		while (*c) {
			if (
				*c == '$' &&
				is_alphanum(*(c + 1))
			) return true;
			++c;
		}
		return false;
	}

	t_cc fvar(t_cc k, bool r)
	{
		static csbuffer bfv(c_BUF_STEP_DCP_VARS);
		if (r) bfv.clean();
		else if (bfv.find(k)) return 0;
		bfv.add(k);
		t_ch 	v = cgi.getn(k);
		if (!v) v = dcp_vars.getn(k);
		if (!v) v = ini.getn(k);
		return isvar(v) ? fvar(v + 1, false) : v;
	}

//!	retrieve a value from #dcp_vars with $var recursion.
	t_cc fvar(t_cc k)
	{
		TRACE_FLOW
		TRACE_VARQ(k)
		t_ch v = fvar(k, true);
#ifdef DEFVEL
		if (v) { TRACE_VARQ(v) }
#endif
		return v;
	}

//! check if a string contains language literal function "LL()".
	bool hasllorvar(t_cc cc)
	{
		if (!cc) return false;
		t_ch c = cc;
		while (*c) {
			if (
				*c == 'L' &&
				*(c + 1) == 'L' &&
				*(c + 2) == '('
			) return true;
			if (
				*c == '$' &&
				is_alphanum(*(c + 1))
			) return true;
			++c;
		}
		return false;
	}

//	============================================================
//	misc functions accessing vars

//!	general directory + file + extension concatenator.
	t_fcc dir_file(t_cc dn, t_cc fn, t_cc ext)
	{
		cbf1.clean() << dn << c_DIR_SEP << fn;
		if (ext) cbf1 << ext;
		return cbf1;
	}

//!	standard data IO file name.
	t_fcc tmp_file(t_cc fn, t_cc ext)
	{
		return dir_file(tmpdir, fn, ext ? ext : c_ext_dat);
	}

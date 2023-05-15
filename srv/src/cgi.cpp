//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include <cgi.h>

#include <ccs.h>
#include <defs.h>
#include <f_io.h>
#include <hdn_vars.h>
#include <ini_io.h>
#include <utilz.h>
#include <vars.h>

// #undef DEVEL
// #define XDEVEL
#include <trace_macros.h>

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>

//	============================================================
//	hidden subs
//!	process tokenized parameters.
	void process_params();

//	check for valid SID
	bool oksid;

//!	current SID, either from file (cgi) or stored separately (server).
	s_cc s_sid;

//	sid creation function poniter.
	t_sid_func* sid_func = &time_sid;

//	password evaluation function pointer.
	t_pwd_eval_func* eval_func = &pwd_eval_any;

//	password save function pointer.
	t_pwd_save_func* save_func = 0;

//!	general cgi parameter call string to cmapper tokenizer.
	void parse_params(cmapper& cm, t_cc bf, bool addp = false);

//	============================================================
//	server section
#ifdef ISSRV
//!	parse paramter string from server.
//!	@param cp parameter string (GET / POST)
//!	@param bp inicates POST method
	void srv_params(t_cc cp, bool bp, const s_cc& sid)
	{
		TRACE_FLOW
		is_post = bp;
		s_sid = sid;
		hdn::cgi.clean();
		if (cp) parse_params(hdn::cgi, cp);
		script_name = "/";
		process_params();
	}

//	============================================================
//	cgi section
#else

//!	Load parameters from CGI GET / POST.
	void cgi_params()
	{
		TRACE_FLOW
		hdn::cgi.clear();
		is_post = false;

		load_sid(s_sid);

		t_ch c = getenv(uc_request_method);

		if (c) {
			TRACE_VARQ(c)
		//	GET
			if (!strcmp(c, uc_get)) {
				c = getenv(uc_query_string);
				if (c) parse_params(hdn::cgi, c);
			}
		//	POST
			else {
				is_post = true;
				c = getenv(uc_content_length);
				if (c) {
					size_t l = atoi(c);
					if (l) {
						fbuffer& rb = rfb(l);
						{ size_t s = fread (rb.ms(), 1, l, stdin); }
						rb.ms()[l] = 0;
						parse_params(hdn::cgi, rb.str());
					}
				}
			}
		}
	//	commonly accessible
		script_name = getenv(uc_script_name);
		process_params();
		if (oksid && !is_ajax) save_sid(s_sid);
	}
#endif

//	============================================================
//	common
	void mk_paramvals()
	{
		if (params) {
			fbuffer& rb = rfb(params);
			char* cp = rb.ms();
			char* c2 = cp;
			while (*c2) {
				if (*c2 == ':') *c2 = '=';
				else if (*c2 == '!') *c2 = '&';
				++c2;
			}
			parse_params(hdn::cgi, rb.str(), true);
		}
	}
//!	process tokenized parameters.
	void process_params()
	{
		TRACE_FLOW
		time(&curr_time);
		currtime = static_cast<unsigned>(curr_time);

		TRACE_VAR(cgi.size())

		curr_sid	= 0;
	//	runtime vars
		hdn::s_rtvs = 0;
		hdn::rtvs.clear();

	//	if no parameters at all
		if (!cgi) {
		//	language and layout from ini
			curr_lang	= ini.get(c_lang);
			curr_lay	= ini.get(c_layout);
			call_page	= c_login;
			is_ajax		= false;
		}
	//	cgi given
		else {

		//	language	: lang, plg, ini
			ctry(curr_lang)
				<< cgi.getn(c_lang)
				<< cgi.getn(c_plg)
				<< ini.get(c_lang);
		//	layout		: layout, ply, ini
			ctry(curr_lay)
				<< cgi.getn(c_layout)
				<< cgi.getn(c_ply)
				<< ini.get(c_layout);
		//	page		: page, login
			{
				t_ch cp;
				ctry(cp)
					<< cgi.getn(c_page)
					<< c_login;
				call_page = cp;
			}

		//	ajax		: as comes
			is_ajax = cgi.getn(c_ajax);

		//	params		: clear
			params = 0;
			paramvs.clear();

			TRACE_INF("cgi given setup")
			TRACE_VARQ(curr_sid)
			TRACE_VARQ(curr_lang)
			TRACE_VARQ(curr_lay)
			TRACE_VARQ(call_page)

		//	SID treatment
			oksid = false;
			{
				t_cc isid = cgi.getn(c_sid);
				t_cc cqed = is_post ? cgi.getn(c_qed) : 0;
			//	login attempt.
				if (cqed) {
					TRACE_VARQ(cqed)
				//	correct login: create new sid
					if (oksid = (*eval_func)(cqed)) {
					//	create new sid
						s_sid.copy((*sid_func)());
					//	and redirect to start page
						call_page = c_start;
					//	save password
						if (save_func) (*save_func)(cqed);
					}
				}
			//	no sid given: set default layout and language
				else if (!isid) {
					TRACE_INF("no SID")
				}
			//	logout: treated like not ok
				else if (!strcmp(call_page, c_logout)) {
					TRACE_INF(c_logout)
				}
			//	check sid against last stored one
				else {
					TRACE_INF("check SID")
					oksid = (s_sid && !strcmp(isid, s_sid));
				}
				TRACE_VARB(oksid)
			}
		//	SID ok
			if (oksid) {
				curr_sid = s_sid;
			//	only after correct login:
			//	check for "params" parameter
				if (!is_ajax) {
					params = cgi.getn(c_params);
					mk_paramvals();
				}
			//	language based auto detection of floating point
			//	fpt.lang, fpt.
				{
					cbf1.clean() << c_fpt << curr_lang;
					t_ch cfp;
					ctry(cfp)
						<< ini.getn(cbf1)
						<< ini.getn(c_fpt)
						<< ".";
					real_fpoint = cfp[0];
					real_digits = atoi(ini.get(c_digits));
				}
			//	check for incomming runtime vars
			//	TODO check for incomming runtime vars when ajax?
				{
					TRACE_INF("runtime vars")
					split(cgi.getn(c_rtvs), hdn::rtvs, 0, '_');
					unsigned n = 0;
					static unsigned l = strlen(uc_rtv_n);
				//	check incomming "RTV_N_.."
				//	no check if no values yet
					if (hdn::rtvs) {
						TRACE_INF("RTV_N_.. ?")
						csbf1.clear();
						if (cgi.grep(csbf1, uc_rtv_n, l, true))
							for (unsigned i = csbf1.size(); i--;) hdn::rtvs.remove(csbf1[i] + l);

					}
				//	check incomming "RTV_Y_.."
				//	always possible
					TRACE_INF("RTV_Y_.. ?")
					csbf1.clean();
					if (cgi.grep(csbf1, uc_rtv_y, l, true))
						for (unsigned i = csbf1.size(); i--;) hdn::rtvs.copy(csbf1[i] + l);
				//	assemble string
					TRACE_INF("join ..")
					if (hdn::rtvs) join(hdn::s_rtvs, hdn::rtvs, '_');
					TRACE_VARQ(s_rtvs);
					csbf1.clear();
				}
			}
		//	SID nok:
			else {
				curr_sid = 0;
				hdn::cgi.remove(c_sid);
			//	ajax call: ignore
				if (is_ajax)  {
					call_page = 0;
					hdn::cgi.remove(c_page);
				}
			//	other: return to login
				else call_page = c_login;
				is_post = false;
			}



		} // cgi given

		TRACE_INF("cgi processed")
		TRACE_VARQ(curr_sid)
		TRACE_VARQ(curr_lang)
		TRACE_VARQ(curr_lay)
		TRACE_VARQ(call_page)
		TRACE_VARB(is_ajax)
		TRACE_VARQ(s_rtvs)

	//	rewrite to ini (e.g. for fvar)
		curr_lang = hdn::cgi.copyv(c_lang,		curr_lang);
		curr_lay  = hdn::cgi.copyv(c_layout,	curr_lay);
		call_page = hdn::cgi.copyv(c_page,		call_page);
		curr_sid  = hdn::cgi.copyv(c_sid,		curr_sid);

	//	convenience: set brand
		hdn::cgi.set(c_brand, layouts.getk(curr_lay));
	//	convenience: set language long description
		hdn::cgi.set(c_language, ini.getk(curr_lang));

	//	runtime vars that are reset to ini defaults with each round
		htm_xml = ini.getn(c_xml);

	//	assembly of standard link parameters string.
		cbf1.clean();
		cbf1 << script_name << '?'
#ifndef ISSRV
			<< "t=" << currtime << c_amp
#endif
			<< c_plg << '=' << curr_lang;
		if (poly    ) cbf1 << c_amp << c_ply << '=' << curr_lay;
		if (s_rtvs  ) cbf1 << c_amp << c_rtvs << '=' << s_rtvs;
		if (curr_sid) cbf1 << c_amp << c_sid << '=' << curr_sid;
		cbf1.over(hdn::s_lnk);
	}

//	============================================================

	bool check_params(t_cc cc)
	{
		TRACE_FLOW
		if (!cc || (params && !strcmp(cc, params))) return true;
		t_ch c1 = cc;
		t_ch c2 = cc;
		s_cc sk;
		s_cc sv;
		while (*c1) {
			if (*c2 == ':') {
				if (c2 > c1) sk.copy(c1, c2 - c1);
				++c2;
				c1 = c2;
			}
			else if ((*c2 == '!') || !*c2) {
				if (sk && c2 > c1) {
					sv.copy(c1, c2 - c1);
					t_cc cv = fvar(sk);
					if (!cv || strcmp(cv, sv)) return false;
				}
				if (*c2) {
					++c2;
					sk = 0;
					sv = 0;
				}
				c1 = c2;
			}
			else ++c2;
		}
		return true;
	}

//!	clear cgi container from all non system (i.e. form) entries.
	void clear_cgi()
	{
		hdn::cgi.intersec(form_vars);
		mk_paramvals();
	}

//	============================================================
//!	definition of individual sid creation function.
	void set_sid_func(t_sid_func* pf) { sid_func = pf; }

#ifndef ISSRV
//!	cgi environment sid creation function.
	t_fcc cgi_sid()
	{
		t_cc cra = getenv(uc_remote_addr);
		t_cc crp = getenv(uc_remote_port);
		if (cra && crp) {
			cbf1.clean() << cra << ':' << crp;
			return cbf1;
		}
		else return time_sid();
	}
#endif

//!	simple time based sid creation function.
	t_fcc time_sid()
	{
		struct tm* ptm = gmtime(&curr_time);
		cbf1.clean();
		cbf1.add(uc22h(static_cast<unsigned char>(ptm->tm_sec  + 100)));
		cbf1.add(uc22h(static_cast<unsigned char>(ptm->tm_min  + 100)));
		cbf1.add(uc22h(static_cast<unsigned char>(ptm->tm_hour + 100)));
		return cbf1;
	}

//!	load SID from tmp file.
	void load_sid(s_cc& s)
	{
		TRACE_FLOW
		s.take(rfile(tmp_file(c_sid)));
	}
//!	save SID to tmp file.
	void save_sid(const s_cc& s)
	{
		TRACE_FLOW
		if (s) wfile(tmp_file(c_sid), s);
	}

//	============================================================
//	password

//!	set password functions.
	void set_pwd_funcs(t_pwd_eval_func* ef, t_pwd_save_func* sf)
	{
		if (ef) eval_func = ef;
		save_func = sf;
	}

//!	dummy password evaluation function.
	bool pwd_eval_any(t_cc c) { return c && c[0]; }

//	============================================================
//	internal subs
//!	general cgi parameter call string to cmapper tokenizer.
	void parse_params(cmapper& cm, t_cc bf, bool addp)
	{
		TRACE_FLOW
		TRACE_VARQ(bf)
		TRACE_VARB(addp)
		t_ch c1 = bf;
		t_ch c2 = bf;
		s_cc sk;
		s_cc sv;
		while (*c1) {
			if (*c2 == '=') {
				if (c2 > c1) sk.copy(c1, c2 - c1);
				++c2;
				c1 = c2;
			}
			else if ((*c2 == '&') || !*c2) {
				if (sk && c2 > c1) {
					sv.copy(c1, c2 - c1);
					unqs(sk.ms());
					unqs(sv.ms());
					TRACE_VARQ(sk.str())
					TRACE_VARQ(sv.str())
					t_cc cv = cm.set(sk, sv, false);
					if (addp) paramvs.add(cv);
				}
				if (*c2) {
					++c2;
					sk = 0;
					sv = 0;
				}
				c1 = c2;
			}
			else ++c2;
		}
		cm.sort();
		TRACE_VAR(cm.size())
	}

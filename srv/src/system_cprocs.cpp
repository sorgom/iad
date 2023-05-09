//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include "system_cprocs.hpp"

#include "cgi.hpp"

// #define XDEVEL
#include "cp_convenience.hpp"
#include "finders.hpp"
#include "fcp.hpp"
#include "hdn_vars.hpp"
#include "sysctrl.hpp"
#include <cstring>

using namespace ta;
using namespace tag;
using namespace bbl;

//	============================================================
//	cp_layout

//!	traverse swapped layouts: long desc -> short name.
	void cp_layout::operator()(const s_cc& lg, const s_cc& sh)
	{
		dml << '\t' << option() << value(sh);
		if (!strcmp(curr_lay, sh)) dml << selected();
		dml << lg << tte << cnl;
	}

	void cp_layout::put(sbuffer&)
	{
		if (poly) {
		//	swap
		//	from: short name -> long desc.
		//	to  : long desc -> short name
			cmapper cms(layouts.size());
			cswap(cms, layouts);
			cms.sort();
			dml << select() << name(c_layout) << size(cms.size()) << cnl;
			cms.traverse(*this);
			dml << tte << cnl;
		}
	}

//	============================================================
//	cp_quick_layout

//!	traverse swapped layouts: short name -> long desc
	void cp_quick_layout::operator()(const s_cc& sh, const s_cc& lg)
	{
	//	sparator
		if (sep) dml << ' ' << c_sep << ' ';
		sep = true;
	//	title
		t_cc ttl = lg;
		t_cc c	 = sh;
	//	same or different
		bool dl = strcmp(curr_lay, c);
	//	layout shorts
		if (dl && !unique) {
			cmapper cml(1);
			cmapper cma(1);
			cml.set(c_layout, c);
			cma.set(c_title, ttl);
			link(c, cml, &cma);
		}
		else {
			dml << span() << title(ttl)
				<< cl(dl ? c_nok : c_ok)
				<< c << tte;
		}
	}

	void cp_quick_layout::put(sbuffer& sb)
	{
		if (poly) {
			unique = dcp_vars.getn(c_unique);
			c_sep = sb ? sb.shift() : "|";
			sep = false;
			layouts.traverse(*this);
		}
	}


//	============================================================
//	cp_language

//!	traverse langs: native long name -> short sign
	void cp_language::operator()(const s_cc& lg, const s_cc& sh)
	{
		dml << '\t' << option() << value(sh) << cl(sh);
		if (!strcmp(curr_lang, sh)) dml << selected();
		dml << lg << tte << cnl;
	}

	void cp_language::put(sbuffer&)
	{
		dml << select() << name(c_lang) << size(langs.size()) << id(c_language) << cnl;
		langs.traverse(*this);
		dml << tte << cnl;
	}

//	============================================================
//	cp_quick_language

	void cp_quick_language::operator()(const s_cc& lg, const s_cc& sh)
	{
	//	cast to t_cc
		t_cc cl = lg;
		t_cc cs = sh;
	//	sparator
		if (sep) dml << ' ' << c_sep << ' ';
		sep = true;
	//	same language: just a span
		if (!strcmp(curr_lang, cs)) dml << span() << cl << tte;
	//	different language: link
		else {
			cmapper cml(1);
			cml.set(c_lang, cs);
			link(cl, cml);
		}
	}

	void cp_quick_language::put(sbuffer& sb)
	{
		c_sep = sb ? sb.shift() : "|";
		sep = false;
		langs.traverse(*this);
	}

//	============================================================
//	cp_login
	void cp_login::put(sbuffer&)
	{
		dml
			<< label() << trans(c_Password) << tte
			<< br()
			<< input() << type(c_password) << name(c_qed) << cdef
		;
	}


//	============================================================
//	cp_LL
	bool cp_LL::ok(sbuffer& sb)
	{
		if (!sb) return false;
		for (int i = sb.size(); i--;)
			if (!(sb[i] && bbl::mapper.getn(sb[i]))) return false;
		return true;
	}

//	============================================================
//	cp_rtv
	bool cp_rtv::ok(sbuffer& sb)
	{
		return sb && rtvs.find(sb.shift());
	}
	void cp_rtv::set(sbuffer& sb)
	{
		if (!sb) return;
		t_cc k = sb.shift();
		bool rb = true;
		if (sb) {
			t_cc v = sb[0];
			for (int i = 6; i--;) {
				if (!strcmp(v, bswitch_vals[i])) {
					sb.shift();
					rb = (i < 3);
					break;
				}
			}
		}
		if (rb) hdn::rtvs.copy(k);
		else hdn::rtvs.remove(k);
		hdn::s_rtvs = 0;
		join(hdn::s_rtvs, hdn::rtvs);
	}

	void cp_rtv::put(sbuffer& sb)
	{
		if (!sb) return;
		t_cc k = sb.shift();

	//	current value
		bool okc = rtvs.find(k);
	//	dml output
		cbf1.clean() << uc_rtv_y << k;
		s_cc sc;
		cbf1.over(sc);
	//	chekbox "RTV_Y_key"
		dml << p() << cl(cpnm) << input() << type(c_checkbox) << name(sc) << id(sc);
		if (okc) dml << checked();
		dml << cnl;
	//	label
		cbf1 << c_rtv_ << k;
		dml << label() << lfor(sc) << trans(cbf1) << tte
			<< tte;
	//	hidden input "RTV_N_key"
		cbf1.clean() << uc_rtv_n << k;
		dml	<< input() << type(c_hidden) <<  name(cbf1) << value(1)
			<< cnl;
	}
//	============================================================
//	cp_rtv1
	void cp_rtv1::put(sbuffer& sb)
	{
		if (!sb) return;
		t_cc k = sb.shift();

	//	current value
		bool okc = rtvs.find(k);

	//	dml output
		t_cc con = okc ? c_On : c_Off;
		t_cc cof = okc ? c_Off : c_On;
		cbf1.clean()  << (okc ? uc_rtv_n : uc_rtv_y) << k;
	//	outer divs class SVB, class submit
		dml << div() << cl(cpnm) << div() << cl(c_submit)
		//	encapsulating span eneaysies css
		//	span with On / Off text
			<< span() << cl(con) << span() << trans(con) << tte << tte << cnl
		//	submit button
			<< input() << type(c_submit) << cl(c_submit) << name(cbf1) << value(trans(cof))
		//	end of divs
			<< tte << tte
		;
	}
//	============================================================
//	cp_rtv_info
	void cp_rtv_info::put(sbuffer& sb)
	{
		if (!rtvs) return;
		t_cc csp = sb ? sb.shift() : ", ";
		t_cc cpr = sb ? sb.shift() : cdef;
		dml << div() << cl(cpnm);
		for (unsigned i = 0; i < rtvs.size(); ++i) {
			if (i) dml << csp;
			t_cc k = rtvs[i];
			cbf1.clean() << c_rtv_ << k;
			dml << span() << cpr << trans(cbf1) << tte;
		}
		dml << tte;
	}
//	============================================================
//	cp_rtv_ico
	void cp_rtv_ico::put(sbuffer& sb)
	{
		if (!rtvs) return;
		dml << div() << cl(cpnm);
	//	create a link if parameter given
		bool bl = sb;
		if (bl) {
			cmapper cmp(1);
			cmp.set(c_page, sb.shift());
			start_link(cmp);
		}
		for (unsigned i = 0; i < rtvs.size(); ++i) {
			if (i) dml << cnl;
			t_cc k = rtvs[i];
			cbf1.clean() << c_rtv_ << k;
			dml << span() << cl(k) << title(trans(cbf1)) << c_nbsp << tte;
		}
		if (bl) dml << tte;
		dml << tte;
	}

//	============================================================
//	cp_DEF
	bool cp_DEF::ok(sbuffer& sb)
	{
		if (!sb) return false;
		for (int i = sb.size(); i--;)
			if (!(sb[i] && sb[i][0])) return false;
		return true;
	}
//	============================================================
//	cp_page_help
	bool phdef = true;

	void cp_page_help::put(sbuffer& sb)
	{
		t_ch ca = sb ? sb.shift() : ini.get(c_no_help).str();
		if (check_once(sb)) dml << format(ch);
		else dml << trans(ca);
	}
//!	check once: help available.
	bool cp_page_help::my_check(sbuffer&)
	{
		TRACE_FLOW
		ch = 0;
	//	help order buffer
		ncsbuffer bf(2);
		t_ch cp;
	//	if help page given: check only for this
		if (cp = fvar(c_hpg)) {
			TRACE_INF(c_hpg << ": " << cp)
			cbf1.clean() << c_help << '_' << cp;
			bf.take(cbf1);
		}
	//	otherwise ...
		else {
			cp = call_page;
			TRACE_INF(c_page << ": " << cp)
		//	if parameters given: assamble page help with parameters
		//	in order of appearance
		//	help_<page>_<value>_<value> ....
			if (paramvs) {
				TRACE_INF(c_params << ": " << paramvs.size())
				cbf1.clean() << c_help << '_' << cp;
				for (unsigned i = 0; i < paramvs.size(); ++i)
					cbf1 << '_' << paramvs[i];
				bf.take(cbf1);
			}
		//	if no params or page_help_default (phdef) set:
		//	add help_<page>
			if (phdef || !paramvs) {
				TRACE_INF(c_page)
				cbf1.clean() << c_help << '_' << cp;
				bf.take(cbf1);
			}
		}
	//	now we have our questioned help items in order
	//	check if they are available
		for (unsigned i = 0; i < bf.size(); ++i) {
			TRACE_VAR(bf[i])
			t_cc cc = mapper.getn(bf[i]);
			if (cc) {
				ch = cc;
				break;
			}
		}
		return ch;
	}

//	============================================================
//	cp_page_path

//	page title
	t_cc cp_page_path::pgt(t_cc pn)
	{
		cbf1.clean() << "pg_" << pn;
		t_cc ct = mapper.getn(cbf1);
		return ct ? ct : pn;
	}

	bool cp_page_path::ok(sbuffer&)
	{
		return bf_pages || fvar(c_subject);
	}

	void cp_page_path::put(sbuffer&)
	{
		t_cc cs = fvar(c_subject);
		t_ch pn;
		cmapper cmq(1);
	//	current pages
		for (unsigned i = 0; i < bf_pages.size(); ++i) {
		//	page name
			pn = bf_pages[i];
		//	make the link
			cmq.set(c_page, pn);
			link(pgt(pn), cmq, 0, false);
			dml << cnl << ">>" << cnl;
		}
	//	call page
		if (cs) {
			pn = call_page;
			cmq.set(c_page, pn);
			link(pgt(pn), cmq, 0, false);
			dml << cnl << ">>" << cnl
				<< span() << trans(cs) << tte;
		}
	//	call page only
		else dml << span() << pgt(call_page) << tte;
	}
//	============================================================
//	cp_current

//	special behaviour for ok:
//	with parameter(s): check if all of them are set current,
//	without just return dcp_current is set.
	bool cp_current::ok(sbuffer& sb)
	{
		if (!sb) return dcp_current;
		while (sb) {
			t_cc pg = sb.shift();
			if (
				!bf_current.find(pg) &&
				strcmp(pg, call_page)
			) return false;
		}
		return true;
	}

//	============================================================
//	cp_sysreset

	bool cp_sysreset::ok(sbuffer&)
	{
		return is_issue(cpnm);
	}
	void cp_sysreset::put(sbuffer&)
	{
		set_issue(cpnm);
		if (is_issue(cpnm)) {
			ctrl::process(ctrl::sysreset);
			db::proc::instance().disconnect();
			fcp::remove();
			hdn::sctrl = ctrl::restart;
		}
	}

//	============================================================
//	cp_prompt

	bool cp_prompt::ok(sbuffer& sb)
	{
		TRACE_FLOW
		TRACE_VARQ(cpnm)
		cbf1.clean() << cpnm;
		if (sb) cbf1 << '_' << sb.shift();
		s_cc sci;
		cbf1.over(sci);
		bool si;
		bool rt;
	//	second or third call: issue set
		if (is_issue(sci)) {
		//	third call ("_PROMPTED_" indicator set)
			if (cgi.getn(c_pr_prompted)) {
				TRACE_INF("third call")
				si = true;
				rt = false;
			//	if "_YES_" was not submitted: clear all cgi contents
				if (!cgi.getn(c_pr_yes)) clear_cgi();
			}
		//	second call ("_PROMPTED_" indicator not set)
			else {
				TRACE_INF("second call")
				si = false; // replication would double
				rt = true;
			//	replicate all cgi parameters (except form ones and own issue)
				cgi.traverse(*this);
			//	set prompted
				dml << tag::input() << ta::type(c_hidden) << ta::name(c_pr_prompted) << ta::value(1) << cnl;
			}
		}
	//	first and other call
		else {
			TRACE_INF("first call")
			si = true;
			rt = false;
		}
		if (si) set_issue(sci);
		return rt;
	}

	void cp_prompt::put(sbuffer&)
	{
		dml
		//	Yes with name "_YES_"
			<< tag::input() << ta::type(c_submit) << ta::cl(c_submit)
			<< ta::name(c_pr_yes) << ta::value(trans(bbl::c_Yes)) << cnl
		//	No
			<< tag::input() << ta::type(c_submit) << ta::cl(c_submit)
			<< ta::value(trans(bbl::c_No)) << cnl
		;
	}

	void cp_prompt::operator()(const s_cc& k, const s_cc& v)
	{
		if (!form_vars.find(k))
			dml << tag::input() << ta::type(c_hidden) << ta::name(k) << ta::value(v) << cnl;
	}

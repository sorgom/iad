//| ============================================================
//! @file
//!
//!	HTML output makers.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include <dml.h>

#include <bbl_mapper.h>
#include <cmapper.h>
#include <defs.h>
#include <cgi.h>
#include <hdn_vars.h>
#include <ini.h>
#include <stypes.h>
#include <vars.h>

#undef DEVEL
#include <trace_macros.h>

#include <cstdlib>
#include <cstring>


//	============================================================
//	dmlstream
	dmlstream& dmlstream::instance()
	{
		static dmlstream inst_dml(c_BUF_STEP_DML_OUT);
		return inst_dml;
	}
	dmlstream& dml = dmlstream::instance();

//	dmlstream& dmlstream::tmp_instance()
//	{
//		static dmlstream inst_dml(c_BUF_STEP_DML_TMP);
//		return inst_dml;
//	}

//	dmlstream& tmpdml = dmlstream::tmp_instance();

	dmlstream::dmlstream(unsigned bsz)
	: open(false)
	, aopen(false)
	, sgl(false)
	, tbf(10)
	, bfout(bsz)
	{
		hdn::form_vars.clean();
		hdn::form_vars
			<< c_page << c_sid << c_onerror << c_params
			<< c_plg  << c_ply << c_lang    << c_layout
			<< c_next
		;
		hdn::form_vars.sort();
	}



//	============================================================
//	tags
//!	open tag
	dmlstream& dmlstream::operator <<(ttag t)
	{
		check_open();
		add('<');
		add(t.name);
		open = true;
		if (t.st) tbf.add(t.name);
		return *this;
	}
//!	open single tag
	dmlstream& dmlstream::operator <<(stag t)
	{
		check_open();
		add('<');
		add(t.name);
		open = true;
		sgl = true;
		return *this;
	}
//!	tag end
	dmlstream& dmlstream::operator <<(tt_end)
	{
		check_open();
		if (tbf) {
			t_cc tn = tbf.pop();
			if (tn) {
				add(script_quote ? c_tclm : c_tcl);
				add(tn);
				add('>');
			}
		}
		return *this;
	}

//	============================================================
//	attributes

//	general quoting
	template <class T> void dmlstream::addq(taa<T> a)
	{
		add(' '); add(a.name); add('=');
		if (htm_quotes || a.q) add('"');
		add(a.val);
		if (htm_quotes || a.q) add('"');
	}

//	standard: t_cc
//	TODO	void dmlstream::attr(taa<t_cc>	a) { if (a.val && a.val[0]) addq(a); }
	void dmlstream::attr(taa<t_cc>	a) { if (a.val) addq(a); }
//	standard: nob
	void dmlstream::attr(taa<nob>	a) { addq(a); }
//	standard: numeric
	void dmlstream::attr(taan       a) { add(' '); add(a.name); add('='); add(a.val); }

	void dmlstream::attr(taa<t_num>	a) { add(' '); add(a.name); add('='); add(a.val); }
//	standard: t_cc (expicit type)
	void dmlstream::attr(taac a)
	{
		if (!a.val) return;
		add(' '); add(a.name); add('=');
		if (htm_quotes || a.q) add('"');
		add(a.val);
		if (htm_quotes || a.q) add('"');
	}

//	literal attribute
	void dmlstream::attr(laa a) { add(' '); add(a.str); }

//	single attribute (no value in html)
	void dmlstream::attr(saa a) {
		add(' ');
		add(a.name);
		if (htm_xml) {
			add('=');
			add(a.name);
		}
	}
//	attribute opener
	void dmlstream::attr(oaa a)
	{
		aopen = true;
		add(' '); add(a.name); add('='); add('"');
	}
//	attribute closer
	void dmlstream::attr(eaa a)
	{
		if (aopen) add('"');
		aopen = false;
	}
//	stream functor attribute
	void dmlstream::attr(sfaa a)
	{
		if (a.sf->data()) {
			aopen = true;
			add(' '); add(a.name); add('='); add('"');
			(*a.sf)(*this);
			add('"');
			aopen = false;
		}
		delete a.sf;
	}

//	general open check
	void dmlstream::check_open() {
		if (aopen) return;
		if (open) {
			if (sgl && htm_xml) add(" /");
			add('>');
		}
		open = false;
		sgl  = false;
	}

//	============================================================
//	tag attributes
namespace ta
{
	eaa ea;

	t_cc quoted_ones[] = { c_QUOTED_ATTS };
	bool quoted(t_cc c)
	{
		t_cc* pc = quoted_ones;
		for (int i = sizeof(quoted_ones); i--; ++pc) if (*pc == c) return true;
		return false;
	}
}

//	============================================================
//	NEW table stream classes / functions
namespace tbl
{
	cinob ncol("c", 0);

	void mktr(bool cl = true)
	{
		TRACE_FLOW
		bool ok = false;
		while (dml && !(dml == c_table)) {
			if (dml == c_tr && !cl) {
				ok = true;
				break;
			}
			dml << tte;
		}
		if (!ok) {
			dml << cnl;
			dml << tag::mtt(c_tr);
			ncol = 0;
		}
	}

	class sf_tbl : public sfunc
	{
	public:
		void operator()(outstream&) const
		{
			TRACE_FLOW
			dml << tag::mtt(c_table);
			table_tbfunc();
		}
	};
	sfunc* tb() { return new sf_tbl; }

	class sf_tr : public sfunc
	{
	public:
		inline void operator()(outstream&) const { mktr(true); }
	};

	sfunc* tr()		{ return new sf_tr; }


	class sf_td : public sfunc
	{
	public:
		inline sf_td(t_cc c, bool b)
		: t(c)
		, cl(b)
		{}
		t_ch t;
		bool cl;
		void operator()(outstream&) const
		{
			mktr(false);
			dml << cnl << '\t' << tag::mtt(t);
			if (table_autoclass && cl) {
				++ncol;
				dml << ta::cl(ncol);
			}
		}
	};

	sfunc* mktd(t_cc t, bool cl = true)
	{
		return new sf_td(t, cl);
	}

	sfunc* th () { return mktd(c_th); }
	sfunc* tha() { return mktd(c_th, false); }
	sfunc* td () { return mktd(c_td); }
	sfunc* tda() { return mktd(c_td, false); }

	class sf_tbe : public sfunc
	{
	public:
		void operator()(outstream&) const
		{
			TRACE_FLOW
			while (dml && !(dml == c_table)) dml << tte;
			if (dml == c_table) dml << cnl << tte << cnl;
		}
	};

	sfunc* tbe() { return new sf_tbe; }


} // namespace tbl

//	============================================================
//!	form issue sub

	s_cc s_issue;

	bool mk_issue(t_cc c1, t_cc c2, t_cc c3)
	{
		if (!c1) return false;
		cbf1.clean() << c_form_issue << '_' << c1;
		if (c2) cbf1 << '_' << c2;
		if (c3) cbf1 << '_' << c3;
		cbf1.over(s_issue);
		return true;
	}


//!	form issue
	void set_issue(t_cc c1, t_cc c2, t_cc c3)
	{
		if (!mk_issue(c1, c2, c3)) return;
		dml << tag::input() << ta::type(c_hidden) << ta::name(s_issue) << ta::value(1) << cnl;
	}

//!	convenience: check if POSTed form is relevant issue.
	bool is_issue(t_cc c1, t_cc c2, t_cc c3)
	{
		if (!is_post) return false;
		if (c1) mk_issue(c1, c2, c3);
		return cgi.getn(s_issue);
	}

//	============================================================
//	FORM
	void attf(const s_cc& k, const s_cc& v)
	{
		if (
			strcmp(c_next,		k) &&
			strcmp(c_onerror,	k) &&
			strcmp(c_params,	k) &&
			strcmp(c_method,	k) &&
			strcmp(c_action,	k)
		) dml << ta::maa<t_cc>(k, v);
	}
//!	form begin.
	void form(cmapper& cma)
	{
		TRACE_FLOW

		t_ch c = script_name;

	//	begin form tag
		dml << tag::mtt(c_form, false) << ta::maa<t_cc>(c_method, uc_post);
		if (c) dml << ta::maa<t_cc>(c_action, c, true);

	//	additional attributes
		cma.traverse(&attf);
	//	end of form tag
		dml << cnl;

	//	hidden inputs
	//	- page / next page
		c = cma.getn(c_next);
		if (!c) c = call_page;
		dml << tag::input() << ta::type(c_hidden) << ta::name(c_page) << ta::value(c) << cnl;

	//	- session id
		if (curr_sid) dml << tag::input() << ta::type(c_hidden) << ta::name(c_sid) << ta::value(curr_sid) << cnl;

	//	- on error
		if (c = cma.getn(c_onerror)) {
			if (!strcmp(c, c_this)) c = call_page;
			dml << tag::input() << ta::type(c_hidden) << ta::name(c_onerror) << ta::value(c) << cnl;
		}
	//	- params
		if (params) dml << tag::input() << ta::type(c_hidden) << ta::name(c_params) << ta::value(params) << cnl;

	//	- language (and layout)
		dml << tag::input() << ta::type(c_hidden) << ta::name(c_plg) << ta::value(curr_lang) << cnl;
		if (poly)
		dml << tag::input() << ta::type(c_hidden) << ta::name(c_ply) << ta::value(curr_lay) << cnl;

	//	- runtime vars
		if (s_rtvs)
		dml << tag::input() << ta::type(c_hidden) << ta::name(c_rtvs) << ta::value(s_rtvs) << cnl;

	}

//	============================================================
//	LINK

//!	attribute maker traverser.
	void attt(const s_cc& k, const s_cc& v)
	{
		if (
			!form_vars.find(k.str()) &&
			strcmp(c_href, k) &&
			strcmp(c_src, k)
		) 	dml << ta::maq<t_cc>(k, v);
	}

//!	parameter maker traverser.
	void vpar(const s_cc& k, const s_cc& v)
	{
		dml << c_amp << k << '=' << v;
	}

//!	parameter masking.
//!	used for link "params" attribute.
	void um_param(const s_cc& s)
	{
		if (!s) return;
		s.mr().copy(unquote(s));
		char* c = s.ms();
		while (*c) {
			if (*c == '=') *c = ':';
			else if (*c == '&') *c = '!';
			++c;
		}
	}

//!	standard link from parsed contents.
	void link(cmapper& cm)
	{
		TRACE_FLOW
		dml << tag::mtt("a", false);
	//	page can be "this"
		t_ch cp = cm.getn(c_page);
		if (!cp || !strcmp(cp, c_this)) cp = call_page;
	//	open href, page, time
		dml << ta::oa(c_href) << s_lnk
			<< c_amp << c_page << '=' << cp
		;
	//	params
		s_cc sp = cm.getn(c_params);
		if (sp) {
			um_param(sp);
			dml << c_amp << c_params << '=' << sp.str();
		}
	//	close href
		dml << ta::ea;
		cm.traverse(&attt);
		if (link_atts) dml << ta::mal(link_atts);
		dml << "";
	}

//!	javascript start.
	void start_js(bool bc)
	{
		dml << tag::script(bc) << ta::oa(c_type) << c_text << '/' << c_javascript << ta::ea << cdef;
	}

//!	javascript expansion.
	void jscript(t_cc c)
	{
		if (c[0] && strcmp(c, c_none)) {
			dml << tag::script()
			<< ta::oa(c_type) << c_text << '/' << c_javascript << ta::ea
			<< ta::oa(c_src)  << droot << c << c_ext_js << ta::ea
			<< tte << cnl;
		}
	}

	void start_link(cmapper& cmq, cmapper* cma, bool p)
	{
		TRACE_FLOW
		dml <<  tag::mtt("a");
	//	open href, time
		dml << ta::oa(c_href) << s_lnk;
	//	complete with cgi ones
				cmxa(cmq, cgi) << c_page;
		if (p)	cmxa(cmq, cgi) << c_params;
	//	and traverse
		cmq.traverse(&vpar);
	//	close href
		dml << ta::ea;
		if (cma) cma->traverse(&attt);
	}

//!	standard link for internal content makers.
	void link(t_cc cd, cmapper& cmq, cmapper* cma, bool p)
	{
		TRACE_FLOW
		start_link(cmq, cma, p);
		dml << cd << tte;
	}

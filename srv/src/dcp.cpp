//| ============================================================
//! @file
//!
//! Dynamic content parser.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include "dcp.hpp"

#include "bbl_mapper.hpp"
#include "ccs.hpp"
#include "defs.hpp"
#include "dirman.hpp"
#include "fcp.hpp"
#include "f_io.hpp"
#include "finders.hpp"
#include "cgi.hpp"
#include "ini.hpp"
#include "ini_io.hpp"
#include "utilz.hpp"
#include "vars.hpp"
#include "xutilz.hpp"

// #undef DEVEL
// #define XDEVEL
#include "trace_macros.hpp"

//  C style file input
#include <cstdlib>
#include <cstring>

	dcp* 	pdcp = 0;

	void dcp::trcma(s_cc& sk, s_cc& sv)
	{
		if (sv && strcmp(sk, c_params) && hasllorvar(sv)) {
			cbf1.clean();
			pdcp->parse(sv, false, cbf1, true);
			cbf1.over(sv);
		}
	}

	void dcp::parsecma(cmapper& cm, dcp* p)
	{
		pdcp = p;
		cm.traverse(&trcma);
	}


//!	dcp string and length memory struct.
	struct dcpcl {
		t_cc cc;
		const unsigned ll;
	};


//!	dcp search string definitions.
	const dcpcl cls[] = {
	//	SSP 0 tag closer
		{ "</", 2 },
	//	SSP 1 variable placement
		{ "$", 1 },
	//	SSP 2 language literal LL(blaBla)
		{ "LL(", 3 },
	//	SSP 3 page link
		{ "<a", 2 },
	//	SSP 4 cproc put
		{ "PUT(", 4 },
	//	SSP 5 cproc IF()
		{ "IF(", 3 },
	//	SSP 6 cproc ENDIF()
		{ "ENDIF(", 6 },
	//	SSP 7 cproc ELSE()
		{ "ELSE(", 5 },
	//	SSP 8 cproc ELIF()
		{ "ELIF(", 5 },
	//	SSP 9 parse another file
		{ "PARSE(", 6 },
	//	SSP 10 comment opener
		{ "<!--", 4 },
	//	SSP 11 comment closer
		{ "-->", 3 },
	//	SSP 12 place another file
		{ "PLACE(", 6 },
	//	SSP 13 cporc SET()
		{ "SET(", 4 },
	//	SSP 14 form begin
		{ "<form", 5 },
	//	SPP 15 ok cproc search
		{ "ok=", 3 },
	//	SSP 16 a macro
		{ "@@", 2 },
	//	SSP 17 page another file
		{ "PAGE(", 5 }
	};

#define c_SSP_CNT 18

//!	position enumeration.
	enum {
		p_tcl = 0,
		p_var,
		p_lang,
		p_link,
		p_put,
		p_if,
		p_endif,
		p_else,
		p_elif,
		p_parse,
		p_open,
		p_close,
		p_place,
		p_set,
		p_form,
		p_ok,
		p_macro,
		p_page
	};

//!	Constructor with file directory
	dcp::dcp()
	: dfs(c_BUF_STEP_DCP_PAGES)
	, pgs(c_BUF_STEP_DCP_PAGES)
	, cma(c_BUF_STEP_ATTRIBS)
	{}

	dcp::~dcp()
	{}

//!	parse a file with r1 state set
	bool dcp::parse(void)
	{
		dml.clear();
		if (!call_page) return false;

		onerror = cgi.getn(c_onerror);

		bool ok = parsep(call_page, true);
		if (!ok && strcmp(call_page, c_start)) {
			call_page = c_start;
			ok = parsep(call_page, true);
		}
		if (bexit) {
			dml.clear();
			call_page = onerror;
			onerror = 0;
			ok = parsep(call_page, true);
		}
		if (!ok) {
			dml.clear();
			dml << msg_error << msg_file_not_found << call_page;
		}
		return ok;
	}

//!	common reader for parse and place.
	char* dcp::readp(t_cc page)
	{
	//	trial order
	//	-	layout/page-lang.htm
	//	-	layout/page.htm
	//	-	common/page-lang.htm
	//	-	common/page.htm

		sbuffer sby;
		sbuffer sba;

		sby << curr_lay << curr_lay << common << common;
		sba << curr_lang << cnull << curr_lang << cnull;

		const htmfinder* const drm = htmfinder::instance();
		t_ch cfn = 0;

		while (!cfn && sby && sba) {
			t_cc cy = sby.shift();
			t_cc ca = sba.shift();
		//	arcor/page
			cbf1.clean() << cy << c_DIR_SEP << page;
		//	arcor/page-de
			if (ca) cbf1 << c_sep << ca;
		//	search
			cfn = drm->ff(cbf1);
		}

		if (cfn) return rfile(cfn);
		return 0;
	}


//!	parse a file with r1 state set
	bool dcp::parsep(t_cc page, bool r1)
	{
		TRACE_FLOW

		TRACE_VARQ(page)
		if (r1) {
			TRACE_INF("clearing dcp")
			bexit = false;
			dfs.clean();
			pgs.clean();
			dcp_vars.clear();
		//	general write (defined by IF, ELIF, ELSE, ENDIF)
			bout = true;
		//	in comment (defined by "<--" / "-->")
			bcom = false;
		//	summary of the above
			bok  = true;
		//	if level and level states
			ifl = 0;
		//	level is true
			memset(ifs, 0, c_AHTM_IF_LEVELS * sizeof(bool));
			memset(ifd, 0, c_AHTM_IF_LEVELS * sizeof(bool));
			ifs[0] = true;
		//	clear storage of current assignment.
			bf_current.clean();
			bf_pages.clean();
		}
		else if (dfs.find(page)) return false;

		char* bff = readp(page);
		if (!bff) return false;
		TRACE_ADDR(bff)

	//	register page name
		dfs.copy(page);
		pgs.copy(page);
		curr_page = pgs.last();

		TRACE_INF("start parse ...")
		TRACE_PAGE_BEGIN(page)

		parse(read_ini_part(dcp_vars, bff), true);

		TRACE_PAGE_END(page)

		pgs.pop();
		if (!pgs) pgs.clear();
		curr_page = pgs.last();

		free(bff);
		return true;
	}

	void dcp::parse(t_ch pff, bool okcast, outstream& os, bool stc)
	{
		TRACE_FLOW
		check_vars();

		if (!pff) return;

		while (is_space(*pff)) ++pff;

		if (!*pff) return;

		input_error = false;

		bool bnl = true;
		char last = 0;

		int ips = stc ? p_var : 0;
		int ipe = stc ? p_lang + 1 : c_SSP_CNT;

	//	call arguments buffers
		ncsbuffer 	bfa1(c_BUF_STEP_BFARG);
		sbuffer 	bfa(c_BUF_STEP_BFARG);

	//	if mode (negation by "NOT" as first argument)
		bool ifmode = true;

		while (*pff) {

			TRACE_ADD(*pff)
			bool cont = true;
			bool wr   = true;

		//	blanks (do not change new line state)
			if (*pff == ' ' || *pff == '\t') { wr = !bnl; }

		//	new line
			else if (*pff == '\n') {
				if (bok && !bnl) os << cnl;
				bnl = true;
				wr = false;
			}

		//	comment
			else if (*pff == '#') {
				if (bnl && *(pff + 1) != '#') {
					while (*pff && *pff != '\n') ++pff;
					wr = false;
				}
				bnl = false;
			}
		//	windows CR
			else if (*pff == '\r') { wr = false; }

			else cont = false;

			if (cont) {
				last = *pff;
				if (bok && wr) os << *pff;
				++pff;
				continue;
			}

		//	all other: not newline
			bnl = false;

		//	check for matches
			const dcpcl* pcl = cls + ips;

		//	go through patterns
			for (int ip = ips; ip < ipe; ++ip) {
			//	string compare with given pattern length
				if (!strncmp(pff, pcl->cc, pcl->ll)) {
					TRACE_ADD(cnl);
					TRACE_INF("MATCH: " << pcl->cc)
					pff += pcl->ll;
					cont = true;

				//	switch 1: all cases of argument parsing
					switch (ip) {
						case p_lang:
						case p_put:
						case p_if:
						case p_elif:
						case p_parse:
						case p_page:
						case p_place:
						case p_set: {
							pff = splitv(pff, bfa1, bfa);
						//	check for negation by "NOT"
							switch (ip) {
								case p_if:
								case p_elif:
									ifmode = true;
									if (bfa && !strcmp(bfa[0], uc_not)) {
										bfa.shift();
										ifmode = false;
									}
								default:
								break;
							}
						}
						break;
						case p_else:
						case p_endif:
							while (*pff && *pff != ')') ++pff;
							if (*pff) ++pff;
						default:
						break;
					}

				//	content evaluation
					switch (ip) {
					//	comment opener "<!-- "
						case p_open:
							if (!is_space(*pff)) {
								pff -= pcl->ll;
								cont = false;
							}
							else {
								bcom = true;
								bok  = false;
							}
						break;
					//	comment closer " -->"
						case p_close:
							if (!is_space(last)) {
								pff -= pcl->ll;
								cont = false;
							}
							else {
								bcom = false;
								bok  = bout;
							}
						break;
					//	language place holder detected
						case p_lang:
							TRACE_INF("LL()")
							if (bok) {
								for (int i = 0; i < bfa.size(); ++i) {
									TRACE_VAR(i)
									TRACE_VARQ(bfa[i])
									if (i) os << ' ';
									t_cc c = bbl::mapper.getk(bfa[i]);
									TRACE_VARQ(c)
									frmout(c, os);
								}
							}
						break;
					//	a put instruction:
					//	- ignores bcom if bok or bout and state equals st_open
						case p_put:
							TRACE_INF("PUT()")
							if (bout) {
								input_error = false;
								fcp::put(bfa);
								if (input_error && onerror && !dfs.find(onerror)) {
									TRACE_INF("input error exit: " << onerror)
									bexit = true;
									return;
								}
							}
						break;
					//	an if instruction
						case p_if:
							TRACE_INF("IF()")
						//	- increases IF level
							++ifl;
						//	- which must not exceed maximum
							if (ifl == c_AHTM_IF_LEVELS) {
								os << cnl << msg_error << msg_case_level << msg_exceeded << cnl;
								return;
							}
						//	- if that is ok: check if lower level is ok
						//	- if so: get cproc
							else {
								TRACE_VARQ(bfa[0])
								ifs[ifl] = (ifs[ifl - 1] && fcp::ok(bfa) == ifmode);
							//	if that is good
								if(ifs[ifl]) {
									TRACE_INF("OK")
								// we are
								//	- not in comment mode
									bcom = false;
								//	- writing fine
									bout = true;
									bok  = true;
								//	- we can say: this level is done
									ifd[ifl] = true;
								}
							//	otherwise
								else {
								//	- no output regardless of comment state
									bout = false;
									bok  = false;
								//	- for whatever reason: this level is not done yet
									ifd[ifl] = false;
								}
							}
							TRACE_VAR(ifl)
							TRACE_VARB(ifs[ifl])
							TRACE_VARB(ifd[ifl])
							TRACE_VARB(bok)
						break;
					//	an elif instruction
						case p_elif:
							TRACE_INF("ELIF()")
						//	check if wrong level
							if (ifl == 0) {
								os << cnl << msg_error << msg_case_level << msg_negative << cnl;
								return;
							}
							else {
								if (ifs[ifl - 1] && !ifd[ifl] && fcp::ok(bfa) == ifmode) {
									bcom 		= false;
									bout		= true;
									bok			= true;
									ifd[ifl]	= true;
									ifs[ifl]	= true;
								}
							//	otherwise
								else {
									bout		= false;
									bok			= false;
									ifs[ifl]	= false;
								}
							}
							TRACE_VAR(ifl)
							TRACE_VARB(ifs[ifl])
							TRACE_VARB(ifd[ifl])
							TRACE_VARB(bok)
						break;
					//	an else instruction
						case p_else:
							TRACE_INF("ELSE()")
						//	check if wrong level
							if (ifl == 0) {
								os << cnl << msg_error << msg_case_level << msg_negative << cnl;
								return;
							}
							else {
								if (ifs[ifl - 1] && !ifd[ifl]) {
									bcom		= false;
									bout		= true;
									bok			= true;
									ifd[ifl]	= true;
									ifs[ifl]	= true;
								}
								else {
									bout		= false;
									bok			= false;
									ifs[ifl]	= false;
								}
							}
							TRACE_VAR(ifl)
							TRACE_VARB(ifs[ifl])
							TRACE_VARB(ifd[ifl])
							TRACE_VARB(bok)
						break;
					//	an endif instruction
						case p_endif:
							TRACE_INF("ENDIF()")
						//	check if wrong level
							if (ifl == 0) {
								os << cnl << msg_error << msg_case_level << msg_negative << cnl;
								return;
							}
							else {
								--ifl;
								bcom = false;
								bout = ifs[ifl];
								bok  = bout && !bcom;
							}
							TRACE_VAR(ifl)
							TRACE_VARB(ifs[ifl])
							TRACE_VARB(ifd[ifl])
							TRACE_VARB(bok)
						break;
					//	parse another file
						case p_page:
							TRACE_INF("PAGE()")
						case p_parse:
						{
							TRACE_INF("PARSE()")
							bool bpage = (ip == p_page);
							TRACE_VARB(bpage);
							bool cpcp = true;
							if (bout) while (bfa) {
								t_cc c = bfa.shift();
								TRACE_VARQ(c)
								if (!c[0]) continue;
								if (bpage) {
									TRACE_INF("adding call_page: " << call_page << " to current and pages")
									bf_current.add(bf_pages.copy(call_page));
//									TRACE_INF("adding " << c << " to current andd pages")
//									bf_current.add(bf_pages.copy(c));
									TRACE_INF("setting call_page " << c)
									call_page.copy(c);
									TRACE_VARQ(call_page)
								}
								TRACE_INF("parse: \"" << c << '"')
								parsep(c, false);
								if (bexit) return;
							}
						//	after parse of a file: ready to get some more variable settings
							if (okcast) {
								pff = read_ini_part(dcp_vars, pff);
								if (!bpage) check_vars();
							}
						}
						break;
					//	place another file
						case p_place:
							if (bout) while (bfa) {
								t_cc c = bfa.shift();
								TRACE_INF("place: \"" << c << '"')
								placep(c);
							}
						break;
					//	a setter
						case p_set:
							if (bout) fcp::set(bfa);
						break;
					//	a form begin "<form"
						case p_form:
							if (!(is_space(*pff) || *pff == '>')) {
								pff -= pcl->ll;
								cont = false;
							}
						//	only of interest if output ok
							else if (bok) {
								TRACE_INF("dcp form")
								cma.clear();
								pff = at2mp(pff, cma, '>');

							//	parse attributes
								parsecma(cma, this);

								if (!cma.getn(c_next)) {
									t_cc cn = fvar(c_next);
									if (cn) cma.set(c_next, cn);
								}
								form(cma);
							}
							else {
								while (*pff && *pff != '>') ++pff;
								if (*pff) ++pff;
							}
						break;
					//	a link begin
						case p_link:
							TRACE_INF("dcp link")
							if (!is_space(*pff)) {
								pff -= pcl->ll;
								cont = false;
							}
						//	if output set
							else if (bok) {
								cma.clear();
								pff = at2mp(pff, cma, '>');

							//	parse attributes
								parsecma(cma, this);

							//	the "params" attribute
								const s_cc& scp = cma.getn(c_params);
								um_param(scp);

								bool cset = cma.getn(c_class);
							//	check if "ok=..." attribute is set
								t_ch co = cma.getn(c_ok);
								if (co) {
									if (!cset) {
										if (*co == '"') {
											++co;
											splitv(co, bfa1, bfa, '"');
										}
										else {
											bfa.clean();
											bfa.add(co);
										}
										cma.set(c_class, (fcp::ok(bfa) ? c_ok : c_nok));
									}
									cma.remove(c_ok);
								}
							//	check if current class can / should be set:
							//	current mode must be on and no class defined
								else if (dcp_current && !cset) {
									bool cpg;
									t_ch pg = cma.getn(c_page);
									if (pg) cpg = !strcmp(pg, call_page);
									else {
										pg = call_page;
										cpg = true;
									}
									if (pg && (
										bf_current.find(pg) ||
										(cpg && check_params(scp))
									)) cma.set(c_class, c_current);
								}
							//	TODO: replace LLs within link / form

							//	create the link
								link(cma);
							}
						//	no output: proceed to closer
							else {
								while (*pff && *pff != '>') ++pff;
								if (*pff) ++pff;
							}
						break;
					//	a var begin "$"
						case p_var: {
							TRACE_INF("$VAR")
							t_ch p2 = pff;
							while (is_alphanum(*p2)) ++p2;
							if (bout && p2 > pff) {
								s_cc sv;
								sv.copy(pff, p2 - pff);
								TRACE_VARQ(sv)
								t_ch c = fvar(sv);
								if (c && strcmp(c, c_none)) {
									TRACE_VARQ(c);
									parse(c);
									if (bexit) return;
								}
							}
							pff = p2;
						}
						break;
					//	an "ok=" condition begin
						case p_ok:
							TRACE_INF("dcp ok= tag")
							if (!is_space(last)) {
								pff -= pcl->ll;
								cont = false;
							}
							else if (!bok) {
							//	ok=|"wumpel zumpel"|...
								if (*pff == '"') {
									++pff;
									while (*pff != '"') ++pff;
									++pff;
								}
							//	ok=|wumpel|...
								else while (is_alphanum(*pff)) ++pff;
							}
							else {
							//	ok=|"wumpel zumpel"|...
								if (*pff == '"') {
									++pff;
									pff = splitv(pff, bfa1, bfa, '"');
								}
							//	ok=|wumpel|...
								else {
									bfa.clean();
									t_ch p2 = pff;
									while (is_alphanum(*p2)) ++p2;
									if (p2 > pff) {
										s_cc sv;
										sv.copy(pff, p2 - pff);
										splitv(sv, bfa1, bfa, 0);
									}
									pff = p2;
								}
								os << c_class << '=' << (bfa && fcp::ok(bfa) ? c_ok : c_nok);
							}
						break;
					//	a tag closer
						case p_tcl:
							if (bout) os << (script_quote ? c_tclm : c_tcl);
						break;
					//	a macro
						case p_macro:
							if (!is_alpha(*pff)) {
								pff -= pcl->ll;
								cont = false;
							}
							else {
								TRACE_INF("@@ MACRO")
							//	strore macro begin
								t_ch c1 = pff - pcl->ll;
							//	proceed to first non alphanum
								while (is_alphanum(*pff)) ++pff;
							//	save vaiable name
								s_cc sf;
								sf.copy(c1, pff - c1);
								TRACE_VARQ(sf.str())
							//	tolerance: skip blanks
								while (is_blank(*pff)) ++pff;
							//	must be '(' now
								if (*pff != '(') {
									pff = c1;
									cont = false;
								}
								else {
								//	check if function is defined
									t_cc cf = fvar(sf);
								//	if not defined just proceed after ')'
									if (!cf) {
										do { ++pff; } while (*pff != ')');
										++pff;
									}
									else {
									//	proceed after '('
										++pff;
									//	split until ')'
										bfa1.clean();
										bfa.clean();
										pff = split(pff, bfa1, ')');
										for (int i = 0; i < bfa1.size(); ++i) {
											t_ch c = bfa1[i];
											if (isvar(c)) c = fvar(c + 1);
											bfa.add(c ? c : cdef);
										}
									//	call function parsing
										{
											s_cc sc;
											parsefunc(cf, bfa, sc);
											TRACE_VARQ(sc.str());
										//	parse the result
											parse(sc, okcast, os);
										}
									}
								}
							}
						break;
						default:
							pff -= pcl->ll;
							cont = false;
						break;
					}
				//	exit pattern loop
					last = 0;
					break;
				} // pattern match
				++pcl;
			} // patterns loop

			if (cont) continue;

			last = *pff;
			if (bok) os << *pff;
			++pff;
		}
	}

//!	parse a function statement.
//! replace $1 ... $9 by given sbuffer [0] ... [8].
	void dcp::parsefunc(t_ch cc, const sbuffer& sb, s_cc& sc)
	{
		if (!cc) return;
		cbuffer bf(128);
//		static cbuffer bf(128);
//		bf.clean();
		while (*cc) {
		//	$$1 .. $$9
			if (
				*cc == '$' &&
				*(cc + 1) > '0' &&
				*(cc + 1) <= '9'
			) {
				cc += 1;
				t_cc cv = sb[*cc - '1'];
				if (bc_has(&is_blank, cv)) bf << '\'' << cv << '\'';
				else bf.add(cv);
				++cc;
			}
			else {
				bf.add(*cc);
				++cc;
			}
		}
		bf.over(sc);
	}

//! just read and place a file to a given output stream.
	bool dcp::placep(t_cc page)
	{
		TRACE_FLOW
		TRACE_VARQ(page)

		char* bff = readp(page);
		if (!bff) {
			dml << msg_error << msg_file_not_found << page << '"';
            return false;
		}
		TRACE_PAGE_BEGIN(page)
		dml << bff;
		TRACE_PAGE_END(page)
		free(bff);
		return true;
	}

//!	check dcp_vars for certain keys.
	void dcp::check_vars(void)
	{
		TRACE_FLOW
		link_atts = fvar(c_link_atts);
		t_ch c = dcp_vars.getn(c_current);
		if (c) {
			ncsbuffer bfc(5);
			split(c, bfc);
			for (unsigned i = 0; i < bfc.size(); ++i) {
				c = bfc[i];
				if (c[0] && !bf_current.find(c)) bf_current.add(bf_pages.copy(c));
			}
			dcp_vars.remove(c_current);
		}
	}

//!	arguments splitter method.
	t_cc dcp::splitv(t_ch cs, csbuffer& csb, sbuffer& sb, char ct)
	{
		csb.clean();
		sb.clean();
		cs = split(cs, csb, ct);
		for (int i = 0; i < csb.size(); ++i) {
			t_ch c = csb[i];
			if (isvar(c)) c = fvar(c + 1);
			sb.add(c ? c : cdef);
		}
		return cs;
	}

//!	the instance.
	dcp parser;

//| ============================================================
//! @file
//!
//! General ini file reader class.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include "ini_io.hpp"

#include "defs.hpp"
#include "f_io.hpp"
#include "ccs.hpp"
#include "stypes.hpp"
#include "utilz.hpp"

// #undef DEVEL
// #define XDEVEL
#include "trace_macros.hpp"

//  C style file input
#include <cstdio>
#include <cstdlib>
#include <cstring>

	class cmtbf : public cm_const_traverser
	{
	public:
		cmtbf(cbuffer& b)
		: bf(b)
		{}
//	TODO adapt save ini traverser to multiline
		void operator()(const s_cc& key, const s_cc& val)
		{
			bf << key << '=' << val << '\n';
		}
	private:
		cbuffer& bf;
	};

	bool save_ini_file(const cmapper& cm, t_cc fn)
	{
		TRACE_FLOW
		cbuffer bf(c_BUF_STEP_CONFIG_CONTENT);
		cmtbf mycmt(bf);
		cm.traverse(mycmt);
		return wfile(fn, bf, bf.size());
	}

//	uncomment a string
	void uncomment(s_cc& sc)
	{
		bool nl = false;
		cbuffer bf(128);
		t_ch c = sc;
		while (*c) {
		//	skip leading spaces
			while (is_space(*c)) ++c;
		//	detect comment after that
			if (*c == '#') {
				while (*c && *c != '\n') ++c;
				continue;
			}
		//	add chars
			while (*c &&  *c != '\n') {
				if (nl) {
					bf.add('\n');
					nl = false;
				}
				if (*c != '\r') bf.add(*c);
				++c;
			}
			nl = (*c == '\n');
		}
	//	remove tailing line breaks
		bf.over(sc);
	}


//	determination of sparator
	bool is_sep(char c)
	{
		switch (c) {
			case '?':
			case '+':
			case '=':
			case '{':
			case ':':
			case '\n':
			case '\r':
				return true;
			default:
				return false;
			break;
		}
	}

	t_cc read_ini_part(cmapper& cm, t_cc bff, bool s, t_cc csec) {
		TRACE_FLOW
		if (!bff) return 0;

		t_ch p1 = bff;
		t_ch pp = p1;

		bool next = false;

	//	no section or currently in section
		bool oks = !csec;

		while (*p1) {

			if (*p1 == '\r') {
				++p1;
				continue;
			}

			if (next) {
				while (*p1 && *p1 != '\n') ++p1;
				if (*p1) ++p1;
				pp = p1;
				next = false;
				continue;
			}

			TRACE_VARQ(*p1);

			TRACE_INF("step 1")
		//	step 1: proceed to first alpha
		//	quit if comment or end
			while (is_blank(*p1)) ++p1;
			if (!*p1 ) break;
			if (*p1 == '#' || *p1 == '\n') {
				next = true;
				continue;
			}
			if (is_sep(*p1)) break;

			TRACE_INF("step 2")
		//	step 2: proceed to first appearance of ?+=:{
			t_ch p2 = p1;
			while (*p2 && !is_sep(*p2)) ++p2;

			if (!*p2 || *p2 == '\n' || *p2 == '\r') break;


		//	'?' berfore "=" ":" or "{" indicates nice mode
			bool nice =	(*p2 == '?'	);
		//	'+'  berfore "=" ":" or "{" indicates add mode
			bool badd = !nice && (*p2 == '+');

		//	if one of these procceed after following blanks
			if (nice || badd) do { ++p2; } while (is_blank(*p2));


		//	must be "=" ":" or "{" now
			if (!(*p2 == ':' || *p2 == '=' || *p2 == '{')) break;

			bool plong = (*p2 == '{');
			bool bcolo = (*p2 == ':');

		//	step 3 test: step back to non blank
			TRACE_INF("step 3")
			t_ch p3 = p2;
			s_cc sk;

			do { --p3; } while (is_blank(*p3) || *p3 == '?' || *p3 == '+');
			++p3;
			if (p3 > p1) {
				sk.copy(reinterpret_cast<t_cc>(p1), p3 - p1);
				TRACE_VARQ(sk)
			}

			TRACE_INF("step 4")
		//	step 4: zero blanks procceed to next non blank
		//	if curly mode: also ignore leading blanks and newlines.
			if (plong)	do { ++p2; } while (is_space(*p2));
			else 		do { ++p2; } while (is_blank(*p2));

		//	empty value
			if (*p2 == '\n' && !plong) {
				++p2;
				p1 = p2;
				continue;
			}
			if (!*p2) {
				pp = p2;
				break;
			}

		//	step 5: value end treatment
			p3 = p2;
			if (plong)	do { ++p3; } while (*p3 && *p3 != '}');
			else 		do { ++p3; } while (*p3 && *p3 != '\n');

			char c3 = *p3;

			if (sk) {
			//	to say the trouth: no adding or nice without sort mode
				if ((nice || badd) && !s) cm.sort();

				t_cc xv = (nice || badd ? cm.getn(sk).str() : cnull);

				if (!(nice && xv)) {

				//	step back to last non blank
					t_ch p4 = p3;
					do { --p4; } while (is_blank(*p4) || *p4 == '\n' || *p4 == '\r');
					++p4;

				//	save state if value is not empty
					if (*p4 && p4 > p2) {

						s_cc sv;
						sv.copy(p2, p4 - p2);
						if (plong) uncomment(sv);
						TRACE_VARQ(sv)

					//	add to existing value?
						if (badd && xv) {
							if (oks) {
								cbuffer cbf(c_BUF_STEP_BBL);
								cbf.add(xv);
								cbf.add(plong ? '\n' : ' ');
								cbf.add(sv.str());
								TRACE_VARQ(cbf.str())
								cm.copy(sk, cbf, s);
							}
						}
					//	or simple set / section begin / end
						else {
//							s_cc sv;
//							sv.copy(p2, p4 - p2);
//							TRACE_VARQ(sv)
						//	section begin / end
							bool bb;
							if (bcolo && (
								(bb = !strcmp(sk, uc_begin)) ||
								(!strcmp(sk, uc_end))
							)) {
								if (csec && !strcmp(sv, csec)) {
									if (bb) oks = true;
									else return 0;
								}
							}
							else if (oks) cm.set(sk, sv, s);
						}
					}
				}
			}

//			sk = 0;
//			nice = false;
//			badd = false;

			p1 = pp = p3;
			if (!c3) break;
			++p1;
			++pp;
		}
		TRACE_VAR(*pp)
		return pp;
	}

	bool load_ini_file(cmapper& cm, t_cc fn, bool s, t_cc csec)
	{
		TRACE_FLOW

		TRACE_INF(msg_loading << fn)

		char* bff = rfile(fn);

		if (!bff) {
            TRACE_ERR(msg_file_not_found << fn << '"');
            return false;
        }
		read_ini_part(cm, bff, s, csec);

		free(bff);

		return true;
	}

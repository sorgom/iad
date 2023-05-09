//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include "xutilz.hpp"

#include "utilz.hpp"

#undef DEVEL
#include "trace_macros.hpp"

//!	a helper function that could be placed somewhere else
	t_cc at2mp(t_cc c, cmapper& mp, char ct)
	{
		if (!c) return c;
		TRACE_FLOW
		t_ch c1 = c;
		while (*c1 == ' ' || *c1 == '\t') { ++c1; }
		t_ch c2 = c1;
		while (*c2 && *c2 != ct) {
			TRACE_VARQ(c2)
		//	nothing given: ignore blanks of c1
		//	= and no key given
			if (*c2 == '=') {
				s_cc sk;
				sk.copy(c1, c2 - c1);
				++c2;
				c1 = c2;
			//	if attribute starts with " we take anything including " terminator
				if (*c2 == '"') {
					do { ++c2; } while (*c2 && *c2 != '"');
					if (*c2 != '"') break;
					++c2;
				}
			//	not quoted: proceed until blank or terminator
				else {
					do { ++c2; } while (*c2 && *c2 != ' ' &&  *c2 != '\t' && *c2 != ct && *c2 != '\n' &&  *c2 != '\r' );
				}
				if (sk && c2 > c1) {
					s_cc sv;
					sv.copy(c1, c2 - c1);
					mp.set(sk, sv);
				}
				while (*c2 == ' ' || *c2 == '\t' || *c2 == '\n' || *c2 == '\r') { ++c2; }
				c1 = c2;
			}
		//	build up key
			else {
				++c2;
				while (*c2 && *c2 != ct && *c2 != '=' && !is_alphanum(*reinterpret_cast<t_uch>(c2))) {
					++c2;
					c1 = c2;
				}
			}
		}
		if (*c2) ++c2;
		return c2;
	}



/*	DEACTIVATED

	cbuffer bfats(c_BUF_STEP_ATTRIBS_STR);

	void travatss(const s_cc& k, const s_cc& v)
	{
		bfats << ' ' << k.str() << '=' << v.str();
	}

	t_cc mp2at(const cmapper& mp)
	{
		bfats.clean();
		mp.traverse(&travatss);
		return bfats.str();
	}
*/

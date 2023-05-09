//| ============================================================
//! @file
//!
//! Unclassified utilities collection.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include "utilz.hpp"

//#undef DEVEL
#include "trace_macros.hpp"

#include <cstring>
#include <cstdio>

//! determination of numerical chracters [0-9]
    bool is_numb(char c)
	{
		return c >= '0' && c <= '9';
	}

//! determination of alphabetical chracters [_A-Za-z]
    bool is_alpha(char c)
	{
		return
			(c >= 'a' && c <= 'z') ||
			(c >= 'A' && c <= 'Z') ||
			c == '_'
		;
	}

//! determination of alphanumerical chracters [_A-Za-z0-9]
    bool is_alphanum(char c)
    {
		return is_alpha(c) || is_numb(c);
    }

//!	alphanumerical and blank
	bool is_alphanum_blank(char c)
	{
		return is_alphanum(c) || is_blank(c);
	}

//! determination of space
	bool is_space(char c)
	{
		return
			is_blank(c) ||
			c == '\n' ||
			c == '\r'
		;
	}
//! determination of blank
	bool is_blank(char c)
	{
		return c == ' ' || c == '\t';
	}

//! determination of hex [0-9a-fA-F]
	bool is_hex(char c)
	{
		return hex_base(c);
	}

//!	apply boolean character function to a complete string.
	bool bc_all(bc_func* pbf, t_ch cc)
	{
		for ( ; *cc; ++cc) if (!(*pbf)(*cc)) return false;
		return true;
	}

//!	@return true if at least one character matches.
	bool bc_has(bc_func* pbf, t_ch cc)
	{
		for ( ; *cc; ++cc) if ((*pbf)(*cc)) return true;
		return false;
	}


//!	reduce all blanks of a character string to single blanks.
	char* space(char* c, bool bs)
	{
		if (!c) return 0;
		while (*c == ' ' || *c == '\t') ++c;
		char* c1 = c;
		char* c2 = c;
		bool bsp = true;
	//	starts with non blank
		while (*c1) {
			do {
			//	c2 is at least one ahead
				++c2;
				bool ok = true;
				char c = *c2;
				if (*c2 == ' ' || *c2 == '\t') {
					if (bsp || bs) ok = false;
					else {
						c = ' ';
						bsp = true;
					}
				}
				else bsp = false;
				if (ok) {
				//	c1 can be same as c2 but doesn't metter
					++c1;
					*c1 = c;
				}
			} while(*c2);
		}
	//	back track: c2 is 0 terminator here
	//	but one blank might be left
		if (!bs) {
			--c1;
			if (*c1 == ' ' || *c1 == '\t') *c1 = 0;
		}
		return c;
	}



//!	get hex base of a hexadecimal character.
	char hex_base(char c)
	{
		return
			(c >= '0' && c <= '9') ? '0' :
			(c >= 'A' && c <= 'F') ? 'A' - 10 :
			(c >= 'a' && c <= 'f') ? 'a' - 10 :
			0
		;
	}

//!	transfer one hexadecimal character to value.
	unsigned char unhex1(char c)
	{
		char hb = hex_base(c);
		return hb ? c - hb : 0;
	}

//!	transfer hexadecimal (2 digit) string to value.
	unsigned char unhex2(t_ch c)
	{
		TRACE_FLOW
		if (!c) return 0;
		unsigned char uc = 0;
		for (unsigned char ci = 2; *c && ci--;) {
			char hb = hex_base(*c);
			if (!hb) break;
			TRACE_VARQ(*c)
			TRACE_VARQ(hb)
			uc *= 16;
			uc += *c - hb;
			++c;
		}
		TRACE_VAR((unsigned) uc)
		return uc;
	}

//!	transfer a query string to readable text.
	void unqs(char* c1)
	{
		char* c2 = c1;
		while (true) {
			if (*c2 == '+') *c1 = ' ';
			else if (*c2 == '%') {
				*c1 = unhex2(c2 + 1);
				for (int i = 2; i--;) {
					++c2;
					if (!*c2) {
						*(c1 + 1) = 0;
						return;
					}
				}
			}
			else *c1 = *c2;
			if (!*c2) return;
			++c2;
			++c1;
		}
	}



//!	find word in string.
//!	returns false if string contains more alpanumerical stuff after word.
	inline bool issep(char c)
	{
		return c == ' ' || c == '\t' || c == ',';
	}

	bool lfind(t_ch c, t_cc w, char ct)
	{
		unsigned lw = strlen(w);
		TRACE_FLOW
		if (!c || !w) return false;
		char cq;
		while (*c && *c != ct) {
			while (issep(*c)) ++c;
			if (!*c || *c == ct) break;
			if (*c == '"' || *c == '\'') {
				cq = *c;
				++c;
			}
			else cq = 0;
			if (*c && !strncmp(c, w, lw)) {
				char cw = *(c + lw);
				if (
					(!cw || cw == ct) ||
					(cq && cw == cq) ||
					(!cq && issep(cw))
				)	return true;
			}
			if (cq) while (*c && *c != cq && *c != ct) ++c;
			else do { ++c; } while (*c && !issep(*c) && *c != ct);
		}
		return false;
	}

	char sc2[3];

//! 2 digit string of unsigned (e.g. for time settings).
	t_fcc u22d(unsigned i)
	{
		if (i > 99) return cdef;
		sprintf(sc2, "%02d", i);
		return sc2;
	}
//!	2 digit hex string of unsigned char (e.g. for mac addresses)
	t_fcc uc22h(unsigned char uc)
	{
		sprintf(sc2, "%02X", uc);
		return sc2;
	}

//!	file extension of a string (including dot ".").
	t_fcc ext(t_cc cc)
	{
		TRACE_FLOW
		if (!cc) return 0;
		t_ch c = cc;
		while (*c) ++c;
		do {
			--c;
			if (*c == '.') return c;
		} while (c != cc);
		return 0;
	}

//!	overwrite a const char* const.
	void setcc(t_cc& cc, t_cc cn)
	{
		memcpy(const_cast<t_ch*>(&cc), &cn, sizeof(t_cc));
	}

//	============================================================
//	ctryer & ctry
	ctryer& ctryer::operator()(t_ch& rc)
	{
		rc = 0;
		pc = &rc;
		return *this;
	}

	ctryer& ctryer::operator<<(t_cc cc)
	{
		if (pc && cc && !*pc) *pc = cc;
		return *this;
	}

//! stream style try to set string value.
//!	e.g. ctry(c) << cc1 << cc2 << cc3;
//!	c becomes first non null value.
	ctryer& ctry(t_ch& rc)
	{
		static ctryer _ctry;
		_ctry(rc);
		return _ctry;
	}

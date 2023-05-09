//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include "val.hpp"

#include "ccs.hpp"
#include "defs.hpp"
#include "sfunc.hpp"
#include "outstream.hpp"
#include "vars.hpp"
#include "utilz.hpp"

//	#undef DEVEL
#include "trace_macros.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace val
{
//!	general validator.
	bool in(const s_cc& sc, int p)
	{
		switch (p) {
			case none	:
			case text	: return in_text(sc);
			case num 	: return in_num (sc);
			case unum 	: return in_unum(sc);
			case real	: return in_real(sc);
			case ip4	: return in_ip4 (sc);
			case mac	: return in_mac (sc);
			case host	: return in_host(sc);
			default		: return false;
		}
	}

	t_fcc out_real_str(t_cc);
	t_fcc out_real_str(double);

//!	general output formatter stream class.
	class sf_out : public sfunc
	{
	public:
		inline sf_out(t_cc c, int i)
		: cc(c)
		, p(i)
		{}
		t_ch cc;
		int p;
		void operator()(outstream& os) const
		{
			if (!cc) return;
			switch (p) {
				case real	: os.add(out_real_str(cc)); break;
				case num 	:
				case ip4	:
				case mac	:
				case host	:
					os.add(cc);
				break;
				default		: frmout(cc, os); break;
				break;
			}
		}
		bool data(void) const { return cc; }
	};

//!	general output formatter stream class delivery function.
	sfunc* out(t_cc cc, int p)
	{
		return new sf_out(cc, p);
	}

//!	special output formatter real from string.
	sfunc* out_real(t_cc cc)
	{
		return new sf_str(out_real_str(cc));
	}
//!	special output formatter real from real.
	sfunc* out_real(double x)
	{
		return new sf_str(out_real_str(x));
	}

//!	general difference detector.
	bool dif(t_cc c1, t_cc c2, int p)
	{
		switch (p) {
			case num	:
			case unum 	: return dif_num(c1, c2);
			case real	: return dif_real(c1, c2);
			default		: return dif_text(c1, c2);
		}
	}

//	============================================================
//	text
//	TODO: validator for text.
	bool in_text(const s_cc& sc)
	{
		char* c = space(sc.ms());
		if (c) sc.mr().copy(c);
		return true;
	}

//!	diff func for text.
	bool dif_text(t_cc c1, t_cc c2)
	{
		return !c1 ? c2 != 0 : !c2 ? c1 != 0 : strcmp(c1, c2) != 0;
	}

//	============================================================
//	numerical

	bool is_num(t_cc cc, bool gez, bool gtz)
	{
		if (!cc) return false;
		t_ch uc = cc;
		if (gtz) gez = true;
		bool beg = true;
		bool dgd = false;
		bool bgz = false;

		while (char u = *uc) {
			if (is_blank(u)) {}
			else if (is_numb(u)) {
				dgd = true;
				if (u > '0') bgz = true;
			}
			else if (beg && !gez && u == '-') {}
			else return false;
			beg = false;
			++uc;
		}
		return dgd && (gtz ? bgz : true);
	}

	bool in_num(const s_cc& sc)
	{
		TRACE_FLOW
		return is_num(sc) && shrink(sc.ms());
	}
	bool in_unum(const s_cc& sc)
	{
		TRACE_FLOW
		return is_num(sc, true) && shrink(sc.ms());
	}

//!	diff func for numerical values.
	bool dif_num(t_cc c1, t_cc c2)
	{
		return !c1 ? c2 != 0 : !c2 ? c1 != 0 : atol(c1) != atol(c2);
	}

//	============================================================
//	real
	bool is_real(t_cc cc, bool tol)
	{
		t_ch uc = cc;
		bool beg = true;
		bool fpd = false;
		bool dgd = false;
		while (char u = *uc) {
			if (is_blank(u)) {}
			else if (is_numb(u)) dgd = true;
			else if (!fpd && u == '.') fpd = true;
			else if (tol && !fpd && (u == ',' || u == real_fpoint)) fpd = true;
			else if (beg && u == '-') {}
			else return false;
			beg = false;
			++uc;
		}
		return dgd;
	}

//	formatted string of real
	char* real_str(double x)
	{
		TRACE_FLOW
		fbuffer& 	fb = rfb(c_SIZE_SPRINTF_DOUBLE);
		char*		cr = fb.ms();
 	//	number of digits given
		if (real_digits >= 0 && real_digits < 10) {
			char cf[5] = "%.2f";
			cf[2] = '0' + real_digits;
			sprintf(cr, cf, x);
		}
 	//	free number of digits
		else sprintf(cr, "%f", x);
		return cr;
	}


	bool in_real(const s_cc& sc)
	{
		TRACE_FLOW
		if (!is_real(sc, true)) return false;
		char* c = shrink(sc.ms());
		if (!c) return false;
		char* uc = c;
		while (*uc) {
			if (*uc == ',' || *uc == real_fpoint) *uc = '.';
			++uc;
		}
		sc.mr().copy(c);
		return true;
	}

	t_fcc out_real_str(t_cc cc)
	{
		TRACE_FLOW
		if (!(cc && cc[0])) return cdef;
		double x = atof(cc);
		return out_real_str(x);
	}

	t_fcc out_real_str(double x)
	{
		TRACE_FLOW
		char* cx = real_str(x);
		if (real_fpoint != '.') {
			char* c = cx;
			for ( ;*c ; ++c) {
				if (*c == '.') {
					*c = real_fpoint;
					break;
				}
			}
		}
		return cx;
	}

//!	diff func for real values.
	bool dif_real(t_cc c1, t_cc c2)
	{
		return !c1 ? c2 != 0 : !c2 ? c1 != 0 : atof(c1) != atof(c2);
	}


//	============================================================
//	bytes
	bool s2b(const s_cc& sc, t_bytes& b, char cs)
	{
		TRACE_FLOW
		if (!sc) return false;
		t_ch 		c = sc;
		unsigned 	cn = 0;
		bool 		ok = true;
		fbuffer& 	fb = rfb(b.sz * 4);
		while (*c && ok) {
		//	proceed to next numerical
			while (*c && (*c < '0' || *c > '9')) ++c;
			if (!*c) break;
			unsigned ic = 0;
			if (cn) fb.add(cs);
		//	evaluate numerical characters
			if (cn < b.sz) while (*c >= '0' && *c <= '9') {
				ic *= 10;
				ic += *c - '0';
				if (ic <= 255) fb.add(*c);
				else ok = false;
				++c;
			}
			else ok = false;
			if (ok) {
				b.bs[cn] = static_cast<unsigned char>(ic);
				++cn;
			}
		}
		ok = ok && cn == b.sz;
		if (ok) sc.mr().copy(fb.str());
		return ok;
	}
	bool hs2b(const s_cc& sc, t_bytes& b, char cs)
	{
		TRACE_FLOW
		if (!sc) return false;
		TRACE_VAR(b.sz)
		t_ch 		c = sc;
		unsigned 	cn = 0;
		bool 		ok = true;
		fbuffer& 	fb = rfb(b.sz * 3);
		while (*c && ok) {
		//	proceed to next hexadecimal
			while (*c && !hex_base(*c)) ++c;
			if (!*c) break;
		//	evaluate hex characters
			unsigned char uc = 0;
			for (unsigned char ci = 2; *c && ci--;) {
				char hb = hex_base(*c);
				if (!hb) break;
				uc *= 16;
				uc += *c - hb;
				++c;
			}
			TRACE_VAR((unsigned) uc)
		//	if next character is also a hex this can't be right
			if (hex_base(*c) || cn == b.sz) ok = false;
			else {
				if (cn) fb.add(cs);
				fb.add(uc22h(uc));
				b.bs[cn] = uc;
				++cn;
			}
		}
		ok = ok && cn == b.sz;
		if (ok) sc.mr().copy(fb.str());
		return ok;
	}

//	============================================================
//	IP and host
	bool in_ip4(const s_cc& sc)
	{
		static t_ip4 ip;
		return in_ip4(sc, ip);
	}

	bool in_ip4(const s_cc& sc, t_ip4& ip)
	{
		return s2b(sc, ip);
	}

	bool in_host(const s_cc& sc)
	{
		TRACE_FLOW
		if (in_ip4(sc)) return true;
		bool ba  = false;
		bool bd  = false;
		bool bdl = false;
		t_ch uc = sc.str();
		for (; *uc; ++uc) {
			if (is_blank(*uc)) {}
			else if (is_alpha(*uc)) {
				ba  = true;
				bdl = false;
			}
			else if (*uc == '.') {
				if (!ba) return false;
				bdl = bd = true;
			}
			else return false;
		}
		if (bdl || !(ba && bd)) return false;
		return shrink(sc.ms());
	}
//	============================================================
//	MAC addresses (6 Bytes Hex)
	bool in_mac(const s_cc& sc)
	{
		TRACE_FLOW
		static t_mac mc;
		return in_mac(sc, mc);
	}
	bool in_mac(const s_cc& sc, t_mac& mc)
	{
		TRACE_FLOW
		return hs2b(sc, mc);
	}

} // namespace val

//| ============================================================
//! @file
//!
//! Input validation, formatted output to dml
//!	and diff decicions.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef VAL_HPP
#define VAL_HPP

#include "sfunc.hpp"
#include "stypes.hpp"
#include "xtypes.hpp"

namespace val
{
	enum {
		none,
		text,
		num,
		unum,
		real,
		ip4,
		mac,
		host
	};

//!	general validator.
	bool in(const s_cc&, int);
//!	general output formatter.
	sfunc* out(t_cc, int);
//!	special output formatter real from string.
	sfunc* out_real(t_cc);
//!	special output formatter real from real.
//!	usefull if APIs deliver floating point values.
	sfunc* out_real(double);
//!	general difference detector.
//!	requires correctly formatted strings to work other than val::none.
	bool dif(t_cc, t_cc, int);

//	============================================================
//	text
//!	TODO: validator for text.
	bool in_text(const s_cc&);
//!	diff func for text.
	bool dif_text(t_cc, t_cc);

//	============================================================
//	numerical (integer / unsigned)

//!	check whether a string is numerical.
//!	@param c	the string
//!	@param gez	must be greater equal zero
//!	@param gtz	must be greater zero
	bool is_num(t_cc c, bool gez = false, bool gtz = false);
//!	validator for numerical values.
	bool in_num(const s_cc&);
//!	validator for unsigned numerical values.
	bool in_unum(const s_cc&);

//!	diff func for numerical values.
	bool dif_num(t_cc, t_cc);

//	============================================================
//	real (float / double)

//!	check whether a string is real.
//!	@param tol allow comma and assigned fpoint sign as dot.
	bool is_real(t_cc, bool tol = false);
//!	validator for real (float, double) values.
	bool in_real(const s_cc&);
//!	diff func for real values.
	bool dif_real(t_cc, t_cc);

//	============================================================
//	IP addresses and host names

//! validator for ip v4 address values.
	bool in_ip4 (const s_cc&);
//! validator for ip v4 address values with value transfer.
	bool in_ip4 (const s_cc&, t_ip4&);
//!	validator for host names (not yet extensively implemented).
//! just a simple check if characters separated by dot
//!	or ip4 address respectively.
	bool in_host(const s_cc&);

//	============================================================
//	MAC addresses (6 Bytes Hex).
	bool in_mac(const s_cc&);
//	MAC addresses (6 Bytes Hex) with value transfer.
	bool in_mac(const s_cc&, t_mac&);

//	============================================================
//	conversion subs

//!	general conversion from sparated numbers string to byte container.
	bool s2b(const s_cc&, t_bytes&, char = '.');

//!	general conversion from sparated hex numbers string to byte container.
	bool hs2b(const s_cc&, t_bytes&, char = ':');

} // namespace val

#endif // VAL_HPP

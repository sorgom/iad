//| ============================================================
//! @file
//!
//! macros for md4 encoding algorithm
//!	according to RFC 1320.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef MD4_MACROS_HPP
#define MD4_MACROS_HPP

//	Constants for MD4Transform routine.
	#undef S11
	#undef S12
	#undef S13
	#undef S14
	#undef S21
	#undef S22
	#undef S23
	#undef S24
	#undef S31
	#undef S32
	#undef S33
	#undef S34

	#define S11 3
	#define S12 7
	#define S13 11
	#define S14 19
	#define S21 3
	#define S22 5
	#define S23 9
	#define S24 13
	#define S31 3
	#define S32 9
	#define S33 11
	#define S34 15

//	F, G and H are basic MD4 functions.

	#undef F
	#undef G
	#undef H
	#undef ROTATE_LEFT
	#undef FF
	#undef GG
	#undef HH


	#define F(x, y, z) ((x & y) | ((~x) & z))
	#define G(x, y, z) ((x & y) | (x & z) | (y & z))
	#define H(x, y, z) (x ^ y ^ z)

//	ROTATE_LEFT rotates x left n bits.
	#define ROTATE_LEFT(x, n) ((x << (n)) | (x >> (32-(n))))

//	FF, GG and HH are transformations for rounds 1, 2 and 3
//	Rotation is separate from addition to prevent recomputation

	#define FF(a, b, c, d, x, s) { a += F (b, c, d) + x; a = ROTATE_LEFT (a, s); }
	#define GG(a, b, c, d, x, s) { a += G (b, c, d) + x + static_cast<t_u4>(0x5a827999); a = ROTATE_LEFT (a, s); }
	#define HH(a, b, c, d, x, s) { a += H (b, c, d) + x + static_cast<t_u4>(0x6ed9eba1); a = ROTATE_LEFT (a, s); }


#endif // MD4_MACROS_HPP

//| ============================================================
//! @file
//!
//! MD4 encoding.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================


//	Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991.
//	All rights reserved.
//
//	License to copy and use this software is granted provided that it
//	is identified as the "RSA Data Security, Inc. MD4 Message-Digest
//	Algorithm" in all material mentioning or referencing this software
//	or this function.
//
//	License is also granted to make and use derivative works provided
//	that such works are identified as "derived from the RSA Data
//	Security, Inc. MD4 Message-Digest Algorithm" in all material
//	mentioning or referencing the derived work.
//
//	RSA Data Security, Inc. makes no representations concerning either
//	the merchantability of this software or the suitability of this
//	software for any particular purpose. It is provided "as is"
//	without express or implied warranty of any kind.
//
//	These notices must be retained in any copies of any part of this
//	documentation and/or software.


#ifndef MD4_HPP
#define MD4_HPP

//	library md4

namespace md4
{
//!	MD4 encoding of unsigned char array of given length.
//!	@param u	MD4 return
//!	@param uc	source bytes to be encoded
//!	@param l	length of source bytes
	void encode(unsigned char u[16], const unsigned char* uc, unsigned l);

//!	convenience: MD4 encoding of C character string of given length.
//!	@param u	MD4 return
//!	@param c	source character string
//!	@param l	length of source character string
	inline void encode(unsigned char u[16], const char* c, unsigned l)
	{
		encode(u, reinterpret_cast<const unsigned char*>(c), l);
	}
} // namespace md4
#endif // MD4_HPP

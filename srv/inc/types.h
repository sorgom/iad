//| ============================================================
//! @file
//!
//! Standard definiton of frequently used types and defaults.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================


//  Please do not namespace this file.

#ifndef TYPES_H
#define TYPES_H

//	============================================================
//	character (string) types

//! Constant character const pointer type.
    typedef const char* const t_cc;

//! Constant character const pointer type returned by functions or methods:
//!	only valid until next function or method call.
    typedef const char* const t_fcc;

//!	a mutable but not redirectable chracter pointer.
	typedef char* const t_mc;

//! const character for default returns: empty.
    extern t_cc cdef;

//! const character for default returns: null.
    extern t_cc cnull;

//!	Constant character pointer by value type.
	typedef const char* t_ch;
	typedef const unsigned char* t_uch;

//!	an unsigned chraracter pointer type.
	typedef unsigned char t_uc;
	typedef const unsigned char* const t_ucc;

//!	numerical representation of a chraracter.
	typedef unsigned int t_cn;

//!	general number representation.
	typedef unsigned int t_num;

//!	2 byte unsigned int.
	typedef unsigned short t_u2;

//!	4 byte unsigned int.
	typedef unsigned t_u4;

//	============================================================
//	byte defined numerical types


#endif // _H

//| ============================================================
//! @file
//!
//! a convenience wrapper for zlib deflate.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef DEFLATE_H
#define DEFLATE_H
#include "stypes.h"

#include <cstdio>

//!	deflate from file to file.
//!	@param src open read source file descriptor
//!	@param trg open write target file descriptor
//!	@param sz  bytes to read from source file (file size)
//!	@return number of bytes in trg / 0 on error
	unsigned deflate(FILE* src, FILE* trg, unsigned sz = 0);

//!	deflate from string buffer to file.
//!	@param buf string buffer
//!	@param sz  buffer contents' size
//!	@param trg open write target file descriptor
//!	@return number of bytes in trg / 0 on error
	unsigned deflate(const char* buf, unsigned sz, FILE* trg);

//!	deflate string to string buffer.
//!	@param buf string buffer
//!	@param sz  buffer contents' size
//!	@param bf  target cbuffer
//!	@return number of bytes in bf / 0 on error
	unsigned deflate(const char* buf, unsigned sz, cbuffer& bf);


#endif // DEFLATE_H

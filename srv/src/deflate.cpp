//|	============================================================
//!	@file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//|	============================================================

#include "deflate.hpp"
#include "f_io.hpp"

#include "trace_macros.hpp"

#include <cstdio>
#include <cstdlib>
#include <zlib.h>

#define c_CHUNK_SIZE 20480

//	============================================================
//	deflate

//!	deflate from file to file.
//!	@param src open read source file descriptor
//!	@param trg open write target file descriptor
//!	@param lvl compression level
//!	@return number of bytes in trg
	unsigned deflate(FILE *src, FILE *trg, unsigned sz)
	{
		TRACE_FLOW
	//	check file descriptors
		if (!(src && trg)) return 0;

	//	allocate deflate state
		z_stream strm;
		strm.zalloc = Z_NULL;
	    strm.zfree  = Z_NULL;
		strm.opaque = Z_NULL;
		if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) return 0;

	//	determination of bytes to read
		if (!sz) sz = frsize(src);
		TRACE_VAR(sz)

	//	determination of buffer size
		unsigned bsz = sz > c_CHUNK_SIZE ? c_CHUNK_SIZE : sz;
		TRACE_VAR(bsz)

	//	create buffers
		unsigned char* out	= new unsigned char[bsz];
		unsigned char* in	= new unsigned char[bsz];

	//	done bytes count
		unsigned nb = 0;

		bool ok = true;
		unsigned td;
	//	standard deflate loop
	//	done when last data in file processed
		while (ok && sz) {
		//	how much to do this round?
			td = sz > bsz ? bsz : sz;
			unsigned char* cp = in;
			ok = ffread(cp, td, src);
			if (ok) {
				sz -= td;
				int flush = sz ? Z_NO_FLUSH : Z_FINISH;
				TRACE_VAR(flush)
				strm.avail_in = td;
				strm.next_in  = in;

			//	run deflate() on input until output buffer not full, finish
			//	compression if all of src has been read in
				do {
					strm.avail_out = bsz;
					strm.next_out  = out;
					ok = deflate(&strm, flush) != Z_STREAM_ERROR;
					if (ok) {
					//	write to target
						td = bsz - strm.avail_out;
						const unsigned char* cpc = out;
						ok = ffwrite(cpc, td, trg);
						nb += td;
					}
				}	while (ok && strm.avail_out == 0);
			}
		}

		delete in;
		delete out;

	//	clean up and return
		deflateEnd(&strm);
		return ok ? nb : 0;
	}

//!	deflate from string buffer to file.
	unsigned deflate(const char* buf, unsigned sz, FILE *trg)
	{
	//	check file descriptors
		if (!(buf && sz && trg)) return 0;

	//	allocate deflate state
		z_stream strm;
		strm.zalloc = Z_NULL;
	    strm.zfree  = Z_NULL;
		strm.opaque = Z_NULL;
		if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) return 0;

	//	create out buffer
		unsigned char* out = new unsigned char[c_CHUNK_SIZE];

	//	deflated bytes count
		unsigned nb = 0;

	//	no file read buffering: assign complete string
		strm.avail_in = sz;
		strm.next_in  = reinterpret_cast<unsigned char*>(const_cast<char*>(buf));

	//	standard deflate loop
	//	run deflate() on input until output buffer not full, finish
	//	compression if all of src has been read in
	//	this loop is passed multipelly if out is bigger than in
		bool ok = true;
		do {
			strm.avail_out = c_CHUNK_SIZE;
			strm.next_out  = out;
			ok = deflate(&strm, Z_FINISH) != Z_STREAM_ERROR;
			if (ok) {
				unsigned have = c_CHUNK_SIZE - strm.avail_out;
			//	write to target
				const unsigned char* cpc = out;
				ok = ffwrite(cpc, have, trg);
				nb += have;
			}
		}	while (ok && strm.avail_out == 0);

		delete out;

	//	clean up and return
		deflateEnd(&strm);
		return ok ? nb : 0;
	}

//!	deflate string to string buffer.
//!	@param buf string buffer
//!	@param sz  buffer contents' size
//!	@param trg open write target file descriptor
//!	@return number of bytes in trg / 0 on error
	unsigned deflate(const char* buf, unsigned sz, cbuffer& bf)
	{
	//	check file descriptors
		if (!(buf && sz)) return 0;

	//	allocate deflate state
		z_stream strm;
		strm.zalloc = Z_NULL;
	    strm.zfree  = Z_NULL;
		strm.opaque = Z_NULL;
		if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) return 0;

	//	clean buffer
		bf.clean();

	//	create out buffer
		unsigned char* out = new unsigned char[c_CHUNK_SIZE];

	//	no file read buffering: assign complete string
		strm.avail_in = sz;
		strm.next_in  = reinterpret_cast<unsigned char*>(const_cast<char*>(buf));

	//	standard deflate loop
	//	run deflate() on input until output buffer not full, finish
	//	compression if all of src has been read in
	//	this loop is passed multipelly if out is bigger than in
		bool ok = true;
		do {
			strm.avail_out = c_CHUNK_SIZE;
			strm.next_out  = out;
			ok = deflate(&strm, Z_FINISH) != Z_STREAM_ERROR;
			if (ok) {
				unsigned have = c_CHUNK_SIZE - strm.avail_out;
			//	write to target
				bf.add(reinterpret_cast<char*>(out), have);
			}
		}	while (ok && strm.avail_out == 0);

		delete out;

	//	clean up and return
		deflateEnd(&strm);
		return ok ? bf.size() : 0;
	}

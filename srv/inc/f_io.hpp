//| ============================================================
//! @file
//!
//!	C style file IO and directory access.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ===========================================================


#ifndef F_IO_HPP
#define F_IO_HPP

#include "types.hpp"
#include <ctime>
#include <cstdio>

#define c_MIN_IO_BSIZE 64

//!	calculation of maximum block size for file io.
	unsigned bsize(unsigned);

//!	determination of size of a read open file
//!	including rewind to file position 0.
	unsigned frsize(FILE* fp);

//!	read from file with maximum block size.
	bool ffread(char*& cp, unsigned td, FILE* fp);

//!	ffread variation for unsigned char.
	inline bool ffread(unsigned char*& cp, unsigned td, FILE* fp)
	{ return ffread(reinterpret_cast<char*&>(cp), td, fp); }

//!	write to file with maximum block size.
	bool ffwrite(const char*& cp, unsigned td, FILE* fp);

//!	ffwrite variation for unsigned char.
	inline bool ffwrite(const unsigned char*& cp, unsigned td, FILE* fp)
	{ return ffwrite(reinterpret_cast<const char*&>(cp), td, fp); }

//!	read file into definetly 0 terminated buffer with size information option.
//!	return unsigned char* must be freed after usage.
//! #include <cstdlib>
//!	free(bf);
//!	(delete bf; works to)
//!	@param fn file name
//!	@param sr size return pointer (excluding zero terminator)
//!	@param ct add zero terminator (default)
	char* rfile(t_cc fn, unsigned* sr = 0, bool zt = true);

//!	write string to file to file.
//!	@param fn file name
//!	@param c  content to be written
//!	@param l  length of content (excluding zero terminator, optional)
	bool wfile(t_cc fn, t_cc c, unsigned l = 0);

//! directoy listing processor class.
	class dircaller
	{
	public:
	//!	method processing element (name).
		virtual void operator()(t_cc) = 0;
	};

//!	type indicating list of directories.
	extern const int tp_dir;

//!	type indicating list of regular files.
	extern const int tp_file;

//!	directoy listing processing function.
	typedef void (dirfunc)(t_cc);

//!	general directoy listing with #dircaller class pointer
//!	or #dirfunc function pointer.
	void lsdir(t_cc d, int tp, dircaller* pdc, dirfunc* pdf);

//!	directoy listing with #dircaller class reference.
//!	@param d directory to be listed
//!	@param tp type specifier: what should be listed
//!	@param dc #dircaller reference
	inline void lsdir(t_cc d, int tp, dircaller& dc) { lsdir(d, tp, &dc, 0); }

//!	directoy listing with #dirfunc function pointer.
//!	@param d directory to be listed
//!	@param tp type specifier: what should be listed
//!	@param pdf #dirfunc function pointer
	inline void lsdir(t_cc d, int tp, dirfunc* pdf)  { lsdir(d, tp, 0, pdf); }

//!	type and existence check directory / file.
//!	@param tp  desired type (tp_dir or tp_file)
//!	@param cfd directory or file name
	bool istp(int tp, t_cc cfd);

//!	convenience: directory check.
	inline bool isdir(t_cc cd) { return istp(tp_dir, cd); }

//!	convenience: file check.
	inline bool isfile(t_cc cf) { return istp(tp_file, cf); }

//!	file modification time.
//!	@param fp file name including path
//	time_t mtm(t_cc fp);


#endif // _H

//| ============================================================
//! @file
//!
//!	C style file IO.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include "f_io.hpp"
#include "defs.hpp"
#include "stypes.hpp"


// #undef DEVEL
#include "trace_macros.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

//!	calculation of maximum block size for file io.
	unsigned bsize(unsigned sz)
	{
		TRACE_FLOW
		unsigned bs = 1;
		while (sz /= 2) bs *= 2;
		TRACE_VAR(bs)
		return bs;
	}

//!	determination of size of a read open file
//!	including rewind to file position 0.
	unsigned frsize(FILE* fp)
	{
		TRACE_FLOW
	//	obtain file size: go to file
		fseek(fp, 0, SEEK_END);
	//	... and and retrieve position
		unsigned sz = ftell(fp);
	//	go to file start position
		rewind(fp);
		TRACE_VAR(sz)
		return sz;
	}

//!	read from file with maximum block size.
	bool ffread(char*& cp, unsigned td, FILE* fp)
	{
		TRACE_FLOW
		unsigned nb;
		unsigned bs = bsize(td);
		unsigned bd;
		while (td) {
			TRACE_VAR(td)
			if (td < c_MIN_IO_BSIZE) {
				bd = nb = td;
				bs = 1;
			}
			else {
				while (bs > td) bs /= 2;
				bd = bs;
				nb = 1;
			}
			TRACE_VAR(bs)
			TRACE_VAR(nb)
			if (fread(cp, bs, nb, fp) != nb) break;
			td -= bd;
			cp += bd;
		};
		return !td;
//		return fread(cp, 1, td, fp) == td;
	}

//!	write to file with maximum block size.
	bool ffwrite(const char*& cp, unsigned td, FILE* fp)
	{
		TRACE_FLOW
		unsigned nb;
		unsigned bs = bsize(td);
		unsigned bd;
		while (td) {
			TRACE_VAR(td)
			if (td < c_MIN_IO_BSIZE) {
				bd = nb = td;
				bs = 1;
			}
			else {
				while (bs > td) bs /= 2;
				bd = bs;
				nb = 1;
			}
			TRACE_VAR(bs)
			TRACE_VAR(nb)
			if (fwrite(cp, bs, nb, fp) != nb) break;
			td -= bd;
			cp += bd;
		};
		return !td;
	}

	char* rfile(t_cc fn, unsigned* sr, bool zt)
	{
		TRACE_FLOW
		if (sr) *sr = 0;

		FILE* fp;
		fp = fopen(fn, "rb");
	  	if (!fp) return 0;

	//	obtain file size
	//	go to file start position
		unsigned sz = frsize(fp);

		TRACE_VAR(sz)

		bool ok = (sz > 0);

	//	allocate memory to contain the whole file (+ zero terminator)
		char* buff = 0;
		ok = ok && (buff = reinterpret_cast<char*>(malloc(sz + (zt ? 1 : 0))));

		if (ok) {
		//	string position
			char* cp = buff;
		//	apply fast read
			ok = ffread(cp, sz, fp);
		}
	//	close anyhow
		fclose (fp);

	//	good: add zero terminator
		if (ok) { if (zt) buff[sz] = 0; }
	//	bad result: free memory
		else if (buff) {
			free(buff);
			buff = 0;
		}

	//	size pointer given
		if (sr) *sr = ok ? sz : 0;

		return buff;
	}
//!	write string to file to file.
	bool wfile(t_cc fn, t_cc c, unsigned sz)
	{
		TRACE_FLOW
		if (!c) return false;
		if (!sz) sz = strlen(c);
		if (!sz) return false;
		FILE* fp = fopen(fn, "w");
		if (!fp) return false;
	//	string position
		t_ch cp = c;
	//	apply fast write
		bool ok = ffwrite(cp, sz, fp);
  		fclose(fp);
		return ok;
	}

//	commonly used
	struct stat st;

//!	directoy listing #dircaller or #dirfunc
	void lsdir(t_cc d, int tp, dircaller* pdc, dirfunc* pdf)
	{
		TRACE_FLOW
		TRACE_VAR(tp)
		if (!pdc && !pdf) return;
		DIR* hdir = opendir(d);
		if (hdir) {
			cbuffer bf(c_BUF_STEP_FILE_PATH);
    		struct dirent* e;
			while (e = readdir(hdir)) {
				bf << d << c_DIR_SEP << e->d_name;
				if (istp(tp, bf)) {
					TRACE_VARQ(e->d_name);
					if (pdc)	(*pdc)(e->d_name);
					else		(*pdf)(e->d_name);
				}
				bf.clean();
			}
    		closedir(hdir);
		}
	}

//!	type and existence check directory / file.
//!	@param cfd directory or file name
//!	@param tp  desired type (tp_dir or tp_file)
	bool istp(int tp, t_cc cfd)
	{
		return
			stat(cfd, &st) != -1 &&
			(st.st_mode & S_IFMT) == tp
		;
	}

	const int tp_dir  = S_IFDIR;
	const int tp_file = S_IFREG;

/*
//!	file modification time.
	time_t mtm(t_cc fp)
	{
		TRACE_FLOW
		if (!fp) return 0;
		struct stat st;
		return stat(fp, &st) == -1 ? 0 : st.st_mtime;
	}
*/

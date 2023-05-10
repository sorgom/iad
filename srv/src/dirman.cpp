//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include "dirman.hpp"

#include "defs.hpp"

//#define XDEVEL
#include "trace_macros.hpp"

#include <cstdio>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

//	============================================================
//	dirman
	dirman::dirman()
	: cdd(0)
	{}

//!	convenience; find + path
	t_fcc dirman::ff(t_cc cf) const
	{
		if (!cdd) return 0;
		t_cc f = find(cf);
		if (f) {
			static cbuffer bf(c_BUF_STEP_FILE_PATH);
			bf.clean() << cdd << c_DIR_SEP << f;
			if (cxt && bcx) bf << cxt;
			return bf;
		}
		return 0;
	}

//!	scan subdirectories and files.
	bool dirman::scan(t_cc cd, t_cc xt, bool cx)
	{
		TRACE_FLOW
		clear();
		if (!cd) return false;
		cdd = cd;
		cxt = xt;
		bcx = cx;
		lxt = xt ? strlen(xt) : 0;
		scn(0);
		sort();
		return true;
	}

//!	internal scan sub.
	bool dirman::scn(t_cc cs)
	{
		TRACE_FLOW
		TRACE_VARQ(cs)
	//	assemble subdirectory
		cbuffer bfd(c_BUF_STEP_FILE_PATH);
		cbuffer bft(c_BUF_STEP_FILE_PATH);
		bfd << cdd << c_DIR_SEP;
		if (cs) {
			bft << cs << c_DIR_SEP;
			bfd << cs << c_DIR_SEP;
		}
		unsigned sd = bfd.size();
		unsigned st = bft.size();
		TRACE_VAR(sd)
		TRACE_VAR(st)
	//	temporary directory buffer;
		csbuffer bfdt;

		DIR* hdir = opendir(bfd);
		if (!hdir) return false;
    	struct dirent* e;
		while (e = readdir(hdir)) {
			t_cc enm = e->d_name;
			if (enm[0] == '.') continue;
			bfd.resize(sd) << enm;
			struct stat stt;
			if (stat(bfd, &stt) == -1) continue;
			int tp = stt.st_mode & S_IFMT;
			bft.resize(st) << enm;
			TRACE_VARQ(bft.str())
		//	file
			if (tp == S_IFREG) {
			//	extension check
				if (lxt) {
					unsigned le = strlen(enm);
					if (le <= lxt || strcmp(enm + (le - lxt), cxt)) continue;
				}
				if (bcx) copy(bft, bft.size() - lxt);
				else copy(bft);
			}
		//	directory
			else if (tp == S_IFDIR) {
				bfdt.copy(bft);
			}
		}
    	closedir(hdir);
	//	scan subdirectories
		for (unsigned i = 0; i < bfdt.size(); ++i) scn(bfdt[i]);
		return true;
	}

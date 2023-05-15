//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include <dflmgr.h>

#include <ccs.h>
#include <deflate.h>
#include <defs.h>
#include <f_io.h>
#include <http.h>
#include <ini_io.h>
#include <utilz.h>
#include <vars.h>

// #define XDEVEL
#include <trace_macros.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <cstdlib>

#define c_DFLMGR_BUF_STEP 32

//	============================================================
//	helpers

	template <class T>
	void ccset(const T& tt, const T& ts)
	{
		memcpy(const_cast<T*>(&tt), &ts, sizeof(T));
	}

//	============================================================
//	dflmgr

//!	name of temporary deflate file (unless dflkeep).
	t_cc dflmgr::c_dfltmp = "tmp.dfl";
//!	name of manager.
	t_cc dflmgr::c_dflmrg = "dflmrg";

//!	constructor with init call.
//!	initialization (must be done after load_ini() call):
	dflmgr::dflmgr()
	: dflxts	(ini.getn(c_deflate))
	, dfldir	(ini.getn(c_dfldir))
	, dflkeep	(ini.getn(c_dflkeep))
	, cmrg		(c_DFLMGR_BUF_STEP)
	, bfl		(c_DFLMGR_BUF_STEP)
	, dflok		(false)
	{
		TRACE_FLOW
		TRACE_VARQ(dfldir)
		TRACE_VARQ(dflxts)
		TRACE_VARB(dflkeep)
	//	reset work copy
		memset(&dff, 0, sizeof(dflfile));
	//	check if directories are valid
		bool ok;
		ok = dflxts && dfldir && isdir(dfldir);
		ccset(dflok, ok);
		TRACE_VARB(dflok);
		ok = dflkeep && dflok && dfldir && isdir(dfldir);
		ccset(dflkeep, ok);
		TRACE_VARB(dflkeep)
	//	forther treatment if all ok
		if (dflok) {
		//	data file
			s_mgrdat.copy(tmp_file(c_dflmrg));
			TRACE_VARQ(s_mgrdat.str());

		//	if defkeep is set compare existing and listed files
			if (dflkeep) {

			//	load register of file assignments
				load_ini_file(cmrg, s_mgrdat);
			//	get all values of cmrg
				csbuffer bft(c_DFLMGR_BUF_STEP);
				cmrg.values(bft);

			//	list all existing deflated files.
				lsdir(dfldir, tp_file, *this);

			//	compare results: remove unlisted files
				for (unsigned i = 0; i < bfl.size(); ++i) {
					t_cc cfl = bfl[i];
					if (!bft.find(cfl)) {
						TRACE_INF(c_remove << ' ' << cfl)
						remove(dir_file(dfldir, cfl));
					}
				}
				bfl.clear();
			}
		}
	}

//!	directory traverse callback (dircaller).
	void dflmgr::operator()(t_cc cd)
	{
		if (bc_all(&is_hex, cd)) {
			TRACE_VARQ(cd)
			bfl.copy(cd);
		}
	}

//!	the application call.
//!	@param fn file path relative to file search directory
//!	@param df do deflate setting
//!	@param cxt file extension (if already extracted)
	const dflfile& dflmgr::dfile(t_ch fn, bool df, t_cc cxt, t_cc etg)
	{
		TRACE_FLOW
		TRACE_VARQ(fn)
		memset(&dff, 0, sizeof(dflfile));
		bool done = false;
		bool chg  = false;
	//	remove leading '/'
		if (fn[0] == '/') ++fn;
	//	directory && file path, file etag
		s_cc fpf;
		cbf1.clean() << htmdir << c_DIR_SEP << fn;
		fpf.copy(cbf1);
	//	file size and last modification
		unsigned fsz = 0;
		time_t   flm = 0;
		struct stat st;
		if (stat(fpf, &st) != -1) {
			flm = st.st_mtime;
			fsz = static_cast<unsigned>(st.st_size);
		}
	//	create file etag
		if (fsz) {
			cbf1 << fsz << static_cast<unsigned>(flm);
			fet.copy(etag(cbf1, cbf1.size()));
			TRACE_VARQ(fet.str())
		//	if modification request etag given and match:
		//	just set nomod flag and return
			if (etg && !strcmp(etg, fet)) {
				TRACE_INF(c_not_modified)
				dff.nomod	= true;
				dff.etg		= fet;
				return dff;
			}
		}
	//	decision whether deflate is indicated
		df = df && dflok && lfind(dflxts, cxt);
		TRACE_VARB(df)

	//	deflate attempts
		if (df) {
			dff.cec = c_deflate;
			s_cc dpf;
			unsigned dsz = 0;
			if (fsz) {
				cbf1.clean();
				if (dflkeep) 	cbf1 << dfldir << c_DIR_SEP << fet;
				else 			cbf1 << tmpdir << c_DIR_SEP << c_dfltmp;
				cbf1.over(dpf);
			//	deflated file size
				if (dflkeep && stat(dpf, &st) != -1)
					dsz = static_cast<unsigned>(st.st_size);
			}

		//	get stored version
			t_ch det = dflkeep ? cmrg.getn(fn) : 0;
			TRACE_VARQ(fet.str())
			TRACE_VARQ(det)

		//	if stored etag version exists and differs
			if (det && (!fet || strcmp(det, fet))) {
				cbf1.clean() << dfldir << c_DIR_SEP << det;
			//	if file or its deflated version exists:
			//	remove stored version and corresponding file
				if (fsz || dsz) {
					TRACE_INF(c_remove << ' ' << fn << ' ' << cbf1.str())
					remove(cbf1);
					cmrg.remove(fn);
					det = 0;
					chg = true;
				}
			//	if none of them exist try to get stored file
			//	this might be the case if file deflated and then removed
				else if (stat(cbf1, &st) != -1) {
					TRACE_INF("old registered deflat version")
					dff.sz  = static_cast<unsigned>(st.st_size);
					dff.tlm = st.st_mtime;
					dff.etg = det;
					dff.fp  = fopen(cbf1, "rb");
					done    = dff.fp;
				}
			}

		//	if deflated file exists take that anyhow
			if (!done && dflkeep && dsz) {
				TRACE_INF("stored deflat version: " << fet)
				dff.sz  = dsz;
				dff.tlm = flm;
				dff.etg = fet;
				dff.fp  = fopen(dpf, "rb");
				done 	= dff.fp;
			}
		//	otherwise deflate source file
			if (!done && fsz) {
				TRACE_INF("deflating " << fpf << " >> " << dpf)
				FILE* src = 0;
				FILE* trg = 0;
				if (
					(src = fopen(fpf, "rb")) &&
					(trg = fopen(dpf, "w+b")) &&
					(dsz = deflate(src, trg)) &&
					(fseek(trg, 0 , SEEK_SET) == 0)
				) {
					dff.sz  = dsz;
					dff.tlm = flm;
					dff.etg = fet;
					dff.fp  = trg;
					done = true;
				}
				if (src) fclose(src);
				if (!done && trg) fclose(trg);
			}
		//	register current deflate file
			if (dflkeep && done && !det) {
				TRACE_INF("store " << fn << ' ' << fet)
				cmrg.copy(fn, fet);
				chg = true;
			}
		} // deflate attempt
	//	open without deflate
		if (!done && fsz) {
			TRACE_INF("no deflate: " << fpf)
			dff.sz  = fsz;
			dff.tlm = flm;
			dff.etg = fet;
			dff.cec = 0;
			dff.fp  = fopen(fpf, "rb");
		}

	//	write changes to registry
		if (chg) save_ini_file(cmrg, s_mgrdat);

		return dff;
	}

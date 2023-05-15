//| ============================================================
//! @file
//!
//! file deflate manager.
//!	main tasks:
//!	create and provide deflated versions of files,
//!	keep deflated versions,
//!	track and remove outdated versions.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef DFLMGR_H
#define DFLMGR_H

#include "cmapper.h"
#include "f_io.h"
#include <cstdio>

//!	deflate file handout to calling application.
	struct dflfile
	{
	//!	no modification.
		bool nomod;
	//!	content length.
		unsigned sz;
	//!	last modified time.
		time_t tlm;
	//!	etag string.
		t_ch etg;
	//!	encoding
		t_ch cec;
	//!	open to read file descriptor.
		FILE* fp;
	};

	class dflmgr : public dircaller
	{
	public:
	//!	constructor with init call.
	//!	initialization (must be done after load_ini() call):
	//!		read register file,
	//!		decide modes,
	//!		clear outdated versions.
		dflmgr();
	//!	directory traverse callback (dircaller).
		void operator()(t_cc);
	//!	the application call.
	//!	@param fn file path relative to file search directory
	//!	@param df do deflate setting
	//!	@param cxt file extension
	//!	@param etg etag from no modification request
		const dflfile& dfile(t_cc fn, bool df, t_cc cxt, t_cc etg = 0);
	//!	general deflate ok.
		const bool dflok;
	private:
	//!	list of extensions to apply deflate with (ini).
		t_cc dflxts;
	//!	deflate directory (ini).
		t_cc dfldir;
	//!	deflate keep (ini).
		const bool dflkeep;
	//!	runtime register.
		cmapper cmrg;
	//!	name of manager.
		static t_cc c_dflmrg;
	//!	name of temporary deflate file (unless dflkeep).
		static t_cc c_dfltmp;
	//!	dflfile work copy.
		dflfile dff;
	//!	dflate files listing buffer.
		csbuffer bfl;
	//!	ini file path.
		s_cc s_mgrdat;
	//!	file based etag.
		s_cc fet;
	};


#endif // DFLMGR_H

//| ============================================================
//! @file
//!
//! directory scan and file find manager.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef DIRMAN_H
#define DIRMAN_H

#include "stypes.h"

	class dirman : public ncsbuffer
	{
	public:
		dirman();
	//!	scan subdirectories and files.
	//!	@param cd directory to scan
	//!	@param xt file extension (if not given: any file)
	//!	@param cx cut extension in stored files list
		bool scan(t_cc cd, t_cc xt = 0, bool cx = false);
	//!	convenience: find + path.
		t_fcc ff(t_cc) const;

	private:
	//!	directory.
		t_ch cdd;
	//!	extension.
		t_ch cxt;
	//!	cut extensions (saves string compare time).
		bool bcx;
	//!	length of extension.
		unsigned lxt;
	//!	internal scan sub.
		bool scn(t_cc cs);
	};

#endif // DIRMAN_H

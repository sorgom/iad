//| ============================================================
//! @file
//!
//! General ini file reader class.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef INI_IO_H
#define INI_IO_H

#include "cmapper.h"

	t_cc read_ini_part(cmapper& cm, t_cc bff, bool s = true, t_cc = 0);

	bool load_ini_file(cmapper& cm, t_cc fn, bool s = true, t_cc = 0);

	bool save_ini_file(const cmapper& cm, t_cc fn);

	void uncomment(s_cc& sc);
#endif // _H

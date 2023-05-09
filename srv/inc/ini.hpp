//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef INI_HPP
#define INI_HPP

#include "types.hpp"

//	convenience inclusion
//	the ini container is accessible by component vars
#include "vars.hpp"

//	convenience inclusion
//	the common stup files are
//	- c_iadcgi_ini	(cgi version)
//	- c_iadsrv_ini	(srv version)
//	both accessible by component ccs
#include "ccs.hpp"

//!	load required ini data.
//	@param fn file name
	bool load_ini(t_cc fn);



#endif // INI_HPP

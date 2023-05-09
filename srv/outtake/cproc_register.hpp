//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef CPROC_REGISTER_HPP
#define CPROC_REGISTER_HPP

#include "cproc.hpp"
#include "types.hpp"

//!	find function.
	cproc* findcp(t_cc k);

//!	reset function.
	void resetcps(void);

//!	cproc count info.
	unsigned cpcount(void);

#endif // CPROC_REGISTER_HPP

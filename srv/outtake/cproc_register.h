//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef CPROC_REGISTER_H
#define CPROC_REGISTER_H

#include "cproc.h"
#include "types.h"

//!	find function.
	cproc* findcp(t_cc k);

//!	reset function.
	void resetcps();

//!	cproc count info.
	unsigned cpcount();

#endif // CPROC_REGISTER_H

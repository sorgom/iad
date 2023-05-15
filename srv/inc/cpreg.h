//| ============================================================
//! @file
//!
//! script generated cproc registry.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef CPREG_H
#define CPREG_H

#include "cproc.h"

//!	find function.
	cproc* findcp(t_cc k);

//!	reset function.
//!	calls individual reset method for all instanciated cprocs.
	void resetcps();

//!	remove all cproc instances from memory.
	void removecps();

//!	cproc count info.
	unsigned cpcount();

#endif // CPREG_H

//| ============================================================
//! @file
//!
//! script generated cproc registry.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef CPREG_HPP
#define CPREG_HPP

#include "cproc.hpp"

//!	find function.
	cproc* findcp(t_cc k);

//!	reset function.
//!	calls individual reset method for all instanciated cprocs.
	void resetcps(void);

//!	remove all cproc instances from memory.
	void removecps(void);

//!	cproc count info.
	unsigned cpcount(void);

#endif // CPREG_HPP

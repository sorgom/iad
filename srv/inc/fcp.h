//| ============================================================
//! @file
//!
//! find and lauch cproc functions.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef FCP_H
#define FCP_H

#include "stypes.h"

namespace fcp
{
//!	find and process: ok.
	bool ok(t_cc);
//!	find and process: ok bulk.
	bool ok(sbuffer&);
//!	find and process: put.
	void put(sbuffer&);
//!	find and process: set.
	void set(sbuffer&);
//! reset all instanciated cprocs.
	void reset();
//! remove all instanciated cprocs.
	void remove();
} // namespace fcp

#endif // FCP_H

//| ============================================================
//! @file
//!
//! find and lauch cproc functions.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef FCP_HPP
#define FCP_HPP

#include "stypes.hpp"

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
	void reset(void);
//! remove all instanciated cprocs.
	void remove(void);
} // namespace fcp

#endif // FCP_HPP

//| ============================================================
//! @file
//!
//! #cproc retrieval and assignment server interface.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef FCPSRV_H
#define FCPSRV_H

#include "stypes.h"

//!	Content processor keyword assignment registry.
	class fcpsrv : public dsb
	{
	public:
	//!	find and process: ok.
		bool ok(t_cc) const;
	//!	find and process: ok bulk.
		bool ok(sbuffer&) const;
	//!	find and process: put.
		void put(sbuffer&) const;
	//!	find and process: set.
		void set(sbuffer&) const;
	//! reset all instanciated cprocs.
		void reset() const;
	//! remove all instanciated cprocs.
		void remove() const;
	};

	extern const fcpsrv& fcp;

#endif // FCPSRV_H

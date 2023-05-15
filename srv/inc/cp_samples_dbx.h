//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef CP_SAMPLES_DBX_H
#define CP_SAMPLES_DBX_H

#include "cmapper.h"
#include "cproc.h"

//	a #cproc derivate providing cached information
//	from boolean values collective database table "SVB".
	class cp_SVB : public db_cproc
	{
	public:
		cp_SVB();
		bool ok(sbuffer&);
		void put(sbuffer&);
		inline void reset() { cm.clear(); }
	private:
	//	internal ok
		bool ok(t_cc);
		cmapper cm;
	};


#endif // CP_SAMPLES_DBX_H

//| ============================================================
//! @file
//!
//! #cproc sample: languages image link list generator.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef CP_SAMPLE_ILANGUAGE_H
#define CP_SAMPLE_ILANGUAGE_H

#include "cproc.h"
#include "cmapper.h"
#include "stypes.h"

//!	languages image link list generator #cproc.
	class cp_ilanguages : public cproc, public cm_const_traverser
	{
	public:
		cp_ilanguages();
		void operator()(const s_cc& k, const s_cc& v);
		void put(sbuffer&);
	private:
		cmapper cmp;
		cmapper cma;
	};



#endif // CP_SAMPLE_ILANGUAGE_H

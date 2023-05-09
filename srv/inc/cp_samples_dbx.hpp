//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef CP_SAMPLES_DBX_HPP
#define CP_SAMPLES_DBX_HPP

#include "cmapper.hpp"
#include "cproc.hpp"

//	a #cproc derivate providing cached information
//	from boolean values collective database table "SVB".
	class cp_SVB : public db_cproc
	{
	public:
		cp_SVB();
		bool ok(sbuffer&);
		void put(sbuffer&);
		inline void reset(void) { cm.clear(); }
	private:
	//	internal ok
		bool ok(t_cc);
		cmapper cm;
	};


#endif // CP_SAMPLES_DBX_HPP

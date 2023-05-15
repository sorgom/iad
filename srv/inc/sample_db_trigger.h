//| ============================================================
//! @file
//!
//! a sample database update trigger function.
//!	adds database update to infos.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef SAMPLE_DB_TRIGGER_H
#define SAMPLE_DB_TRIGGER_H

#include "types.h"
#include "db.h"

//!	a sample database update trigger: adds database update to infos.
	class info_trigger : public db::update_trigger
	{
	public:
//		inline ~info_trigger() {}
	//!	trigger call.
		void trigger(const sbuffer&);
	};

#endif // SAMPLE_DB_TRIGGER_H

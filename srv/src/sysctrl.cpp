//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include "sysctrl.hpp"


namespace ctrl
{
	sysctrl* psctrl = 0;

	void set(sysctrl* p) { psctrl = p; }

	void process(int i)
	{
		if (psctrl) psctrl->process(i);
	}

} // namespace ctrl

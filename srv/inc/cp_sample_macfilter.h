//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef CP_SAMPLE_MACFILTER_H
#define CP_SAMPLE_MACFILTER_H

#include "cproc.h"


//!	WLAN mac address filter display and setter.
	class cp_macfilter : public db_cproc, private cp_once
    {
    public:
	//!	LITERALS: 'Trusted Nodes', Activate, Deactivate, Edit, Remove, Clear, Add
	//!	OPTIONS: reversed
	//!	- reversed: displays mac addresses, tooltips: names
	//!	- standard: displays names, tooltips: mac addresses
		void put(sbuffer&);

	//!	true if mac filtering is active (independent of WLAN or number of trusted nodes)
	//!	checked once per page call
		inline bool ok(sbuffer& sb) { return check_once(sb); }

		cp_macfilter();
		inline void reset() { reset_once(); }
	protected:
		bool my_check(sbuffer&);
	private:
		t_cc tbl;
	};
#endif // CP_SAMPLE_MACFILTER_H

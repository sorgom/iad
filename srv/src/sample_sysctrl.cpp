//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include "sample_sysctrl.hpp"

#include "ini.hpp"
#include "sysctrl.hpp"

// #define XDEVEL
#include "trace_macros.hpp"

#include <cstdlib>

//	============================================================
//	simplectrl

	void simplectrl::process(int nr)
	{
		TRACE_FLOW
		TRACE_VAR(nr)
		switch (nr) {
//			case ctrl::error    :
//			case ctrl::ok       :
//			case ctrl::hello    :
//			case ctrl::stats    :
//			case ctrl::restart  :
//			case ctrl::shutdown :
			case ctrl::sysreset :
			{
				t_cc csr = ini.getn(c_sysreset);
				TRACE_VARQ(csr)
				if (csr) {
					int ret = system(csr);
					TRACE_VAR(ret)
				}
			}
			break;
			default:
			break;
		}
	}

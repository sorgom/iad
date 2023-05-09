//| ============================================================
//! @file
//!
//! A sample #cproc that fakes online state values
//!	for ajax update demonstration.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef CP_SAMPLE_ONLINE_HPP
#define CP_SAMPLE_ONLINE_HPP

#include "cproc.hpp"

	class cp_fake_online_state : public db_cproc
	{
	public:
		void put(sbuffer&);
	};

#endif // CP_SAMPLE_ONLINE_HPP

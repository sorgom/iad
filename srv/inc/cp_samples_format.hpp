//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef CP_SAMPLES_FORMAT_HPP
#define CP_SAMPLES_FORMAT_HPP

#include "cproc.hpp"

//!	sample formatter #cproc.
//!	call parameter: format key
	class cp_sample_format : public db_cproc
    {
    public:
		cp_sample_format();
		void put(sbuffer&);
		bool ok(sbuffer&);
	private:
		char tbl[7];
    };


#endif // _H

//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef SAMPLE_SYSCTRL_H
#define SAMPLE_SYSCTRL_H

#include "sysctrl.h"

//!	a simple sys controller,
//!	calling value of ini key "sysreset"
//!	with ctrl number ctrl::sysrest.
	class simplectrl : public ctrl::sysctrl
	{
	public:
		void process(int);
	};

#endif // SAMPLE_SYSCTRL_H

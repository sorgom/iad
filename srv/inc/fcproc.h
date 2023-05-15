//| ============================================================
//! @file
//!
//! cproc finders for CGI and standalone SRV version.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef FCPROC_H
#define FCPROC_H

#ifdef ISSRV
	#include "fcpsrv.hpp"
#else
	#include "fcpcgi.hpp"
#endif

#endif // FCPROC_H

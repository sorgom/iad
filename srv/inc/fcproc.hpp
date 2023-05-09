//| ============================================================
//! @file
//!
//! cproc finders for CGI and standalone SRV version.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef FCPROC_HPP
#define FCPROC_HPP

#ifdef ISSRV
	#include "fcpsrv.hpp"
#else
	#include "fcpcgi.hpp"
#endif

#endif // FCPROC_HPP

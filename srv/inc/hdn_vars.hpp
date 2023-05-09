//| ============================================================
//! @file
//!
//! hidden vars repository.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef HDN_VARS_HPP
#define HDN_VARS_HPP

#include "cmapper.hpp"

namespace hdn
{

	extern bool			poly;
	extern bool			srv;

	extern unsigned		sctrl;

	extern cmapper		ini;
	extern cmapper		cgi;
	extern sbuffer		form_vars;
	extern ncsbuffer	dbnames;
	extern csbuffer		rtvs;
	extern s_cc			s_rtvs;
	extern s_cc			s_lnk;

	extern cmapper		layouts;
	extern cmapper		langs;

} // namespace hdn

#endif // HDN_VARS_HPP

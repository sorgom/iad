//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include <hdn_vars.h>

#include <defs.h>

namespace hdn
{
	bool		poly		= false;
	bool		srv			= false;

	unsigned	sctrl		= 0;

	cmapper		ini			;
	cmapper		cgi			(c_BUF_STEP_HTML_IN);
	sbuffer		form_vars	(10);
	ncsbuffer	dbnames		(c_BUF_STEP_DBNAMES);
	csbuffer	rtvs		(8);
	s_cc		s_rtvs		= 0;
	s_cc		s_lnk		= cdef;

	cmapper		layouts		(8);
	cmapper		langs		(8);


} // namespace hdn

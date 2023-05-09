//| ============================================================
//! @file
//!
//! extended utilities collection.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef XUTILZ_HPP
#define XUTILZ_HPP

#include "cmapper.hpp"

//!	split the attributes part of a html tag into a key / value map.
//!	(does not clear the map)
//! @param c  string to be examinated
//!	@param mp cmapper to be filled
//!	@param ct terminator character e.g. '>'
//!	@return pointer after end of examination i.e. after terminater character / 0
	t_cc at2mp(t_cc c, cmapper& mp, char ct = 0);

/*	DEACTIVATED
	t_cc mp2at(const cmapper& mp);
*/

#endif // XUTILZ_HPP

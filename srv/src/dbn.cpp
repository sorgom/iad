//| ============================================================
//! @file
//!
//! Hard coded database table and field names.
//!	This file would be
//!	- once created by script
//!	- maintained in a half script mode
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include "dbn.hpp"
#include <cstring>

namespace dbn
{
//	standard values
	t_cc Id		= "Id";

//	commonly used field names
	t_cc Active = "Active";
	t_cc Down	= "Down";
	t_cc Fixed	= "Fixed";
	t_cc Mac	= "Mac";
	t_cc Name	= "Name";
	t_cc Num	= "Num";
	t_cc Ok		= "Ok";
	t_cc Rate	= "Rate";
	t_cc Up		= "Up";
	t_cc Val	= "Val";

//	special key words
	t_cc S_prefix		= "S_";
	t_cc S_FRM	= "S_FRM";
	t_cc Col			= "Col";
	t_cc Frm			= "Frm";
	t_cc S_NRS	= "S_NRS";

//	format key words
	t_cc NUM	= "NUM";
	t_cc UNUM	= "UNUM";
	t_cc REAL	= "REAL";
	t_cc MAC	= "MAC";
	t_cc IP4	= "IP4";
	t_cc HOST	= "HOST";
	t_cc TEXT	= "TEXT";
	t_cc BOOL	= "BOOL";
	t_cc NOED	= "NOED";

//	user.db
//	- general single value int storage
	t_cc SVI			= "SVI";
//	- general single value bool storage.
	t_cc SVB			= "SVB";
//	- time servers and manual time
	t_cc TimeSrv		= "TimeSrv";
	t_cc TimeDiff		= "TimeDiff";
//	- format sample tables
	t_cc Sp				= "Sp";
//	- mac filter sample
	t_cc MacFlt			= "MacFlt";
//	runtime.db
	t_cc OnlineState	= "OnlineState";

//	special vars
	const unsigned S_length = strlen(S_prefix);

} // namespace dbn

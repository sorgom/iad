//| ============================================================
//! @file
//!
//! Hard coded database table and field names.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef DBN_H
#define DBN_H

#include "types.h"

namespace dbn
{
//	standard values
	extern t_cc Id;

//	commonly used field names
	extern t_cc Active;
	extern t_cc Down;
	extern t_cc Fixed;
	extern t_cc Mac;
	extern t_cc Name;
	extern t_cc Num;
	extern t_cc Ok;
	extern t_cc Rate;
	extern t_cc Up;
	extern t_cc Val;

//	special key words
//!	prefix of not editable tables "S_".
	extern t_cc S_prefix;
//! name of table listing table / column formats.
	extern t_cc S_FRM;
	extern t_cc Col;
	extern t_cc Frm;

//!	name of table listing not resizeable tables.
	extern t_cc S_NRS;

//	format key words
	extern t_cc NUM;
	extern t_cc UNUM;
	extern t_cc REAL;
	extern t_cc MAC;
	extern t_cc IP4;
	extern t_cc HOST;
	extern t_cc TEXT;
	extern t_cc BOOL;
	extern t_cc NOED;

//	user.db
//	- general single value int storage.
	extern t_cc SVI;
//	- general single value bool storage.
	extern t_cc SVB;
//	- time servers and manual time (field)
	extern t_cc TimeSrv;
	extern t_cc TimeDiff;

//	- format sample tables
	extern t_cc Sp;
//	- mac filter sample
	extern t_cc MacFlt;

//	runtime.db
//	- ajax updater sample
	extern t_cc OnlineState;

//	special vars
//!	length of S_prefix (for strncmp).
	extern const unsigned S_length;
} // namespace dbn

#endif // DBN_H

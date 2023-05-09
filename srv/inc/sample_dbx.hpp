//| ============================================================
//! @file
//!
//! Customized database handling extensions
//!	for a given database setup.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef SAMPLE_DBX_HPP
#define SAMPLE_DBX_HPP

#include "db.hpp"

//!	get a single int value by name.
	int get_svi(db::proc&, t_cc);

//!	set a single int value by name with / without trigger.
	void set_svi(db::proc&, t_cc, int, bool = false, bool = true);

//!	collect column name - format information for a table.
	void init_frm(db::proc&, t_cc);

//! retrieve a format key (string) of a column name.
	t_cc col2key(t_cc);

//!	retrieve a format number of a format key.
	int key2frm(t_cc);

//!	retrieve format number of a column name.
//! (convenience combination of #col2key and #key2frm.
	inline int col2frm(t_cc cc) { return key2frm(col2key(cc)); }

//! determination of boolean columns by format key.
	bool key2bool(t_cc);

//!	determination of editable columns by format key.
	bool key2edit(t_cc);

#endif // SAMPLE_DBX_HPP

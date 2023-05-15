//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef CP_SAMPLES_DB_H
#define CP_SAMPLES_DB_H

#include "cproc.h"

//!	a simple table viewer #cproc.
	class cp_view_table : public db_cproc
	{
	public:
		void put(sbuffer&);
	};

//!	general table viewer setting: use column names as table cell (td) classes.
//! default: ON.
	extern bool vtac;

//!	general table viewer setting: display column names as table headings (th).
//! default: ON.
	extern bool vtch;

//!	general table viewer setting: instant contents translation.
//! default: OFF, OFF after each call of view_table / view_column.
	extern bool vttr;

//!	NULL display string (default nothing).
	extern s_cc s_db_null;

//!	a #cproc switch for table viewer column names as table cell (td) classes.
	class cp_view_table_autoclass : public bswitch
	{
	public:
		inline cp_view_table_autoclass()
		: bswitch(vtac)
		{}
	};

//!	a #cproc switch for table viewer column names as table headings.
	class cp_view_table_columns : public bswitch
	{
	public:
		inline cp_view_table_columns()
		: bswitch(vtch)
		{}
	};
//!	a #cproc switch for table viewer instant contents translation.
	class cp_view_table_trans : public bswitch
	{
	public:
		inline cp_view_table_trans()
		: bswitch(vttr)
		{}
	};
//!	a #cproc to adjust what to display if a database field has NULL value.
//! default is show nothing.
	class cp_NULL : public sswitch
	{
	public:
		inline cp_NULL()
		: sswitch(s_db_null)
		{}
	};

//!	a #cproc to view contents of a database table column separated by line breaks.
	class cp_view_column : public db_cproc
	{
	public:
		void put(sbuffer&);
	};

//!	a database tables list #cproc.
	class cp_tables : public db_cproc
	{
	public:
		void put(sbuffer&);
	};

//!	a database table editor #cproc.
	class cp_edit_table : public db_cproc
	{
	public:
		void put(sbuffer&);
	};


#endif // CP_SAMPLES_DB_H

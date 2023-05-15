//| ============================================================
//! @file
//!
//! sqlite database access.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef DB_H
#define DB_H

#include "ccs.h"
#include "cmapper.h"
#include "stypes.h"
#include "types.h"

class sqlite3;
class sqlite3_stmt;

namespace db
{
	class con
	{
	public:
		con();
		inline ~con() { disconnect(); }
		bool connect(t_cc, const csbuffer&);
		void disconnect();
	protected:
		sqlite3* d;
		int rc;
		char* ce;
	//! sqlite statement pointer.
		sqlite3_stmt* sqs;
	//!	last database character.
	//!	(precisely character after last one.)
		inline const char ldbc() { return cas; }
	//!	process sql statement(s) without data return.
		bool sqx(t_cc);
	//!	sql buffer.
		cbuffer bq;
	private:
	//!	"attach as" database prefix character.
		char cas;
	};

//	class data_setter;
	class proc;

//!	data tainer produced by select query of #proc.
//!	has to be deleted after usage.
	class data
	{
	public:
		data();
	//!	number of rows retrieved.
		inline unsigned rows() const { return nr; }
	//!	number of columns retrieved.
		inline unsigned cols() const { return nc; }
	//!	column name.
		inline const s_cc& col(unsigned n) const { return bcols[n]; }
		inline const s_cc& operator[](unsigned n) const { return bcols[n]; }
	//!	next data value.
		inline const s_cc& next() const { return bdata[pos++]; }
	//! go back to position 0.
		inline void rewind() const { pos = 0; }
	//!	retrieve value at row / column.
		inline const s_cc& val(unsigned r, unsigned c) { return bdata[r * nc + c]; }
	//!	find a string within data.
	//!	CAUTION data will be sorted afterwards.
		inline bool find(t_cc c) { return bdata.find(c, true); }
	private:
		friend class proc;
	//!	columns.
		ncsbuffer bcols;
	//!	data.
		ncsbuffer bdata;
	//!	number of rows.
		unsigned nr;
	//!	number of cols.
		unsigned nc;
	//	current position.
		mutable unsigned pos;
	};

//	db stream function class.
	class dfunc;
//	pseudo stream types
	struct t_and {};
	struct t_not_null {};
	struct t_or {};
	struct t_pile {};
	struct t_where {};

//	pseudo stream type generators

//!	stream table name to retrieve data from:
//!	db::proc& << db::from(table name).
	dfunc* from(t_cc t);

//!	stream beginning of where clause:
//!	db::proc& << db::where().
	const t_where* where();

//!	stream a not null statement after where clause:
//!	db::proc& << db::where() << some_column << db::nn().
	const t_not_null* nn();

//!	stream a join to another table:
//!	db::proc& << db::join(ft).
//!	@param ft double meaning referring field and reffered table
	dfunc* join(t_cc ft);

//!	stream a join to another table:
//!	db::proc& << db::join(p, ft).
//!	@param p  position of refferring table in stream, first is 1
//!	@param ft double meaning referring field and reffered table
	dfunc* join(unsigned p, t_cc ft);

//!	begin table update with table and given "Id" value.
//!	@param t table name
//!	@param i Id as string
	dfunc* update(t_cc t, t_cc i);

//!	begin table update with table and given "Id" value.
//!	@param t table name
//!	@param i Id as numeric value
	dfunc* update(t_cc t, unsigned i);

//!	begin table update with table, specified field and field value.
//!	@param t table name
//!	@param f field name
//!	@param v field value as string
	dfunc* update(t_cc t, t_cc f, t_cc v);

//!	begin table update with table, specified field and field value.
//!	@param t table name
//!	@param f field name
//!	@param v field value as numeric value
	dfunc* update(t_cc t, t_cc f, unsigned v);

//!	execute statement.
//!	@param tr send / execute database update trigger.
	dfunc* exec(bool tr = false);

//!	pile up statement instead of instant execution.
	const t_pile*	pile();

//!	stream AND.
	const t_and*	a();

//!	stream OR.
	const t_or*		o();

//!	remove a data set form table.
//!	@param t table name

	dfunc* remove(t_cc t, t_cc i);
	dfunc* remove(t_cc t, unsigned i);
	dfunc* insert(t_cc t);
	dfunc* order(bool = true);

//!	table modification (update) trigger type.
	class update_trigger
	{
	public:
	//!	trigger call.
	//!	@param sb updated tables list
		virtual void trigger(const sbuffer& sb) = 0;
	};

//!	sql processor class.
	class proc : public con
	{
	public:
	//!	standard instance.
		static proc& instance();

	//!	assign update trigger.
		inline void set_trigger(update_trigger* t) { trg = t; }

	//!	general stream behaviour.
		template <class T>
		inline proc& operator<<(T t) { add(t); return *this; }

	//!	retrieve a data container after data retrieval statement.
	//!	@param dv insert default values if table empty.
		data* get(bool dv = false);

	//!	simply get the count of available data after data retrieval statement.
		unsigned count();

	//!	if only one value expected: get this one.
	//!	return is zero if none found.
		t_fcc getv();

	//!	get last id of a table.
	//!	e.g. after an insert.
		t_fcc lid(t_cc);

	//!	list of all primary tables (connected darabase).
		inline bool ptables(csbuffer& sb) { return gtbls(sb); }
	//!	list of all secondary tables (attached databases).
		bool atables(csbuffer&);
	//! list of all tables (connected and attached).
		inline bool tables(csbuffer& sb) { return gtbls(sb) && atables(sb); }

	//!	process an exec #dfunc or developer's commit();
		bool commit(bool = false);
	//!	save update / remove / insert for bulk execution.
		void pile();

	//!	send a database update trigger.
		void trigger();

	//!	information wether bulk updates are standing out.
		inline bool updates() { return bb; }

	//!	add a new empty set of data to table.
		bool addset(t_cc);

	//!	process a from #dfunc.
		void d_from(t_cc);
	//!	process a join #dfunc.
		void d_join(unsigned, t_cc);
	//!	process an order #dfunc.
		void d_order(bool);
	//!	process an update #dfunc.
		void d_update(t_cc, t_cc, unsigned, t_cc);
	//!	process a remove #dfunc.
		void d_remove(t_cc, t_cc, unsigned);
	//!	process an insert #dfunc.
		void d_insert(t_cc);

	private:
	//!	individual instance constructor.
		proc();

	//!	data retrieval query string builder.
		t_cc str();

	//	adds for different types
	//!	dfunc processing.
		void add(dfunc*);
	//!	context dependent: add a string.
		void add(t_cc);
	//!	context dependent: add a numeric value.
		void add(int);
	//!	not null.
		void add(const t_not_null*);
	//!	where.
		inline void add(const t_where*) { state = st_where_var; }
	//!	where: and.
		inline void add(const t_and*)	{ cwa = cao = c_db_and; }
	//!	where: or.
		inline void add(const t_or*)	{ cwa = cao = c_db_or; }
	//!	pile up last update statement for bulk transaction.
		void add(const t_pile*);

	//!	prepare remove / update statement.
		void aru(int);
	//!	assambly sql of update / insert / remove.
		bool asq(bool = true);
	//!	data reset.
		bool reset(bool = false);

	//!	table listings sub.
		bool gtbls(csbuffer&, char = 0);

	//!	"select ..." buffer.
		cbuffer bs;
	//!	"where ... " buffer.
		cbuffer bw;
	//! join buffer.
		cbuffer bj;
	//!	order by buffer.
		cbuffer bo;
	//!	bulk execution sql buffer.
		ncsbuffer bb;
	//!	tables involved buffer.
		sbuffer bt;

	//!	input state
		enum {
		//	before select
			st_null,
		//	after select / join
			st_vars,
		//	order by
			st_order,
		//	after where expecting var name or unsigned
			st_where_var,
		//	after where var expecting value
			st_where_val,
		//	update mode: vars
			st_update_var,
			st_update_val,
		//	insert mode: vars
			st_insert_var,
			st_insert_val,
		//	removal of entries
			st_remove
		};
		int state;
	//	interim variable name
		t_ch cvn;
	//	where or and / or
		t_ch cwa;
	//	and	/ or
		t_ch cao;
	//	order by / ,
		t_ch cord;
	//	sql statement has been built indicator
		bool built;
	//	main table name
		t_ch tbl;
	//	minor runtime vars
		t_ch cid;
		unsigned nid;
		t_ch fld;
		bool desc;
	//!	table numeration.
		unsigned cn;
	//!	assigned trigger.
		update_trigger* trg;
	};
} // namespace db

#endif // DB_H

//| ============================================================
//! @file
//!
//! sqlite database access.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include <db.h>

#include <ccs.h>
#include <dbn.h>
#include <defs.h>
#include <vars.h>

// #define XDEVEL
#include <trace_macros.h>

#include <sqlite3.h>
#include <cstdlib>
#include <cstring>


namespace db
{
//	============================================================
//	specialized literals
	t_cc c_JOIN 		= " LEFT OUTER JOIN ";
	t_cc c_ON_T 		= " ON T";
	t_cc c_WHERE_ALL 	= "1=1";
	t_cc c_COUNT		= "count(*)";
	t_cc c_sq_master	= "sqlite_master";
	t_cc c_sq_sequence	= "sqlite_sequence";
	t_cc c_sq_seq		= "seq";
	t_cc c_sq_name		= "name";
	t_cc c_EQQ			= "=\"";
	const char c_QQ		= '"';

//	============================================================
//	db::con

	con::con()
	: d(0)
	, cas('A')
	, bq(128)
	{}

	bool con::connect(t_cc dir, const csbuffer& sb)
	{
		TRACE_FLOW
		if (d) return true;
		if (!sb) return false;
	//	open database attempt
		bq.clean() << dir << c_DIR_SEP << sb[0] << c_ext_db;
		TRACE_INF("open " << bq.str())
		rc = sqlite3_open(bq, &d);
		if (rc != SQLITE_OK) {
			TRACE_ERR("sqlite3_open")
			d = 0;
			return false;
		}
		cas = 'A';
	//	attach other databases
		for (int i = 1; i < sb.size(); ++i) {
			TRACE_INF(c_db_attach << sb[i])
			bq.clean() << c_db_attach << c_QQ << dir << c_DIR_SEP << sb[i] << c_ext_db << c_QQ << c_db_as << cas << ';';
			TRACE_VARQ(bq.str())

			if (!sqx(bq)) {
				disconnect();
				return false;
			}
			++cas;
		}
		bq.clean();
		return true;
	}

	void con::disconnect()
	{
		if (d) {
			sqlite3_close(d);
			d = 0;
		}
	}

//!	process sql statement(s) without data return.
	bool con::sqx(t_cc cs)
	{
		TRACE_FLOW
	//	are we conected?
		if (!(d && cs)) return false;
		TRACE_VARQ(cs)
	//	prepare statement
		rc = sqlite3_prepare(
			d,
			cs,
			strlen(cs),
			&sqs,
			0
		);
		bool ok = (rc == SQLITE_OK);
	//	TODO con::sqexec return of sqlite3_step
		if (ok) rc = sqlite3_step(sqs);
		else {
			TRACE_ERR("con::sqx")
		}
		sqlite3_finalize(sqs);
		return ok;
	}
//	============================================================
//	pseudo stream type generators

	class dfunc
	{
	public:
		virtual ~dfunc() {}
		virtual void operator()(proc*) {}
	};

	const t_where* 		where() 	{ return 0; }
	const t_not_null* 	nn() 		{ return 0; }
	const t_pile* 		pile() 		{ return 0; }
	const t_or* 		o() 		{ return 0; }
	const t_and* 		a() 		{ return 0; }

//	============================================================
//	trigger && null string
	update_trigger* p_trigger = 0;
	void set_update_trigger(update_trigger* pt)
	{
		TRACE_FLOW
		p_trigger = pt;
	}

//	============================================================
//	db:data
	data::data()
	: bcols(12)
	, bdata(36)
	, nc(0)
	, nr(0)
	, pos(0)
	{}

//	============================================================
//	db:proc
	proc& proc::instance()
	{
		static proc* p = new proc;
		return *p;
	}

	proc::proc()
	: bs(32)
	, bw(32)
	, bj(32)
	, bb(16)
	, bt(16, true)
	, cao(c_db_and)
	, tbl(0)
	, trg(0)
	{}

	t_cc proc::str()
	{
		if (!built) {
			if (!tbl) {
				reset();
				return 0;
			}
		//	select ...
			bq.clean() << c_db_select;
			if (bs) bq.add(bs.str());
			else bq.add('*');
		//	from ...
			bq << c_db_from << tbl << " T1";
		//	joins
			if (bj) bq.add(bj.str());
		//	where clauses
			if (bw) bq.add(bw.str());
		//	order by clauses
			if (bo) bq.add(bo.str());
		//	finalyze
			bq.add(';');
			built = true;
		}
		return bq;
	}

	t_fcc proc::lid(t_cc t)
	{
		TRACE_FLOW
		bs.clean();
		bw.clean();
		bo.clean();
		bj.clean();
		built = false;
		tbl = c_sq_sequence;
		bs << c_sq_seq;
		bw << c_db_where << "T1." << c_sq_name << "=\"" << t << '"';
		return getv();
	}


//	============================================================
//	dfunc*

	void proc::add(dfunc* df)
	{
		(*df)(this);
		delete df;
	}

//	============================================================
//	dfunc: remove
	class df_remove : public dfunc
	{
	public:
		inline df_remove(t_cc t, t_cc c, unsigned n)
		: ct(t)
		, ci(c)
		, ni(n)
		{}
		t_cc ct;
		t_cc ci;
		unsigned ni;
		void operator()(proc* pr)
		{
			TRACE_FLOW
			pr->d_remove(ct, ci, ni);
		}
	};

	dfunc* remove(t_cc t, t_cc i)
	{
		return new df_remove(t, i, 0);
	}
	dfunc* remove(t_cc t, unsigned i)
	{
		return new df_remove(t, 0, i);
	}

	void proc::d_remove(t_cc t, t_cc c, unsigned n)
	{
		TRACE_FLOW
		tbl = t;
		cid = c;
		nid = n;
		fld = dbn::Id;
		aru(st_remove);
	}

//	============================================================
//	dfunc: insert
	class df_insert : public dfunc
	{
	public:
		inline df_insert(t_cc t)
		: ct(t)
		{}
		t_cc ct;
		void operator()(proc* pr)
		{
			TRACE_FLOW
			pr->d_insert(ct);
		}
	};

	dfunc* insert(t_cc t)
	{
		return new df_insert(t);
	}

	void proc::d_insert(t_cc t)
	{
		TRACE_FLOW
		bs.clean();
		bw.clean();
		bj.clean();
		tbl = t;
		state = st_insert_var;
	}
//	============================================================
//	dfunc: update
	class df_update : public dfunc
	{
	public:
		inline df_update(t_cc t, t_cc c, unsigned n, t_cc f)
		: ct(t)
		, ci(c)
		, ni(n)
		, cf(f)
		{}
		t_cc ct;
		t_cc ci;
		unsigned ni;
		t_cc cf;
		void operator()(proc* pr)
		{
			TRACE_FLOW
			pr->d_update(ct, ci, ni, cf);
		}
	};

	dfunc* update(t_cc t, t_cc i)
	{
		return new df_update(t, i, 0, dbn::Id);
	}
	dfunc* update(t_cc t, unsigned i)
	{
		return new df_update(t, 0, i, dbn::Id);
	}
	dfunc* update(t_cc t, t_cc f, t_cc v)
	{
		return new df_update(t, v, 0, f);
	}
	dfunc* update(t_cc t, t_cc f, unsigned v)
	{
		return new df_update(t, 0, v, f);
	}

	void proc::d_update(t_cc t, t_cc c, unsigned n, t_cc f)
	{
		TRACE_FLOW
		tbl = t;
		cid = c;
		nid = n;
		fld = f;
		aru(st_update_var);
	}

//	============================================================
//	dfunc: exec
	class df_exec: public dfunc
	{
	public:
		inline df_exec(bool t)
		: tr(t)
		{}
		bool tr;
		void operator()(proc* pr)
		{
			TRACE_FLOW
			pr->commit(tr);
		}
	};
	dfunc* exec(bool t)
	{
		return new df_exec(t);
	}

//	============================================================
//	dfunc: from
	class df_from: public dfunc
	{
	public:
		inline df_from(t_cc t)
		: ct(t)
		{}
		t_cc ct;
		void operator()(proc* pr)
		{
			TRACE_FLOW
			pr->d_from(ct);
		}
	};
	dfunc* from(t_cc t)
	{
		return new df_from(t);
	}
	void proc::d_from(t_cc t)
	{
		built = false;
		bs.clean();
		bw.clean();
		bo.clean();
		bj.clean();
		tbl = t;
		cn = 1;
		cwa = c_db_where;
		cord = c_db_order_by;
		state = st_vars;
	}

//	============================================================
//	dfunc: join
	class df_join : public dfunc
	{
	public:
		inline df_join(unsigned i, t_cc c)
		: p(i)
		, ft(c)
		{}
		unsigned p;
		t_cc ft;
		void operator()(proc* pr)
		{
			TRACE_FLOW
			pr->d_join(p, ft);
		}

	};

	dfunc* join(unsigned p, t_cc ft)
	{
		TRACE_FLOW
		return new df_join(p, ft);
	}
	dfunc* join(t_cc ft)
	{
		TRACE_FLOW
		return new df_join(0, ft);
	}
	void proc::d_join(unsigned p, t_cc ft)
	{
		TRACE_FLOW
		p = p ? p : cn;
		++cn;
		bj << c_JOIN << ft << " T" << cn << c_ON_T << p << '.' << ft << "=T" << cn << '.' << dbn::Id;
		state = st_vars;
	}

//	============================================================
//	dfunc: order
	class df_order : public dfunc
	{
	public:
		inline df_order(bool b)
		: asc(b)
		{}
		bool asc;
		void operator()(proc* pr)
		{
			TRACE_FLOW
			pr->d_order(asc);
		}
	};

	dfunc* order(bool asc)
	{
		TRACE_FLOW
		TRACE_VARB(asc)
		return new df_order(asc);
	}

	void proc::d_order(bool asc)
	{
		TRACE_FLOW
		TRACE_VARB(asc)
		desc = !asc;
		state = st_order;
	}

//	end: dfunc
//	============================================================

//	check text input: detection of quotes '"':
	t_cc chktxt(t_cc cc) {
		unsigned l = 0;
		unsigned q = 0;
		t_ch c;
		for (c = cc; *c; ++c, ++l) if (*c == c_QQ) ++q;
		if (!q) return cc;
	//	quotes detected
		l += q;
		fbuffer& fb = rfb(l);
		for (c = cc; *c; ++c) {
			fb.add(*c);
		//	double quotes
			if (*c == c_QQ) fb.add(*c);
		}
		return fb.str();
	}

	void proc::add(t_cc c)
	{
		switch (state) {
	//	SELECT
		//	select: vars
			case st_vars:
				if (bs) bs.add(',');
				bs << 'T' << cn << '.' << c;
			break;
		//	select order
			case st_order:
				bo << cord << 'T' << cn << '.' << c;
				if (desc) bo << c_db_desc;
				cord = ",";
			break;
		//	after where: expecting var name or unsigned
			case st_where_var:
				cvn = c;
				state = st_where_val;
			break;
		//	after where var: expecting value
			case st_where_val:
				bw << cwa << 'T' << cn << '.' <<  cvn << c_EQQ << chktxt(c) << c_QQ;
				cwa = cao;
				state = st_where_var;
			break;
	//	UPDATE
		//	after update statement expecting var name
			case st_update_var:
				if (bs) bs.add(',');
				else bs << c_db_set;
				bs.add(c);
				state = st_update_val;
			break;
		//	after update var name expecting value
			case st_update_val:
				if (c == c_db_null) bs << "=" << c;
				else bs << c_EQQ << chktxt(c) << c_QQ;
				state = st_update_var;
			break;
	//	INSERT
			case st_insert_var:
				if (bw) bw.add(',');
				bw.add(c);
				state = st_insert_val;
			break;
			case st_insert_val:
				if (bs) bs.add(',');
				bs << c_QQ << chktxt(c) << c_QQ;
				state = st_insert_var;
			break;

			default:
			break;
		}
	}

	void proc::add(int n)
	{
		switch (state) {
			case st_where_var:
				if (n < 0) break;
				cvn = dbn::Id;
			case st_where_val:
				bw << cwa << 'T' << cn << '.' <<  cvn << '=' << n;
				cwa = cao;
				state = st_where_var;
			break;
			case st_update_val:
				bs << "=" << n;
				state = st_update_var;
			break;
			case st_insert_val:
				if (bs) bs.add(',');
				bs << n;
				state = st_insert_var;
			break;
			default:
			break;
		}
	}

	void proc::add(const t_not_null*)
	{
		if (state == st_where_val) {
			bw << cwa << 'T' << cn << '.' <<  cvn << c_db_not << c_db_null;
			cwa = cao;
			state = st_where_var;
		}
	}

	t_fcc proc::getv()
	{
		TRACE_FLOW
	//	are we conected?
		if (!d) return 0;
	//	assembly of sql statement
		t_cc cs = str();
		if (!cs) return 0;
		TRACE_VARQ(cs)

		static s_cc sv;
		sv = 0;
	//	prepare statement
		rc = sqlite3_prepare(
			d,
			cs,
			strlen(cs),
			&sqs,
			0
		);
		if (rc == SQLITE_OK) {
			bool ok = (sqlite3_column_count(sqs) == 1);
			if (ok) {
				rc = sqlite3_step(sqs);
				ok = (rc == SQLITE_ROW);
			}

			if (ok) sv.copy(reinterpret_cast<t_cc>(sqlite3_column_text(sqs, 0)));
		}

		sqlite3_finalize(sqs);
		TRACE_VARQ(sv.str())
		return sv;
	}

//!	add a new empty set of data
	bool proc::addset(t_cc t)
	{
		bw.clean() << c_db_insert << c_db_into << t << c_db_default << c_db_values << ';';
		return sqx(bw);
	}

//!	execute query.
	data* proc::get(bool dv)
	{
		TRACE_FLOW
	//	are we conected?
		if (!d) return 0;
	//	assembly of sql statement
		t_cc cs = str();
		if (!cs) return 0;
		TRACE_VARQ(cs)
		data* pd = 0;
	//	prepare statement
		rc = sqlite3_prepare(
			d,
			cs,
			strlen(cs),
			&sqs,
			0
		);
		if (rc == SQLITE_OK) {
		//	if that was ok:
		//	- count columns
			unsigned cols = sqlite3_column_count(sqs);
			if (cols) {
				unsigned rows = 0;
			//	- create new datatainer
				pd = new data;
			//	- and retrieve data
				while (true) {
					rc = sqlite3_step(sqs);
					if (rc != SQLITE_ROW) break;
					++rows;
					for (int c = 0; c < cols; ++c) {
						t_ucc uc = sqlite3_column_text(sqs, c);
						if (uc) pd->bdata.copy(reinterpret_cast<t_cc>(uc));
						else pd->bdata.add(cnull);
					}
				};
			//	behaviour with no data found
				if (!(rows || dv)) {
					delete pd;
					pd = 0;
				}
				else {
				//	set count values
					pd->nc = cols;
					pd->nr = rows;
				//	set column names
					for (int c = 0; c < cols; ++c)
						pd->bcols.copy(sqlite3_column_name(sqs, c));
				}
			}
		}
		sqlite3_finalize(sqs);

	//	return new address
		return pd;
	}

//!	execute count
	unsigned proc::count()
	{
		TRACE_FLOW
	//	are we conected?
		if (!d) return 0;
		bs.clean() << c_COUNT;

	//	assembly of sql statement
		t_cc cs = str();
		if (!cs) return 0;
		TRACE_VARQ(cs)

		unsigned cn = 0;
	//	prepare statement
		rc = sqlite3_prepare(
			d,
			cs,
			strlen(cs),
			&sqs,
			0
		);
		if (rc == SQLITE_OK) {
			bool ok = (sqlite3_column_count(sqs) == 1);
			if (ok) {
				rc = sqlite3_step(sqs);
				ok = (rc == SQLITE_ROW);
			}
			if (ok) cn = sqlite3_column_int(sqs, 0);
		}
		sqlite3_finalize(sqs);
		TRACE_VAR(cn)
		return cn;
	}

	void proc::aru(int s)
	{
		TRACE_FLOW
		bs.clean();
		bw.clean();
		bj.clean();
		state = s;

	//	wildcard given?
		if (cid && !strcmp(cid, c_db_wildcard)) bw << c_db_where << c_WHERE_ALL;
	//	id given?
		else if (cid || nid) {
			if (cid)	bj << cid;
			else 		bj << nid;
			bw << c_db_where << fld << '=';
			if (fld == dbn::Id) bw << bj.str();
			else bw << '"' << bj.str() << '"';
		}
		else state = st_null;
	}

//	update uses:
//	- bj for id storage
//	- bw for id where clause storage
//	- bs for set ... storage
//	insert uses
//	- bw for var name storage
//	- bs for value storage


//	assambly sql of update / insert / remove
	bool proc::asq(bool c)
	{
		if (c) {
			bq.clean();
			switch (state) {
				case st_update_var:
				case st_insert_var:
				case st_remove:
				break;
				default:
				return false;
			}
		}
		if (!bw) return false;

		switch (state) {
			case st_update_var:
				if (!bs) return false;
				bq << c_db_update << tbl << bs.str() << bw.str();
			break;
			case st_insert_var:
				if (!bs) return false;
				bq << c_db_insert << c_db_into << tbl << '(' << bw.str() << ')' << c_db_values << '(' << bs.str() << ')';
			break;
			case st_remove:
				bq << c_db_delete << c_db_from << tbl << bw.str();
			break;
			default:
				return false;
			break;
		}

	//	end of statement
		bq.add(';');

	//	add table name for trigger processing
		bt.add(tbl);

		bw.clean();
		bs.clean();
		return true;
	}

//	store update data for bulk execution
	void proc::add(const t_pile*)
	{
		pile();
	}

	void proc::pile()
	{
//		if (asq()) bb.copy(bq.str());
		if (asq()) bb.take(bq);
		reset();
	}

	bool proc::reset(bool bu)
	{
		state = st_null;
		bs.clean();
		bw.clean();
		bj.clean();
		bo.clean();
		bq.clean();
		if (bu) {
			bb.clean();
			bt.clean();
		}
		return false;
	}

	bool proc::commit(bool tr)
	{
		TRACE_FLOW
		if (!(d && (bb || bw))) return reset(true);

		if (!bb) switch (state) {
			case st_update_var:
			case st_insert_var:
				if (!bs) return reset(true);
			case st_remove:
			break;
			default:
			return reset(true);
		}
		bool ok = true;
		bq.clean();
	//	check if bulk statements where piled up
		if (bb) {
			for (int i = 0; ok && i < bb.size(); ++i) ok = sqx(bb[i]);
			bb.clean();
		}
	//	add last update / insert if given
		if (ok && asq(false)) ok = sqx(bq);

	//	trigger
		if (ok && tr) trigger();

	//	back to beginning
		reset(true);

		return ok;
	}

	void proc::trigger()
	{
		TRACE_FLOW
		if (trg) trg->trigger(bt);
//		for (int i = 0; i < bt.size(); ++i) (*p_trigger)(bt[i]);
	}

//	============================================================
//	table listings
	bool proc::gtbls(csbuffer& sb, char c)
	{
	//	are we conected?
		if (!d) return false;
	//	sql statements with or without database prefix
	//	select name from A.sqlite_master where type='table';
	//	select name from   sqlite_master where type='table';
		bq.clean() << c_db_select << c_name << c_db_from;
		if (c) bq << c << '.';
		bq	<< c_sq_master
			<< c_db_where << c_type << '=' << '\'' << c_table << '\'' << ';';

	//	prepare statement
		rc = sqlite3_prepare(
			d,
			bq,
			strlen(bq),
			&sqs,
			0
		);
		const bool ok = (
			rc == SQLITE_OK &&
			sqlite3_column_count(sqs) == 1
		);

		while (ok) {
			rc = sqlite3_step(sqs);
			if (rc != SQLITE_ROW) break;
			t_cc cc = reinterpret_cast<t_cc>(sqlite3_column_text(sqs, 0));
			if (strcmp(cc, c_sq_sequence)) sb.copy(cc);
		}
		sqlite3_finalize(sqs);
		return ok;
	}

	bool proc::atables(csbuffer& sb)
	{
	//	any dbs attached?
		const char ca = ldbc();
		if (ca == 'A') return true;
	//	executions with buffer callback
		bool ok = true;
		for (char c = 'A'; c < ca && ok; ++c) ok = gtbls(sb, c);
	//	result
		return ok;
	}

} // namespace db

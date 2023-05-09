//| ============================================================
//! @file
//!
//! #cproc retrieval and assignment CGI interface.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef ISSRV

#include "fcpcgi.hpp"

#include "ccs.hpp"
#include "cproc.hpp"
#include "defs.hpp"

#undef DEVEL
#include "trace_macros.hpp"

#include <cstdlib>
// #include <cstring>

//	============================================================
//	CPROC FINDER

	const fcpcgi& fcpcgi::instance(void)
	{
		static fcpcgi f;
		return f;
	}

	const fcpcgi& fcp = fcpcgi::instance();

	class fcpair
	{
	public:
		fcpair(t_cc k, ccproc* c)
		: key(k)
		, ccp(c)
		{}
		inline cproc* proc(void) const { return (*ccp)(); }
		t_cc key;
	private:
		ccproc* ccp;
	};


	typedef const fcpair* const t_fcp;
//!	compare function for qsort / bsearch
	int fcpcomp(const void* p1, const void* p2)
	{
		t_fcp* fcp1 = reinterpret_cast<t_fcp*>(p1);
		t_fcp* fcp2 = reinterpret_cast<t_fcp*>(p2);
		return strcmp((*fcp1)->key, (*fcp2)->key);
	}

	fcpcgi::fcpcgi()
	: bs(0)
	, db(0)
	, st(c_BUF_STEP_FCPROC)
	{}

//!	Assign a processor creator to a keyword.
	void fcpcgi::set(t_cc k, ccproc* c)
	{
		TRACE_FLOW
		if (alloc()) {
			fcpair* sp = new fcpair(k, c);
			db[ds] = sp;
			++ds;
		}
	}

//!	memory allocation
	bool fcpcgi::alloc(void)
	{
		TRACE_FLOW
		if (bs > ds) return true;
		int ns = bs + st;
		TRACE_VAR(ns)
		int ms = ns * sizeof(fcpair*);
		db = reinterpret_cast<fcpair**>(bs ? realloc(db, ms) : malloc(ms));
		bs = ns;
		TRACE_VAR(ds)
		TRACE_VAR(bs)
		return true;
	}
//! call sort after insertions done.
//!	Otherwise map won't work.
	void fcpcgi::sort(void)
	{
		TRACE_FLOW
		if (!ds) return;
		if (bs > ds) {
			int ms = ds * sizeof(fcpair*);
			db = reinterpret_cast<fcpair**>(realloc(db, ms));
			bs = ds;
		}
		qsort(db, ds, sizeof(fcpair*), &fcpcomp);
	}
//!	internal find
	fcpair* fcpcgi::find(t_cc k) const
	{
		TRACE_FLOW
		if (!ds && k) return 0;
		fcpair sp(k, 0);
		t_fcp fcp1 = &sp;
		void* p = bsearch(&fcp1, db, ds, sizeof(fcpair*), &fcpcomp);
		return p ? *reinterpret_cast<fcpair**>(p) : 0;
	}

//!	find and process: ok
	bool fcpcgi::ok(t_cc c) const
	{
		TRACE_FLOW
		sbuffer sb(1);
		t_fcp spc = find(c);
		return spc && spc->proc()->ok(sb);
	}
//!	find and process: ok
	bool fcpcgi::ok(sbuffer& sb) const
	{
		TRACE_FLOW
		t_fcp spc = find(sb.shift());
		return spc && spc->proc()->ok(sb);
	}

//!	find and process: put
	void fcpcgi::put(sbuffer& sb) const
	{
		TRACE_FLOW
		while (sb) {
			TRACE_VARQ(sb[0])
			t_fcp spc = find(sb.shift());
			if (spc) spc->proc()->put(sb);
		}
	}

//!	find and process: set
	void fcpcgi::set(sbuffer& sb) const
	{
		while (sb) {
			t_fcp spc = find(sb.shift());
			if (spc) spc->proc()->set(sb);
		}
	}

#endif
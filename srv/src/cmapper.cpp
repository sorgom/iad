//| ============================================================
//! @file
//!
//! General base classes for character string mapping.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include "cmapper.hpp"

#include "defs.hpp"

// #undef DEVEL
// #define XDEVEL
#include "trace_macros.hpp"

#include <cstdlib>
#include <cstring>

	struct spair
	{
		s_cc key;
		s_cc val;
	};
	typedef const spair* const t_spc;

//!	compare function for qsort / bsearch
	int spcomp(const void* p1, const void* p2)
	{
		TRACE_FLOW
		t_spc spp1 = *reinterpret_cast<t_spc*>(p1);
		t_spc spp2 = *reinterpret_cast<t_spc*>(p2);
		TRACE_VARQ(spp1->key.str())
		TRACE_VARQ(spp2->key.str())
		return strcmp(spp1->key, spp2->key);
	}

//!	compare function for qsort / bsearch
	int spxcomp(const void* p1, const void* p2)
	{
		TRACE_FLOW
		t_spc spp1 = *reinterpret_cast<t_spc*>(p1);
		t_spc spp2 = *reinterpret_cast<t_spc*>(p2);

		TRACE_ADDR(spp1)
		TRACE_ADDR(spp2)

		if (spp1 && spp2) return strcmp(spp1->key, spp2->key);
		if (spp1) return -1;
		if (spp2) return  1;
		return 0;
	}

//	============================================================
//	static

	s_cc cmapper::sck;
	s_cc cmapper::scv;
	bool cmapper::setkv(t_cc k, t_cc v, bool ck, bool cv)
	{
		TRACE_FLOW
		if (!k) return false;

		TRACE_VARQ(k)
		TRACE_VARQ(v)
		TRACE_VAR(ck)
		TRACE_VAR(cv)

		if (ck) sck.copy(k);
		else sck = k;
		if (v && cv) scv.copy(v);
		else scv = v;
		return true;
	}


//!	constructor [with given expansion step size].
    cmapper::cmapper(unsigned s, cm_load_func* lf)
	: dsb(s ? s : c_BUF_STEP_CMAPPER)
	, bs(0)
	, db(0)
	, ldfc(lf)
	, loaded(false)
	, srtd(true)
	{}

//!	string search device.
	t_cc cmapper::find(t_cc k, bool f) const
	{
		TRACE_FLOW
		if (f && !srtd) {
			qsort(db, ds, sizeof(spair*), &spcomp);
			srtd = true;
		}
		return getn(k);
	}

//! the core method: get the meaning of a key.
//! Does not return NULL pointer (unless value is null).
//! Returns empty character string in case of not found.
    const s_cc& cmapper::get(t_cc k) const
    {
		TRACE_FLOW
		spair* sp = findp(k);
		return (sp ? sp->val : sdef);
	}

//! variation of core method: get the meaning of a key.
//! Return NULL pointer in case of not found..
    const s_cc& cmapper::getn(t_cc k) const
    {
		TRACE_FLOW
		spair* sp = findp(k);
		return (sp ? sp->val : snull);
    }

//! convenience variation of core method:
//!	search by NOB,
//! Return NULL pointer in case of not found.
	const s_cc& cmapper::getn(const nob& nb) const
	{
		TRACE_FLOW
		cbuffer bf(c_BUF_STEP_NOB);
		nb.put(bf);
		return getn(bf);
	}

//! variation of core method: get the meaning of a key.
//! Does not return NULL pointer (unless value is null).
//! Returns key / cdef in case of not found.
	t_cc cmapper::getk(t_cc k) const
    {
		TRACE_FLOW
		if (!k) return cdef;
		t_ch v = 0;
		spair* sp = findp(k);
		if (sp) v = sp->val.str();
		return (v ? v : k ? k : cdef);
	}
//! variation of core method:
//! Does not return NULL pointer (unless value or def is null).
//! Returns default value d in case of not found.
	t_cc cmapper::getd(t_cc k, t_cc d) const
	{
		TRACE_FLOW
		spair* sp = findp(k);
		return (sp ? sp->val.str() : d);
	}

//!	first key available.
//!	(can be null)
	t_cc cmapper::firstkey(void)
	{
		TRACE_FLOW
		if (!ds) return 0;
		return db[0]->key;
	}

	t_cc cmapper::xset(t_cc k, t_cc v, bool s, bool ck, bool cv)
	{
		TRACE_FLOW
		if (setkv(k, v, ck, cv)) return set(sck, scv, s);
		return 0;
	}

	t_cc cmapper::xadd(t_cc k, t_cc v, bool ck, bool cv)
	{
		TRACE_FLOW
		if (setkv(k, v, ck, cv)) return add(sck, scv);
		return 0;
	}
//! set contents from s_cc
    t_cc cmapper::set(const s_cc& k, const s_cc& v, bool s)
	{
		TRACE_FLOW
		loaded = true;
		if (!k) return 0;
		if (!v) {
			if (s) remove(k);
			return 0;
		}
		spair* sp = s ? findp(k) : 0;
		if (sp) {
			sp->val = v;
			if (sp->key.refs() && !k.refs()) sp->key = k;
		}
		else if (alloc()) {
			sp = new spair;
			sp->key = k;
			sp->val = v;
			db[ds] = sp;
			++ds;
			srtd = false;
			if (s) sort();
		}
		if (sp) return sp->val;
		else return 0;
	}

//! add contents from s_cc
    t_cc cmapper::add(const s_cc& k, const s_cc& v)
	{
		TRACE_FLOW
		loaded = true;
		if (!k) return 0;
		spair* sp = findp(k);
		if (sp) {
			if (sp->key.refs() && !k.refs()) sp->key = k;
		}
		else if (v && alloc()) {
			sp = new spair;
			sp->key = k;
			sp->val = v;
			db[ds] = sp;
			++ds;
			srtd = false;
			sort();
		}
		if (sp) return sp->val;
		else return 0;
	}

//!	delete an entry.
	bool cmapper::remove(t_cc k)
	{
		TRACE_FLOW
		if (!(ds && k)) return false;
		TRACE_VARQ(k);
		spair sp;
		sp.key = k;
		spair* fp = &sp;
		bool ok = false;
		while (ds) {
			void* p = bsearch(&fp, db, ds, sizeof(spair*), &spcomp);
			if (!p) return ok;
			spair*& rp = *reinterpret_cast<spair**>(p);
			delete rp;
			rp = 0;
			qsort(db, ds, sizeof(spair*), &spxcomp);
			--ds;
			ok = true;
		}
		return ok;
	}

	void cmapper::intersec(const ssd& osd)
	{
		TRACE_FLOW
		if (!ds) return;
		TRACE_VAR(ds);
		int dsm = 0;
		spair** dp = db;
		for (unsigned i = ds; i--; ++dp) {
			if (!osd.find((*dp)->key)) {
				delete *dp;
				*dp = 0;
				++dsm;
			}
		}
		if (dsm) {
			qsort(db, ds, sizeof(spair*), &spxcomp);
			ds -= dsm;
		}
		else sort();
	}

//! call sort after insertions done.
//!	Otherwise map won't work.
	void cmapper::sort(bool r)
	{
		TRACE_FLOW
		if (ds) {
			if (r) shrink();
			if (!srtd) qsort(db, ds, sizeof(spair*), &spcomp);
		}
		srtd = true;
	}
//!	shrink memory to used size.
	void cmapper::shrink(void)
	{
		TRACE_FLOW
		if (ds && bs > ds) {
			unsigned ms = ds * sizeof(spair*);
			db = reinterpret_cast<spair**>(realloc(db, ms));
			bs = ds;
		}
	}

//! clear data contents.
	void cmapper::clear(void)
	{
		TRACE_FLOW
		TRACE_VAR(ds)
		TRACE_VAR(bs)
		if (db) {
			TRACE_INF("deleting")
			spair** pb = db;
			for (unsigned i = ds; i--; ++pb) {
				delete *pb;
				*pb = 0;
			}
			free(db);
			db = 0;
		}
		ds = 0;
		bs = 0;
	}
//!	clean memory without shrink.
	void cmapper::clean(void)
	{
		TRACE_FLOW
		if (db) {
			TRACE_INF("deleting")
			spair** pb = db;
			for (unsigned i = ds; i--; ++pb) {
				delete *pb;
				*pb = 0;
			}
		}
		ds = 0;
	}

//!	traverse all data with given traverser
	void cmapper::traverse(cm_const_traverser& tr) const
	{
		TRACE_FLOW
		if (!ds) return;
		TRACE_VAR(ds);
		spair** dp = db;
		for (unsigned i = ds; i--; ++dp) tr((*dp)->key, (*dp)->val);
	}

//!	traverse all data with given traverser
	void cmapper::traverse(cm_traverser& tr)
	{
		TRACE_FLOW
		spair** dp = db;
		for (unsigned i = ds; i--; ++dp) tr((*dp)->key, (*dp)->val);
	}

//!	traverse all data with given traverse function pointer.
	void cmapper::traverse(cm_const_trav_func* pf) const
	{
		TRACE_FLOW
		spair** dp = db;
		for (unsigned i = ds; i--; ++dp) (*pf)((*dp)->key, (*dp)->val);
	}

//!	traverse all data with given traverse function pointer.
	void cmapper::traverse(cm_trav_func* pf)
	{
		TRACE_FLOW
		spair** dp = db;
		for (unsigned i = ds; i--; ++dp) (*pf)((*dp)->key, (*dp)->val);
	}

//!	list all keys to an sbuffer.
	unsigned cmapper::keys(csbuffer& sb) const
	{
		TRACE_FLOW
		spair** dp = db;
		for (unsigned i = ds; i--; ++dp) sb.aoc((*dp)->key);
		return ds;
	}

//!	list all values to an sbuffer.
	unsigned cmapper::values(csbuffer& sb) const
	{
		TRACE_FLOW
		spair** dp = db;
		for (unsigned i = ds; i--; ++dp) sb.aoc((*dp)->val);
		return ds;
	}
//!	"grep" to an sbuffer.
//!	list all keys to a csbuffer which begin with given string.
	unsigned cmapper::grep(csbuffer& sb, t_cc k, unsigned l, bool gt) const
	{
		TRACE_FLOW
		if(!ds || !k) return 0;
		if (!l) l = strlen(k);
		TRACE_VARQ(k)
		TRACE_VAR(l)
		unsigned n = 0;
		spair** dp = db;
		for (unsigned i = ds; i--; ++dp) {
			const s_cc& sc = (*dp)->key;
			if (sc && !strncmp(sc, k, l) && (!gt || *(sc + l))) {
				sb.aoc(sc);
				++n;
			}
		}
		return n;
	}

	spair* cmapper::findp(t_cc k) const
	{
		TRACE_FLOW
		if (!k) return 0;
		if (!loaded) {
			loaded = true;
			if (ldfc) {
				TRACE_INF("calling load func")
				(*ldfc)(const_cast<cmapper&>(*this));
			}
		}
		if (!ds) return 0;
		spair sp;
		sp.key = k;
		t_spc fp = &sp;
		void* p = bsearch(&fp, db, ds, sizeof(spair*), &spcomp);
		return p ? *reinterpret_cast<spair**>(p) : 0;
	}

//!	memory allocation
	bool cmapper::alloc(void)
	{
		TRACE_FLOW
		if (ds < bs) return true;
		unsigned ns = bs + st;
		unsigned ms = ns * sizeof(spair*);
		db = reinterpret_cast<spair**>(bs ? realloc(db, ms) : malloc(ms));
		bs = ns;
		TRACE_VAR(ds)
		TRACE_VAR(bs)
		return true;
	}

//	============================================================
//	cmapper extension functions

//	some static vars required:
	t_ch mvkey;
	bool mvfound;
	bool mvover;
	sbuffer* mvbuf = 0;

//!	a cmapper traverser function asigning multiple values to a key.
	void trav_mvals(const s_cc& k, const s_cc& v)
	{
		if (mvover) return;
		if (!strcmp(mvkey, k)) {
			mvfound = true;
			mvbuf->add(v);
		}
		else if (mvfound) mvover = true;
	}

//!	for multiple cmappers:
//!	retreive multiple values assigned to a key to an sbuffer reference.
	bool cmvals(sbuffer& sb, const cmapper& cm, t_cc k)
	{
		if (!k) return false;
		mvkey 	= k;
		mvfound = false;
		mvover	= false;
		mvbuf 	= &sb;
		cm.traverse(&trav_mvals);
		return mvfound;
	}

//	============================================================
//	cmxc
//!	only one instance available.
	cmxc& cmxc::instance(void)
	{
		static cmxc* pcmxc = new cmxc;
		return *pcmxc;
	}
//!	set target, source, copy mode.
	const cmxc& cmxc::set(cmapper& trg, const cmapper& src, bool cp, bool ow)
	{
		cmsrc = &src;
		cmtrg = &trg;
		cpy = cp;
		owr = ow;
		return *this;
	}
//!	stream keys to copy.
	const cmxc& cmxc::operator<<(t_cc k) const
	{
		TRACE_FLOW
		t_ch v;
		if (cmsrc && cmtrg && (v = cmsrc->getn(k))) {
			TRACE_VARQ(v);
			if (owr)	cmtrg->xset(k, v, true, false, cpy);
			else 		cmtrg->xadd(k, v, false, cpy);
		}
		return *this;
	}
//!	def con.
	cmxc::cmxc()
	: cmsrc(0)
	, cmtrg(0)
	{}

//!	exchange values from cmapper to cmapper.
	const cmxc& cmx(cmapper& trg, const cmapper& src, bool cp, bool ow)
	{
		TRACE_FLOW
		cmxc& cx = cmxc::instance();
		cx.set(trg, src, cp, ow);
		return cx;
	}

	cmapper* pcmswap = 0;

	void trswap(const s_cc& sk, const s_cc& sv)
	{
		t_cc k = sk;
		t_cc v = sv;
		pcmswap->set(v, k, false);
	}

//!	convenience: swap keys and values.
	bool cswap(cmapper& trg, const cmapper& src)
	{
		if (&trg == &src) return false;
		pcmswap = &trg;
		src.traverse(&trswap);
		pcmswap = 0;
		trg.sort();
		return true;
	}

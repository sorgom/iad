//| ============================================================
//! @file
//!
//! Enhanced string processing types.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include "stypes.hpp"

//  tracing off: tests passed
// #define XDEVEL
// #undef DEVEL
#include "trace_macros.hpp"

//  for realloc etc.
#include <cstdio>
#include <cstdlib>
#include <cstring>

//	============================================================
//	frequently used stream types
	const tt_end tte;

//  ============================================================
//	dsb
	dsb::dsb(unsigned s)
		: ds(0)
		, st(s > 0 ? s : c_DEFAULT_CBUFF_STEP)
	{
		TRACE_FLOW
		TRACE_VAR(this)
		TRACE_VAR(s)
		TRACE_VAR(st)
	}

//  ============================================================
//	xbuffer
	xbuffer::xbuffer(unsigned s)
		: dsb(s)
		, db(0)
	{
		TRACE_FLOW
		TRACE_VAR(this)
		TRACE_VAR(st)
	}

	unsigned xbuffer::rs(void)
	{
		return db ? (ds = strlen(db)) : (ds = 0);
	}
//  ============================================================
//	helper

//	standardized char* creation
//	protects from mixing up malloc and new / free and delete
	inline char* ccprv(unsigned s)	{ return reinterpret_cast<char*>(malloc(s)); }

	inline void ccrmv(t_ch c) 		{ free(const_cast<char*>(c)); }

//  ============================================================
//  class s_cc

//!	empty constructor
    s_cc::s_cc()
	: cc(cnull)
	, rc(0)
	{ }

//! Constructor from #t_cc aussumes const pointer
//! and thus does not copy anything.
    s_cc::s_cc(t_cc c)
	: cc(c)
	, rc(0)
	{ }

//! constructor from xbuffer copying contents.
//! should only be used for computed values.
    s_cc::s_cc(const xbuffer& cb)
    : cc(ccprv(cb.size() + 1))
    , rc(new unsigned(1))
    {
        strcpy(const_cast<char*>(cc), cb.str());
    }

//! Copy Constructor without copying the contents.
//! Just points to same contents as original.
//! Encreases reference counter if given.
	s_cc::s_cc(const s_cc& o)
	: cc(o.cc)
	, rc(o.rc)
	{
        if (rc) ++*rc;
	}
//! Copy operator from s_cc
	void s_cc::operator=(const s_cc& o)
	{
		if (&o == this) return;
		unlink();
		cc = o.cc;
		rc = o.rc;
		if (rc) ++*rc;
	}

//! Copy operator from xbuffer.
    void s_cc::operator=(const xbuffer& cb)
    {
        unlink();
        cc = ccprv(cb.size() + 1);
        strcpy(const_cast<char*>(cc), cb.str());
        rc = new unsigned(1);
    }
//! Copy operator from const char* const.
    void s_cc::operator=(t_cc c)
    {
		if (c == cc) return;
        unlink();
        rc = 0;
        cc = c;
    }
//!	explicit copy contents from unsave t_cc
	void s_cc::copy(t_cc c)
	{
		TRACE_FLOW
		TRACE_VARQ(c)
		if (c) copy(c, strlen(c));
		else unlink();
	}

//!	explicit copy contents from unsave t_cc
	void s_cc::copy(t_cc c, unsigned l)
	{
		TRACE_FLOW
		if (!c && l) {
			unlink();
			return;
		}
		TRACE_VAR(l)
		char* c2 = ccprv(l + 1);
        memcpy(c2, c, l);
		c2[l] = 0;
		unlink();
		cc = c2;
        rc = new unsigned(1);
		TRACE_VARQ(c2)
	}
//!	take over a hep seated string.
//!	string is going to be deleted with destruction.
	void s_cc::take(t_cc c)
	{
		unlink();
		if (!c) return;
		cc = c;
        rc = new unsigned(1);
	}

//! mutable access to data
//!	only given if based on copy
	char* s_cc::ms(void) const
	{
		TRACE_FLOW
		return rc ? const_cast<char*>(cc) : 0;
	}

//! Information about reference count.
//! refs 0 means referring to built in t_cc.
//! refs 1 or more means carrying 1 copy,
//! which is referred to by refs objects.
    unsigned s_cc::refs(void) const { return rc ? *rc : 0; }

//!	empty information.
	bool s_cc::empty(void) const
	{
		return !cc || !cc[0];
	}
//!	the opposit of empty.
	bool s_cc::val(void) const
	{
		return cc && cc[0];
	}

    void s_cc::unlink(void)
    {
		TRACE_FLOW
        if (rc) {
            TRACE_INF("UNLINK")
	        --*rc;
	        if (*rc == 0) {
                TRACE_INFO("DELETE " << cc)
				ccrmv(cc);
	            delete rc;
	        }
        }
		cc = cnull;
		rc = 0;
    }

	const s_cc sdef(cdef);
	const s_cc snull(cnull);

//  ============================================================
//  class cbuffer

	cbuffer::cbuffer(unsigned s)
	: xbuffer(s)
	, bs(0)
	{
		TRACE_FLOW
		TRACE_VAR(this)
		TRACE_VAR(st)
	}

//	add a character
	void cbuffer::add(char c)
	{
		if (!c) return;
		if (bs > ds + 1 || alloc()) {
			db[ds] = c;
			++ds;
		}
	}

//	add a character string
	void cbuffer::add(t_cc c)
	{
		if (!c) return;
		add(c, strlen(c));
	}

//!	add a character string that is not null terminated
//!	with given size.
	void cbuffer::add(t_cc c, unsigned s)
	{
		if (bs > ds + s || alloc(s)) {
			memcpy(db + ds, c, s);
			ds += s;
		}
	}
//!	add an other cbuffer.
	void cbuffer::add(const cbuffer& cb)
	{
		if (cb.size()) add(cb.str(), cb.size());
	}

//!	add an unsigned int
	void cbuffer::add(unsigned i)
	{
		TRACE_FLOW
		TRACE_INF("cbuffer int")
		unsigned ic = i;
		unsigned l = 0;
		do { ++l; ic /= 10; } while (ic);
		TRACE_VAR(l);
		if (bs > ds + l || alloc(l)) {
			ic = l;
			do {
				--ic;
				*(db + ds + ic) = static_cast<char>('0' + i % 10);
				i /= 10;
			} while (ic);
			ds += l;
			TRACE_VAR(ds);
		}
	}
	void cbuffer::add(int i)
	{
		if (i < 0) add('-');
		unsigned u = abs(i);
		add(u);
	}

//	hand over contents to #s_cc container
//	and reset own contents to 0.
	void cbuffer::over(s_cc& sc, bool c)
	{
		TRACE_FLOW
		sc.copy(db, ds);
		if (c)	clear();
		else	clean();
	}

//!	shift contents (remove n bytes at front).
	void cbuffer::shift(unsigned n)
	{
		if (!n || !ds) return;
		if (n >= ds) clean();
		else {
			ds -= n;
			memmove(db, db + n, ds);
			memset(db + ds, 0, bs - ds);
		}
	}

//!	shift contents (bring last n bytes to front).
	void cbuffer::mshift(unsigned n)
	{
		if (n >= ds) return;
		shift(ds - n);
	}

//	clear contents
	void cbuffer::clear(void)
	{
		TRACE_FLOW
		if (bs) {
			free(db);
			db = 0;
		}
		bs = 0;
		ds = 0;
	}

//!	resize buffer (keep front part)
	cbuffer& cbuffer::resize(unsigned n)
	{
		if (n < ds) {
			memset(db + n, 0, bs - n);
			ds = n;
		}
		return *this;
	}

//!	clean contents without memory shrink
	cbuffer& cbuffer::clean(void)
	{
		TRACE_FLOW
		if (ds) {
			memset(db, 0, bs);
			ds = 0;
		}
		return *this;
	}

//	provide memory for following input
//	- position
//	- size
	bool cbuffer::alloc(unsigned s)
	{
		TRACE_FLOW
		TRACE_VAR(this)
		TRACE_VAR(s)
		unsigned ns = (
			s == 1 ?
			bs + st :
			((ds + s + 1) / st + 1) * st
		);
		db = reinterpret_cast<char*>(bs ? realloc(db, ns) : malloc(ns));
	//	set new bytes after old area (size) to zero
		memset(db + bs, 0, ns - bs);
		bs = ns;
		return true;
	}

//  ============================================================
//  class fbuffer

//	fast character buffer with fixed size
//	- does not control full state.
//	class fbuffer
	fbuffer::fbuffer(unsigned s)
	: mx(s)
	{
		db = ccprv(s + 1);
		dp = db;
	}

	fbuffer::~fbuffer() { ccrmv(db); }

	void fbuffer::add(char c)
	{
		*dp = c;
		++dp;
		*dp = 0;
		++ds;
	}
	void fbuffer::add(t_ch c)
	{
		for (; *c; ++c) {
			*dp = *c;
			++dp;
			++ds;
		}
		*dp = 0;
	}
	void fbuffer::clean(void)
	{
		db[0] = 0;
		dp = db;
		ds = 0;
	}
//  ============================================================
//  class sbuffer
	int ccomp(const void* p1, const void* p2)
	{
		TRACE_FLOW
		t_cc c1 = *reinterpret_cast<t_cc*>(p1);
		t_cc c2 = *reinterpret_cast<t_cc*>(p2);
		TRACE_ADDR(c1)
		TRACE_ADDR(c2)
		TRACE_VARQ(c1)
		TRACE_VARQ(c2)
		if (c1 && c2)  return strcmp(c1, c2);
		if (c1) return -1;
		if (c2) return  1;
		return 0;
	}

	sbuffer::sbuffer(unsigned s, bool sr)
	: dsb(s)
	, db(0)
	, bs(0)
	, srt(sr)
	, srtd(false)
	{
		TRACE_FLOW
		TRACE_VAR(this)
		TRACE_VAR(st)
	}

//	add a character string
	void sbuffer::add(t_cc c)
	{
		TRACE_FLOW
		TRACE_INF("sbuffer t_cc")
		TRACE_VAR(this)
		TRACE_VAR(c)
		if (
			!(srt && find(c)) &&
			(bs > ds || alloc())
		) {
			db[ds] = c;
			++ds;
			srtd = false;
			if (srt) sort();
		}
	}
//!	at
	t_cc sbuffer::operator[](unsigned p) const
	{
		TRACE_FLOW
		return (p < ds ? db[p] : cdef);
	}

//!	shift a value
	t_cc sbuffer::shift(void)
	{
		TRACE_FLOW
		if (!ds) return cdef;
		t_cc c = db[0];
		--ds;
		if (!ds) clean();
		else {
			unsigned char* p = reinterpret_cast<unsigned char*>(db);
			memmove(p, p + sizeof(t_ch), ds * sizeof(t_ch));
		}
		return c;
	}

//!	pop a value
	t_cc sbuffer::pop(void)
	{
		if (!ds) return cdef;
		t_cc c = db[ds - 1];
		--ds;
		if (!ds) clean();
		return c;
	}

//!	last value in stack
	t_cc sbuffer::last(void)
	{
		if (!ds) return cdef;
		return db[ds - 1];
	}
	bool sbuffer::operator==(t_cc c)
	{
		return (strcmp(last(), c) == 0);
	}

//!	clean contents without memory shrink
	void sbuffer::clean(void)
	{
		ds = 0;
	}
//	clear contents
	void sbuffer::clear(void)
	{
		if (bs) free(db);
		bs = 0;
		ds = 0;
	}

//	provide memory for following input
//	- position
//	- size
	bool sbuffer::alloc(void)
	{
		TRACE_FLOW
		TRACE_INF("sbuffer")
		TRACE_VAR(this)
		TRACE_VAR(bs)
		TRACE_VAR(this->st)
		const unsigned ns = bs + (st > 0 ? st : c_DEFAULT_CBUFF_STEP); //st;
		const unsigned ms = ns * sizeof(t_ch);
		TRACE_VAR(ns)
		TRACE_VAR(ms)
		db = reinterpret_cast<t_ch*>(bs > 0 ? realloc(db, ms) : malloc(ms));
		bs = ns;
		return true;
	}

//!	sort contents
	bool sbuffer::sort(void) const
	{
		if (!ds) return false;
		if (srtd) return true;
		TRACE_FLOW
		qsort(db, ds, sizeof(t_cc), &ccomp);
		srtd = true;
		return true;
	}

//!	check if key exists
	t_cc sbuffer::find(t_cc c, bool f) const
	{
		if (!c || !ds) return 0;
		if (f && !srtd) sort();
		if (!srtd) return 0;
		TRACE_FLOW
		void* pv = bsearch(&c, db, ds, sizeof(t_cc), &ccomp);
		return pv ? (*reinterpret_cast<t_cc*>(pv)) : 0;
	}


//  ============================================================
//  class csbuffer
	typedef const s_cc* const pc_s_cc;

	int sccomp(const void* p1, const void* p2)
	{
		TRACE_FLOW
		pc_s_cc ps1 = *reinterpret_cast<pc_s_cc*>(p1);
		pc_s_cc ps2 = *reinterpret_cast<pc_s_cc*>(p2);
		TRACE_ADDR(ps1)
		TRACE_ADDR(ps2)
		if (ps1 && ps2) {
			if (*ps1 && *ps2) return strcmp(*ps1, *ps2);
			if (*ps1) return -1;
			if (*ps2) return  1;
		}
		if (ps1) return -1;
		if (ps2) return  1;
		return 0;
	}

	csbuffer::csbuffer(unsigned s, bool sr)
	: dsb(s)
	, db(0)
	, bs(0)
	, srt(sr)
	, srtd(true)
	{ }

//!	add or copy an s_cc type.
	t_cc csbuffer::aoc(const s_cc& s)
	{
		TRACE_FLOW
		if (srt && !s) return 0;
		s_cc* ps = 0;
	//	find existing pointer if sorted data
		if (ds && srtd) {
			const s_cc* pf = &s;
			void* pv = bsearch(&pf, db, ds, sizeof(s_cc*), &sccomp);
			if (pv) ps = *reinterpret_cast<s_cc**>(pv);
		}
		if (!ps) {
		//	allocate new pointer
			if (bs > ds || alloc()) {
				ps = db[ds] = new s_cc(s);
				++ds;
				srtd = false;
				if (srt) isort();
			}
			else return 0;
		}
		return *ps;
	}

//	add a character string
	t_cc csbuffer::add(t_cc c)
	{
		TRACE_FLOW
		TRACE_INF("csbuffer t_cc")
		stmp = c;
		return aoc(stmp);
	}
//	add a character string
	t_cc csbuffer::copy(t_cc c)
	{
		TRACE_FLOW
		stmp.copy(c);
		return aoc(stmp);
	}
//	add a character string
	t_cc csbuffer::copy(t_cc c, unsigned l)
	{
		TRACE_FLOW
		stmp.copy(c, l);
		return aoc(stmp);
	}
//!	take value from cbuffer.
	t_cc csbuffer::take(cbuffer& cbf)
	{
		cbf.over(stmp);
		return aoc(stmp);
	}
//!	sort contents
	bool csbuffer::sort(bool s)
	{
		if (s) srt = true;
		return isort();
	}
//!	internal sort
	bool csbuffer::isort(void) const
	{
		if (!ds)  return false;
		if (srtd) return true;
		TRACE_FLOW
		qsort(db, ds, sizeof(s_cc*), &sccomp);
		srtd = true;
		return true;
	}

//!	check if key exists
	t_cc csbuffer::find(t_cc c, bool f) const
	{
		s_cc** ps = ifind(c, f);
		return ps ? (*ps)->str() : 0;
	}

//!	internal find
	s_cc** csbuffer::ifind(t_cc c, bool f) const
	{
		if (!ds || !c) return 0;
		if (f && !srtd) isort();
		if (!srtd) return 0;
		TRACE_FLOW
		s_cc sc = c;
		s_cc* pf = &sc;
		void* pv = bsearch(&pf, db, ds, sizeof(s_cc*), &sccomp);
		return pv ? reinterpret_cast<s_cc**>(pv) : 0;
	}

//!	remove a value.
	bool csbuffer::remove(t_cc c)
	{
		TRACE_FLOW
		if (!srt) return false;
		TRACE_VARQ(c)
		s_cc** ps = ifind(c, true);
		if (ps) {
			delete (*ps);
			*ps = 0;
			srtd = false;
			isort();
			--ds;
		}
		TRACE_VAR(ds)
		return ps;
	}

//!	at
	const s_cc& csbuffer::at(unsigned p) const
	{
		TRACE_FLOW
		return (p < ds ? *(db[p]) : snull);
	}

//!	pop a value
	void csbuffer::pop(void)
	{
		TRACE_FLOW
		if (!ds) return;
		s_cc** pd = db + (ds - 1);
		delete *pd;
		*pd = 0;
		--ds;
	}
//!	last value in stack
	const s_cc& csbuffer::last(void)
	{
		TRACE_FLOW
		return (ds ? *(db[ds - 1]) : snull);
	}

	bool csbuffer::operator==(t_cc c)
	{
		TRACE_FLOW
		return (strcmp(last(), c) == 0);
	}

	void csbuffer::clean(void)
	{
		s_cc** pd = db;
		for (unsigned i = ds; i--; ++pd) {
			delete *pd;
			*pd = 0;
		}
		ds = 0;
	}

//	clear contents
	void csbuffer::clear(void)
	{
		s_cc** pd = db;
		for (unsigned i = ds; i--; ++pd) {
			delete *pd;
		}
		if (bs) free(db);
		db = 0;
		bs = 0;
		ds = 0;
	}

//!	copy operator.
	void csbuffer::operator=(const csbuffer& o)
	{
		clear();
		// st  = o.st;
		srt = o.srt;
		if (!o.ds) return;
		s_cc** odb = o.db;
		for (int i = o.ds; i--;) {
			if (bs > ds || alloc()) {
				db[ds] = new s_cc(**odb);
				++ds;
				++odb;
			}
		}
		if (srt) isort();
	}


//	provide memory for following input
	bool csbuffer::alloc(void)
	{
		unsigned ns = bs + st;
		unsigned ms = ns * sizeof(s_cc*);
		db = reinterpret_cast<s_cc**>(bs ? realloc(db, ms) : malloc(ms));
		bs = ns;
		return true;
	}

//	============================================================
//	stype utilities

//!	split a const character string into parts to a an csbuffer.
	t_cc split(t_ch c1, csbuffer& sb, char ct, char cs)
	{
		TRACE_FLOW
		if (!c1) return 0;
//		t_ch c1 = c;
		while (*c1 && *c1 != ct) {
			while (*c1 == ' ' || *c1 == '\t' || *c1 == cs) ++c1;
			if (!*c1 || *c1 == ct) break;
			t_ch c2 = c1;
			if (*c1 == '"' || *c1 == '\'') {
			//	move both pointers to sign after "
			//	this can be anything: 0, ", blank, ...
				++c2;
			//	move c2 until " or 0
				while (*c2 && *c2 != *c1 && *c2 != ct) ++c2;
				++c1;
			}
			else {
			//	move c2 until blank or 0
				do { ++c2; } while (*c2 && *c2 != ' ' && *c2 != '\t' && *c2 != cs && *c2 != ct);
			}
		//	c2 is " or 0 but can be identical to c1
			if (c2 != c1) sb.copy(c1, c2 - c1);
			c1 = c2;
			if (*c1 && *c1 != ct) ++c1;
		}
	//	proceeded until defined terminator?
		if (*c1) ++ c1;
		return c1;
	}
//!	join.
	t_cc join(s_cc& sc, const csbuffer& sb, char cj)
	{
		cbuffer bf;
		for (unsigned i = 0; i <  sb.size(); ++i) {
			if (i) bf << cj;
			bf << sb[i];
		}
		bf.over(sc);
		return sc;
	}

//!	"grep": csbuffer to csbuffer:
//!	list all items which begin with given string.
	unsigned grep(csbuffer& trg, const csbuffer& src, t_cc k, unsigned l, bool gt)
	{
		TRACE_FLOW
		unsigned s = src.size();
		if(!s || !k) return 0;
		if (!l) l = strlen(k);
		TRACE_VARQ(k)
		TRACE_VAR(l)
		unsigned n = 0;
		for (unsigned i = 0; i < s; ++i) {
			const s_cc& sc = src[i];
			if (sc && !strncmp(sc, k, l) && (!gt || *(sc + l))) {
				trg.aoc(sc);
				++n;
			}
		}
		return n;
	}

//!	retrieve non quoted part of a quoted string.
	t_fcc unquote(t_ch c, bool b)
	{
		if (!c) return c;
		bool unq = false;
		while (*c == '"' || (b && (*c == ' ' || *c == '\t'))) { unq = true; ++c; }
		if (!*c) return cdef;
		t_ch c2 = c;
		while (*c2) ++c2;
		--c2;
		while (*c2 == '"' || (b && (*c2 == ' ' || *c2 == '\t'))) { unq = true; --c2; }
		if (!unq) return c;
		static s_cc sc;
		sc.copy(c, c2 - c + 1);
		return sc;
	}

//!	human formatting to #cbuffer 2: an unsigned number.
	void human(outstream& bf, unsigned i)
	{
		if (i > 1024) return human(bf, static_cast<double>(i));
		else bf.add(i);
	}

//!	human formatting to #cbuffer 1: a floating point.
	void human(outstream& bf, double d, t_ch ck)
	{
		static t_cc c1024 = " KMGTXXX";
		static t_cc chf   = "%0.2f";
		if (!ck) ck = c1024;
		unsigned nc = 0;
		while (d > 1024.0) {
			++nc;
			d /= 1024.0;
		}
		TRACE_VAR(nc)
		char* tmp = new char[8];
		sprintf(tmp, chf, d);
		bf << tmp << ' ' << ck[nc];
		delete tmp;
	}

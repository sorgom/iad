//| ============================================================
//! @file
//!
//! General base classes for character string mapping.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef CMAPPER_HPP
#define CMAPPER_HPP

#include "stypes.hpp"
#include "nob.hpp"

//	STL includes

	class cmapper;

	typedef void (cm_const_trav_func)(const s_cc&, const s_cc&);
	typedef void (cm_trav_func		)(s_cc&, s_cc&);

	class cm_const_traverser
	{
	public:
		virtual void operator()(const s_cc& k, const s_cc& v) = 0;
	};

	class cm_traverser
	{
	public:
		virtual void operator()(s_cc& k, s_cc& v) = 0;
	};

	typedef void (cm_load_func) (cmapper&);

	struct spair;

//! cstring to cstring (key to meaning) translator class.
//!	comparable to STL map.
    class cmapper : public dsb, public ssd
    {
    public:
	//!	constructor [with given expansion step size].
        cmapper(unsigned = 0, cm_load_func* = 0);

		inline ~cmapper() { clear(); }

	//!	string search device.
		t_cc find(t_cc k, bool f = false) const;

    //! the core method: get the meaning of a key.
    //! Does not return NULL pointer.
    //! Returns empty character string in case of not found.
        const s_cc& get(t_cc k) const;

	//! variation of core method:
    //! Return NULL pointer in case of not found.
        const s_cc& getn(t_cc k) const;

	//! convenience variation of core method:
	//!	search by NOB,
	//! Return NULL pointer in case of not found.
		const s_cc& getn(const nob&) const;

	//! variation of core method:
	//! Does not return NULL pointer.
    //! Returns key in case of not found.
		t_cc getk(t_cc k) const;

	//! variation of core method:
	//! Does not return NULL pointer.
    //! Returns default value d in case of not found.
		t_cc getd(t_cc k, t_cc d) const;

	//!	first key available,
	//!	returns 0 in case of not found.
		t_cc firstkey(void);

	//! set contents from s_cc, overwrite existing value.
    //! @param k key
    //! @param v value
    //! @param s instant search and sort
	//!	@return value
        t_cc set(const s_cc& k, const s_cc& v, bool s = true);

	//!	add contents from s_cc, do nothing if key already set.
    //! @param k key
    //! @param v value
	//!	@return value
        t_cc add(const s_cc& k, const s_cc& v);

	//!	general set with set / copy decision.
	//! @param k key
    //! @param v value
	//! @param s instant search and sort
    //!	@param ck copy key instead of set
    //!	@param cv copy value instead of set
	//!	@return value
		t_cc xset(t_cc k, t_cc v, bool s = true, bool ck = false, bool cv = false);

	//!	general add with set / copy decision (does not change existing values).
	//! @param k key
    //! @param v value
    //!	@param ck copy key instead of set
    //!	@param cv copy value instead of set
	//!	@return value
		t_cc xadd(t_cc k, t_cc v, bool ck = false, bool cv = false);

    //! set contents fro t_cc.
    //! @param k key
    //! @param v value
    //! @param s instant search and sort
	//!	@return value
        inline t_cc set(t_cc k, t_cc v, bool s = true)	{ return xset(k, v, s); }

	//! set contents from fluent t_cc.
	//! @param k key
    //! @param v value
    //! @param s instant search and sort
	//!	@return value
        inline t_cc copy(t_cc k, t_cc v, bool s = true) { return xset(k, v, s, true, true); }

	//!	set contents from key reference and fluent value.
	//! @param k key (reference)
    //! @param v value (fluent)
    //! @param s instant search and sort
	//!	@return value
		inline t_cc copyv(t_cc k, t_cc v, bool s = true) { return xset(k, v, s, false, true); }

    //! set contents from xbuffers.
    //! @param cbk key buffer
    //! @param cbv value buffer
    //! @param s instant search and sort
	//!	@return value
        inline t_cc set(const xbuffer& cbk, const xbuffer& cbv, bool s = true)
		{
			return xset(cbk, cbv, s, true, true);
		}

	//!	delete an entry.
	//!	@return true if key was found and removed
		bool remove(t_cc k);

	//!	delete all entries that don't exist in other string search device.
		void intersec(const ssd&);

    //! call sort after insertions done.
	//!	Otherwise map won't work.
	//! @param r instant size reduction
		void sort(bool r = false);

	//!	shrink memory to used size.
		void shrink(void);

    //! clear data contents.
        void clear(void);

	//!	clean memory without shrink.
		void clean(void);

	//!	traverse all data with given traverser.
		void traverse(cm_const_traverser&) const;

	//!	traverse all data with given traverser.
		void traverse(cm_traverser&);

	//!	traverse all data with given traverse function pointer.
		void traverse(cm_const_trav_func*) const;

	//!	traverse all data with given changing traverse function pointer.
		void traverse(cm_trav_func*);

	//!	list all keys to a csbuffer.
		unsigned keys(csbuffer&) const;

	//!	list all values to a csbuffer.
		unsigned values(csbuffer&) const;

	//!	"grep" to an sbuffer.
	//!	list all keys to a csbuffer which begin with given string.
		unsigned grep(csbuffer& sb, t_cc k, unsigned l = 0, bool gt = false) const;

	private:
        unsigned bs;
		spair** db;

	//!	asssigned loader function pointer
		cm_load_func* ldfc;
		mutable bool loaded;
	//!	internal find
		spair* findp(t_cc k) const;
	//!	sort state
		mutable bool srtd;
	//!	memory allocation
		bool alloc(void);
	//!	set / add temps
		static s_cc sck;
		static s_cc scv;
		static bool setkv(t_cc k, t_cc v, bool ck =false, bool cv = false);
    };

//	============================================================
//	cmapper extension functions

//!	for multiple value cmappers:
//!	retreive multiple values assigned to a key to an sbuffer reference.
	bool cmvals(sbuffer& sb, const cmapper& cm, t_cc k);

//!	cmapper value exchange class
	class cmxc
	{
	public:
	//!	only one instance available.
		static cmxc& instance(void);
	//!	set target, source, copy mode.
		const cmxc& set(cmapper& trg, const cmapper& src, bool cp, bool ow);
	//!	stream keys to copy.
		const cmxc& operator<<(t_cc) const;
	private:
	//!	def con.
		cmxc();
		const cmapper* cmsrc;
		cmapper* cmtrg;
		bool cpy;
		bool owr;
	};

//!	exchange values from cmapper to cmapper.
//!	e.g. cmx(target, source) << c_lang << c_lay << c_sid;
//!	@param trg target cmapper
//!	@param src source cmapper (unchanged)
//!	@param cp  copy value instead of reference
//!	@param ow  overwrite existing source value
	const cmxc& cmx(cmapper& trg, const cmapper& src, bool cp = false, bool ow = true);

//!	convenience: exchange values from cmapper to cmapper
//!	but do not overwrite existing values.
//!	e.g. cmxa(target, source) << c_lang << c_lay << c_sid;
//!	@param trg target cmapper
//!	@param src source cmapper (unchanged)
//!	@param cp  copy value instead of reference
	inline const cmxc& cmxa(cmapper& trg, const cmapper& src, bool cp = false)
	{
		return cmx(trg, src, cp, false);
	}

//!	convenience: swap keys and values.
//!	e.g. swap(trg, src);
//!	trg is not cleaned or cleared.
//!	@param trg target cmapper
//!	@param src source cmapper (unchanged)
//!	@return false if src and trg are identical, true otherwise
	bool cswap(cmapper& trg, const cmapper& src);


#endif // CMAPPER_HPP

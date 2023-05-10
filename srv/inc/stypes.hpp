//| ============================================================
//! @file
//!
//! Enhanced string processing types.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef STYPES_HPP
#define STYPES_HPP

#include "outstream.hpp"
#include "types.hpp"
#include <string.h>


//! default step size for buffer expansion
#ifndef c_DEFAULT_CBUFF_STEP
	#define c_DEFAULT_CBUFF_STEP 16
#endif

//	============================================================
//	frequently used stream types

//!	multi purpose stream terminator.
	struct tt_end {};
//!	multi purpose stream terminator instance.
	extern const tt_end tte;

//	============================================================

//!	buffer with data size prototype
	class dsb
	{
	 public:
	 	dsb(unsigned s = 0);
	//! data / empty.
        inline bool empty(void) const { return ds == 0; }
        inline bool data (void) const { return ds >  0; }
    //! size of contents
        inline unsigned  size(void) const { return ds; }
		inline operator bool() const { return ds > 0; }
	protected:
	//!	used data size.
		unsigned ds;
    //!	buffer expansion step size.
		const unsigned st;
	};

//!	char and string buffer prototype
	class xbuffer : public dsb
	{
    public:
		xbuffer(unsigned s = 0);
	//!	retrieve string content.
		inline t_cc str(void)	const { return db ? db : cdef; }
	//!	const char* behauviour.
		inline operator t_cc()	const { return str(); }
	//!	retrieve internal string for manipulation.
	//!	(use with caution)
		char* const ms(void)	{ return db; }
	//!	adapt xbuffer setting after internal string manipulation.
		unsigned rs(void);
	protected:
    //  the storage
        char* db;
	};

//!	string search device
	class ssd
	{
	public:
		virtual t_cc find(t_cc c, bool f = false) const = 0;
	};

//	============================================================
//	smart pointing

//! Const char smart pointer.
    class s_cc
    {
    public:
	//!	empty constructor.
        s_cc();
    //! constructor from const char* const
    //! (which must not be the temporary c_str() representation of std::string).
        s_cc(t_cc);

    //! constructor from xbuffer copying contents.
    //! should only be used for computed values.
        s_cc(const xbuffer&);

    //! destructor decides about deletion of contents by reference count.
        inline ~s_cc() { unlink(); }

    //! Copy operator from xbuffer.
        void operator=(const xbuffer&);

    //! Copy operator from const char* const.
        void operator=(t_cc);

	//!	explicit copy contents from unsave t_cc.
		void copy(t_cc);

	//!	explicit copy contents from non zero terminated
	//! (part) string with given part length.
		void copy(t_cc, unsigned);

    //! Copy constructor does not copy contents.
        s_cc(const s_cc&);

	//! Copy operator from s_cc
	 	void operator=(const s_cc&);

	//!	take over a hep seated string.
	//!	string is going to be deleted with destruction.
		void take(t_cc);

    //! can be used as a const char
    //! whithin scope of exitence.
        inline operator t_ch() const { return cc; }

	//!	for explicit cast.
		inline t_ch str(void) const { return cc; }

	//! mutable access to data
	//!	only given if based on copy, otherwise 0.
		char* ms(void) const;

	//!	mutable reference access.
		inline s_cc& mr(void) const { return const_cast<s_cc&>(*this); }

    //! Information about reference count.
    //! refs 0 means referring to built in t_cc.
    //! refs 1 or more means carrying 1 copy,
    //! which is referred to by refs objects.
        unsigned refs(void) const;

	//!	empty information.
		bool empty(void) const;

	//!	the opposit of empty.
		bool val(void) const;

    private:
        t_ch cc;
        unsigned* rc;
        void unlink(void);
    };

	extern const s_cc sdef;
	extern const s_cc snull;

//	============================================================
//	char and string buffers

//! character buffer for parsing purposes.
    class cbuffer : public xbuffer, public outstream
    {
    public:
	//!	constructor [with given expansion step size].
        cbuffer(unsigned s = 0);

	//!	dest.
		inline ~cbuffer() { clear(); }

    //! add a character.
        void add(char c);

    //! add a character string.
        void add(t_cc c);

	//!	add a character string that is not null terminated
	//!	with given size.
		void add(t_cc c, unsigned s);

	//!	add an other cbuffer.
		void add(const cbuffer&);

	//!	add an unsigned int.
		void add(unsigned i);

	//!	add an int.
		void add(int i);

	//!	hand over contents to #s_cc container
	//!	and reset own contents to 0.
		void over(s_cc&, bool = false);

	//!	shift contents (remove n bytes at front).
	//!	@param n number of bytes to be removed at front
		void shift(unsigned n);

	//!	shift contents (bring last n bytes to front).
	//!	@param n number of bytes to be moved to front
		void mshift(unsigned n);

	//!	resize buffer (keep front part)
		cbuffer& resize(unsigned);

    //! clear contents.
        void clear(void);

	//!	clean contents without memory shrink.
		cbuffer& clean(void);

    private:
    //! provide memory for following input
        bool alloc(unsigned = 1);
    //  current buffer size including zero termination
        unsigned bs;
    };

//!	fast character buffer with fixed size
//!	- does not control full state.
	class fbuffer : public xbuffer
	{
	public:
	//!	constructor with given size.
		fbuffer(unsigned s);
    //! add a character.
		void add(char);
	//!	add a string.
		void add(t_ch);
    //! the contents as character string.
		inline t_cc str (void) const { return db; }
	//!	full information.
		inline bool full(void) const { return ds >= mx; }
	//!	remaining space to be filled.
		inline unsigned  pot (void) const { return mx - ds; }
	//!	clean contents without memory shrink.
		void clean(void);

		~fbuffer();
		const unsigned mx;
	private:
		fbuffer();
		char* dp;
	};

//! character string buffer (referencing).
	class sbuffer : public dsb, public ssd
    {
    public:
	//!	constructor [with given expansion step size].
        sbuffer(unsigned s = c_DEFAULT_CBUFF_STEP, bool sr = false);

	//!	dest.
		inline ~sbuffer() { clear(); }

	//! add a character string.
        void add(t_cc c);

	//!	add a character string streamed.
		inline sbuffer& operator<<(t_cc c) { add(c); return *this; }

	//!	value at position.
		t_cc operator[](unsigned p) const;

	//!	shift a value.
		t_cc shift(void);

	//!	pop a value.
		t_cc pop(void);

	//!	last value in stack.
		t_cc last(void);
		bool operator==(t_cc);

    //! clear contents.
        void clear(void);

	//!	clean contents without memory shrink.
		void clean(void);

	//!	sort contents.
		bool sort(void) const;

	//!	check if key exists.
		t_cc find(t_cc c, bool f = false) const;
    private:
    //! provide memory for following input.
        bool alloc(void);
    //  the storage.
        t_ch* db;
    //  current buffer size.
        unsigned bs;
	//	sort tracker
		bool srt;
		mutable bool srtd;
    };

//! character string buffer (copying).
	class csbuffer : public dsb, public ssd
    {
    public:
	//!	constructor [with given expansion step size].
        csbuffer(unsigned s = c_DEFAULT_CBUFF_STEP, bool sr = true);

	//!	dest.
		inline ~csbuffer() { clear(); }

	//!	add or copy an s_cc type.
		t_cc aoc(const s_cc&);

	//! add a character string by reference.
        t_cc add(t_cc c);

	//!	add a character string streamed.
		inline csbuffer& operator<<(t_cc c) { add(c); return *this; }

	//! add a character string by value.
        t_cc copy(t_cc c);

	//!	take value from cbuffer.
		t_cc take(cbuffer&);

	//! add a non zero terminated character string by value with given length.
		t_cc copy(t_cc c, unsigned l);

	//!	sort contents and switch on sort mode.
		bool sort(bool = true);

	//!	check if key exists.
		t_cc find(t_cc c, bool f = false) const;

	//!	value at position.
		const s_cc& at(unsigned p) const;
	//!	value at position the array way.
		inline const s_cc& operator[](unsigned p) const
		{ return at(p); }

	//!	pop a value.
		void pop(void);

	//!	last value in stack.
		const s_cc& last(void);
		bool operator==(t_cc);

	//!	remove a value.
	//!	does not do anything if in non sort mode.
	//!	@return true if value was found and removed
		bool remove(t_cc);

    //! clear contents.
        void clear(void);

	//!	clean contents without memory shrink.
		void clean(void);

	//!	copy operator.
		void operator=(const csbuffer&);

    private:
    //! provide memory for following input
        bool alloc(void);
    //! the storage
        s_cc** db;
    //! current buffer size
        unsigned bs;
	//! sort behaviour.
		bool srt;
		mutable bool srtd;
	//!	unused data size
		int uds;
	//!	copier temp s_cc.
		s_cc stmp;
	//!	internal find
		s_cc** ifind(t_cc c, bool f = false) const;
	//!	internal sort
		bool isort(void) const;
    };

//! convenience: non sorting character string buffer (copying).
	class ncsbuffer : public csbuffer
    {
    public:
	//!	constructor [with given expansion step size].
        inline ncsbuffer(unsigned s = c_DEFAULT_CBUFF_STEP)
		: csbuffer(s, false)
		{}
		inline ~ncsbuffer() { clear(); }
	};

//	============================================================
//	stype utilities

//!	split a const character string into parts to a csbuffer.
	t_cc split(t_ch c, csbuffer& sb, char ct = 0, char cs = ',');

//!	join.
	t_cc join(s_cc&, const csbuffer& sb, char cj = ',');

//!	"grep": csbuffer to csbuffer:
//!	list all items which begin with given string.
	unsigned grep(csbuffer& trg, const csbuffer& src, t_cc k, unsigned l = 0, bool x = false);

//!	retrieve non quoted part of a quoted string.
//!	@param b cut leading and tailing blanks
	t_fcc unquote(t_ch, bool b = true);

//!	human formatting to #cbuffer 1: a floating point.
	void human(outstream&, double, t_ch = 0);

//!	human formatting to #cbuffer 2: an unsigned number.
	void human(outstream&, unsigned);


#endif // _H

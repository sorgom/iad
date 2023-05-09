//| ============================================================
//! @file
//!
//! Name object nob.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef NOB_HPP
#define NOB_HPP

#include "outstream.hpp"
#include "stypes.hpp"

//	STL includes
//	#include <iostream>

	class nob
	{
	public:
		inline nob()
		: parent(0)
		{}
		inline nob(const nob& p)
		: parent(&p)
		{}

		virtual void out(outstream& os) const  = 0;

	//!	the core nob rule: put out own contents and next higher.
		void put(outstream& os) const;

		const nob* const parent;
	};

	inline outstream& operator<<(outstream& os, const nob& x)
	{
		x.put(os);
		return os;
	}


//! A nob has a name an protentially a parent nob.
//! It can write its name.
    class cnob : public nob
    {
    public:
    //! Constructor without name.
        inline cnob()
		: nm(cdef)
		{}
    //! Constructor with name.
        inline cnob(t_cc n)
		: nm(n)
		{}
    //! Constructor with with parent reference and name.
        inline cnob(const nob& p, t_cc n = cdef)
		: nob(p)
		, nm(n)
		{}
    //!	change name.
		inline void operator=(t_cc n) { nm = n; }
		inline void out(outstream& os) const { os.add(nm); }
		inline operator const nob&() const { return *this; }

	protected:
		const char* nm;
    };

	inline outstream& operator<<(outstream& os, const cnob& x)
	{
		x.put(os);
		return os;
	}


	class inob : public nob
    {
    public:
    //! Constructor with number.
        inline inob(t_num n = 0)
		: nn(n)
		{}
    //! Constructor with parent reference and number.
        inob(const nob& p, t_num n = 0)
		: nob(p)
		, nn(n)
		{}
	//!	change number.
		inline void operator=(t_num n) { nn = n; }
	//!	Count up
		inline inob& operator++(void) { ++nn; return *this; }
		inline inob& operator()(void) { ++nn; return *this; }

		inline void out(outstream& os) const { os.add(nn); }
		inline operator const nob&() const { return *this; }

	protected:
		t_num nn;
	};

	inline outstream& operator<<(outstream& os, const inob& x)
	{
		x.put(os);
		return os;
	}

	class cinob : public nob
    {
    public:
    //! Constructor with name and number.
        inline cinob(t_cc c = cdef, t_num n = 0)
		: cc(c)
		, nn(n)
		{}
    //! Constructor with parent reference and number.
        cinob(const nob& p, t_cc c = cdef, t_num n = 0)
		: cc(c)
		, nn(n)
		, nob(p)
		{}
		inline void operator=(t_num n) { nn = n; }
		inline cinob& operator++(void) { ++nn; return *this; }
		inline cinob& operator()(void) { ++nn; return *this; }
		inline void out(outstream& os) const {
			os.add(cc);
			os.add(nn);
		}
	protected:
		const char* cc;
		t_num nn;
    private:
        cinob();
	};

	inline outstream& operator<<(outstream& os, const cinob& x)
	{
		x.put(os);
		return os;
	}

#endif // _HPP

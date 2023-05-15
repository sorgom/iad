//| ============================================================
//! @file
//!
//!	Content processor class -
//!	the core class of modular content processing.
//!	And subsidiary features.
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef CPROC_H
#define CPROC_H

#include "stypes.h"
#include "db.h"

//	============================================================
//	cproc & commonly used derivates
	class setcproc;

//!	Content processor class - the core class of modular content processing.
//!	All content processors are to be be derived from this class.
    class cproc
    {
    public:
		inline cproc()
		: cpnm(cdef)
		{}
		virtual ~cproc() {}
	//!	Information whether content can be produced at all.
	//!	Used for HTML parser decisions.
	//!	This information is usually gathered with construction.
		inline virtual bool ok(sbuffer&) { return true; }
	//!	Output of content to stream with arguments.
		inline virtual void put(sbuffer&) {}
	//!	set an assigned switch or value.
		inline virtual void set(sbuffer&) {}
	//!	if a reset is neccessary define this.
	//!	reset will be called in server version
	//!	before each html request processing.
		inline virtual void reset() {}
	//!	what's my name? information can be used to set issues etc.
		t_cc cpnm;
	private:
    };

//!	a cproc derivate spicialized in switch of bool.
	class bswitch : public cproc
	{
	public:
		virtual ~bswitch() {}
		inline bswitch(bool& b)
		: rb(b)
		{}
		void set(sbuffer&);
		inline virtual bool ok(sbuffer&) { return rb; }
	private:
		bool& rb;
		bswitch();
	};

//!	a cproc derivate spicialized in readonly of bool.
	class binfo : public cproc
	{
	public:
		virtual ~binfo() {}
		inline binfo(const bool& b)
		: rb(b)
		{}
	//!	set is blocked.
		inline void set(sbuffer&) {}
		inline bool ok(sbuffer&) { return rb; }
	//!	puts out (translation of) "Yes" / "No".
		void put(sbuffer&);
	private:
		const bool& rb;
		binfo();
	};

//!	a cproc derivate spicialized in switch char.
	class cswitch : public cproc
	{
	public:
		virtual ~cswitch() {}
		inline cswitch(char& c)
		: rc(c)
		{}
		void set(sbuffer&);
	private:
		char& rc;
		cswitch();
	};

//!	a cproc derivate spicialized in switch int.
	class iswitch : public cproc
	{
	public:
		virtual ~iswitch() {}
		inline iswitch(int& i)
		: ri(i)
		{}
		void set(sbuffer&);
	private:
		int& ri;
		iswitch();
	};

//!	a cproc derivate spicialized in switch usigned int.
	class uswitch : public cproc
	{
	public:
		virtual ~uswitch() {}
		inline uswitch(unsigned& i)
		: ri(i)
		{}
		void set(sbuffer&);
	private:
		unsigned& ri;
		uswitch();
	};

//!	cproc derivate spicialized in switch character strings.
	class sswitch : public cproc
	{
	public:
		virtual ~sswitch() {}
		inline sswitch(s_cc& s)
		: rs(s)
		, sd(s)
		{}
		void set(sbuffer&);
	private:
		s_cc& rs;
		s_cc sd;
		sswitch();
	};


namespace db { class proc; }

//!	a standard cproc for database access.
	class db_cproc : public cproc
	{
	public:
		virtual ~db_cproc() {}
		inline db_cproc()
		: db(db::proc::instance())
		{}
		inline virtual bool ok(sbuffer&) { return db_check(); }
	protected:
		bool db_check();
		db::proc& db;
	private:
		static bool db_chkd;
		static bool db_ok;
	};

//!	a cproc enhancer: checks for issue, and parameters "0" or "1".
	class cp_splitter
	{
	protected:
		bool bissue;
		bool bout;
		bool split(t_cc, sbuffer&);
	};

//! a cproc enhancer: protects from check repetition.
	class cp_once
	{
	protected:
		inline cp_once()
		: once_chkd(false)
		, once_ok(false)
		{}
		virtual bool my_check(sbuffer&) = 0;
		bool check_once(sbuffer&);
		inline void reset_once() { once_chkd = false; }
	private:
		bool once_chkd;
		bool once_ok;
	};

#endif // CPROC_H

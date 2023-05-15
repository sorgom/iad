//| ============================================================
//! @file
//!
//! Cproc sample collection 2.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef CP_SAMPLES_TIME_H
#define CP_SAMPLES_TIME_H

#include "cproc.h"
#include "cp_date_time.h"
#include "stypes.h"
#include <ctime>

//!	a simple sample time processor.
//!	does not change system time but saves time difference to system time in database table.
	class fake_time : public time_processor, private db_cproc
	{
	public:
		static fake_time* instance();
		time_t get_time();
		void   set_time(time_t);
		inline void reset() { loaded = false; }
	private:
		fake_time();
		bool loaded;
		int idiff;
	};


//!	a sample time processor cproc.
	class cp_fake_time : public time_setter
	{
    public:
		cp_fake_time();
	};

//!	a sample time servers input with database support #cproc.
	class cp_time_servers : public db_cproc, public cp_splitter
	{
    public:
	//!	Output of content to stream with arguments.
		void put(sbuffer&);
	};

//!	a sample cproc: puts out currently active time server.
	class cp_time_server : public db_cproc, private cp_once
	{
    public:
		cp_time_server();
	//!	returns true if a time server is active.
		inline bool ok(sbuffer& sb) { return check_once(sb); }
	//!	puts out current time server name.
		void put(sbuffer&);
	//!	reset 'once'
		inline void reset() { reset_once(); }
	protected:
		bool my_check(sbuffer&);
	private:
		s_cc name;
	};

//!	a simple hh:mm:ss clock display.
	class cp_clock : public db_cproc
	{
    public:
	//!	Output of content to stream with arguments.
		void put(sbuffer&);
	};

#endif // CP_SAMPLES_TIME_H

//| ============================================================
//! @file
//!
//! Errors and infos storage and output.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef ERR_INF_H
#define ERR_INF_H

#include "stypes.h"
#include "cproc.h"
#include "sfunc.h"

	class msg_cproc;

	class msg_tainer
	{
	public:
		msg_tainer();
		template <class T> inline
		msg_tainer& operator<<(T t) 	{ bm.add(t); return *this; }
		msg_tainer& operator<<(tt_end);
		msg_tainer& operator<<(sfunc*);
	private:
		friend class msg_cproc;
		cbuffer 	bm;
		ncsbuffer 	bms;
	};

	extern msg_tainer infos;
	extern msg_tainer errors;

	class msg_cproc : public cproc
	{
	public:
		inline msg_cproc(msg_tainer& r)
		: rmt(r)
		{}
		inline bool ok(sbuffer&) { return rmt.bms.size() > 0; }
		void put(sbuffer&);
	private:
		msg_cproc();
		msg_tainer& rmt;
	};

	class cp_infos : public msg_cproc
	{
	public:
		inline cp_infos()
		: msg_cproc(infos)
		{}
	};

	class cp_errors : public msg_cproc
	{
	public:
		inline cp_errors()
		: msg_cproc(errors)
		{}
	};

#endif // ERR_INF_H

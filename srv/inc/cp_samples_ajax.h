//| ============================================================
//! @file
//!
//! Ajax updater content processors.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef CP_SAMPLES_AJAX_H
#define CP_SAMPLES_AJAX_H

#include "cproc.h"
#include "vars.h"

//! check preconditions for ajax updater.
	class cp_ajax_able : public cproc
	{
	public:
		inline bool ok(sbuffer&) { return curr_sid; }
	};

//! check preconditions for ajax updater.
	class cp_is_ajax : public cproc
	{
	public:
		inline bool ok(sbuffer&) { return is_ajax; }
	};

	class pre_ajax : public cproc
	{
	protected:
		pre_ajax() {}
		bool pre();
		static s_cc ajx;
		bool ok;
	};


//!	begin an ajax refresh <CONTAINER id=ajaxNNN> #cproc.
//!	puts out the container only if not ajax request.
//!	default container is div.
	class cp_begin_ajax : public pre_ajax
	{
	public:
		void put(sbuffer&);
	};
//! close ajax container #cproc.
	class cp_end_ajax : public pre_ajax
	{
	public:
		void put(sbuffer&);
	};

//!	close current ajax refresh container if open and create periodical update statement.
	class cp_ajax_updater : public pre_ajax
	{
	public:
		void put(sbuffer&);
	};

#endif // CP_SAMPLES_AJAX_H

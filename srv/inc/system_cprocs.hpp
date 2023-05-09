//| ============================================================
//! @file
//!
//! Language and Layout selector cprocs.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef SYSTEM_CPROCS_HPP
#define SYSTEM_CPROCS_HPP

#include "cproc.hpp"
#include "cmapper.hpp"
#include "vars.hpp"

//!	poly layout allowed info cproc.
	class cp_poly : public binfo
	{
	public:
		inline cp_poly()
		: binfo(poly)
		{}
	};

//!	layout (brand) selector cproc.
	class cp_layout : public cproc, public cm_const_traverser
	{
	public:
		void put(sbuffer&);
		void operator()(const s_cc& k, const s_cc& v);
	};

//!	layout (brand) selector cproc.
	class cp_quick_layout : public cproc, public cm_const_traverser
	{
	public:
		void put(sbuffer&);
		void operator()(const s_cc& k, const s_cc& v);
	private:
		t_ch c_sep;
		bool sep;
		bool unique;
	};

//!	language selector cproc.
	class cp_language : public cproc, public cm_const_traverser
	{
	public:
		void put(sbuffer&);
		void operator()(const s_cc& k, const s_cc& v);
	};

//!	languages quick link list generator cproc.
	class cp_quick_language : public cproc, public cm_const_traverser
	{
	public:
		void put(sbuffer&);
		void operator()(const s_cc& k, const s_cc& v);
	private:
		t_ch c_sep;
		bool sep;
	};

//!	login cproc.
	class cp_login : public cproc
	{
	public:
		void put(sbuffer&);
	};

//!	language literal defined cproc.
	class cp_LL : public cproc
	{
	public:
		bool ok(sbuffer&);
	};

//!	runtime switch get & set cproc: checkbox.
	class cp_rtv : public cproc
	{
	public:
		bool ok(sbuffer&);
		virtual void put(sbuffer&);
		void set(sbuffer&);
	};

//!	runtime switch get & set cproc: one click.
	class cp_rtv1 : public cp_rtv
	{
	public:
		void put(sbuffer&);
	};

//!	runtime switches display: translated names listing.
	class cp_rtv_info : public cp_rtv
	{
	public:
		void put(sbuffer&);
	};
//!	runtime switches display: spans with class.
	class cp_rtv_ico : public cp_rtv
	{
	public:
		void put(sbuffer&);
	};

//!	variable defined cproc.
	class cp_DEF : public cproc
	{
	public:
		bool ok(sbuffer&);
	};

//!	always true cproc.
	class cp_true : public cproc
	{
	public:
		inline bool ok(sbuffer&) { return true; }
	};

//!	always false cproc.
	class cp_false : public cproc
	{
	public:
		inline bool ok(sbuffer&) { return false; }
	};

//!	compiler macro DEVEL cproc.
	class cp_DEVEL : public cproc
	{
	public:
		inline bool ok(sbuffer&) {
#ifdef DEVEL
			return true;
#else
			return false;
#endif
		}
	};
//!	compiler macro ISHOST cproc.
	class cp_HOST : public cproc
	{
	public:
		inline bool ok(sbuffer&) {
#ifdef ISHOST
			return true;
#else
			return false;
#endif
		}
	};

//!	compiler macro ISSRV cproc.
	class cp_SRV : public cproc
	{
	public:
		inline bool ok(sbuffer&) {
#ifdef ISSRV
			return true;
#else
			return false;
#endif
		}
	};

//!	this variable defines whether a help assigned to a page
//!	should be displayed if parameters are set
//!	and no help for page + parameter availablbe.
	extern bool phdef;

//!	the switch cproc for #page_help_default.
	class cp_page_help_default : public bswitch
	{
	public:
		inline cp_page_help_default()
		: bswitch(phdef)
		{}
	};

//!	page help cproc.
	class cp_page_help : public cproc, private cp_once
	{
	public:
		inline bool ok(sbuffer& sb) { return check_once(sb); }
		void put(sbuffer&);
		inline void reset(void) { reset_once(); }
	protected:
	//!	check once: help available.
		bool my_check(sbuffer&);
	private:
		t_ch ch;
	};

//!	pages path display cproc.
	class cp_page_path : public cproc
	{
	public:
		bool ok(sbuffer&);
		void put(sbuffer&);
	private:
		t_cc pgt(t_cc);
	};

//!	dcp current link switcher and current evaluator.
	class cp_current : public bswitch
	{
	public:
		inline cp_current()
		: bswitch(dcp_current)
		{}
	//	special behaviour for ok:
	//	with parameter(s): check if all of them are set current,
	//	without just return dcp_current is set.
		bool ok(sbuffer&);
	};

//!	system reset cproc.
	class cp_sysreset : public cproc
	{
	public:
		bool ok(sbuffer&);
		void put(sbuffer&);
	};


//!	the prompt cproc.
//	USGAE
//	<form>
//		IF(prompt)
//			your own prompt stuff
//			a submit button with name "_YES_"
//		ELSE()
//			your current stuff
//			a submit button
//		ENDIF()
//	</form>

	class cp_prompt : public cproc, public cm_const_traverser
	{
	public:
		bool ok(sbuffer&);
	//!	convnience: writes "Yes" and "No" submit buttons.
		void put(sbuffer&);
	//	traverser
		void operator()(const s_cc& k, const s_cc& v);
	};

#endif // SYSTEM_CPROCS_HPP

//| ============================================================
//! @file
//!
//! Macro colletion for tracing and debugging
//! DO NOT INCLUDE WITHIN OTHER HEADER FILES.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef TRACE_MACROS_H
#define TRACE_MACROS_H

//	compiler option -DDEVEL

#ifndef c_TRACE_VAR_WIDTH
	#define c_TRACE_VAR_WIDTH 16
#endif

#if defined(DEVEL) || defined (XDEVEL)

#ifndef DEVEL
	#define DEVEL
#endif

	#include <iostream>
	#include <iomanip>
//	#include <typeinfo>
	#include "ccs.hpp"
	#include "trace_stypes.hpp"

//!	trace flow helper class.
	class cTRC
	{
	public:
		inline cTRC(const char* const c, std::ostream& o = std::cerr)
		: msg(c)
		, os(o)
		{ os << std::endl << msg_trc_enter << msg << std::endl;}
		inline ~cTRC() { os << msg_trc_exit << msg << std::endl;}
	private:
		cTRC();
		const char* const msg;
		std::ostream& os;
	};
//!	function / method ENTER / EXIT tracing.
	#define TRACE_FLOW cTRC __tmp_TRC__( __func__ );
//!	add something to trace without line break.
	#define TRACE_ADD(msg) std::cerr << msg;
//!	standard var begin.
	#define TRACE_VAR1(var) std::setw(c_TRACE_VAR_WIDTH) << std::left << #var << msg_info
//!	quoted string / NULL tracing.
	#define TRACE_VARQ(var) { std::cerr << TRACE_VAR1(var); if (var) std::cerr << '"' << var << '"'; else std::cerr << uc_null; std::cerr << std::endl; }
#else
	#define TRACE_VAR1(var)
	#define TRACE_FLOW
	#define TRACE_ADD(msg)
	#define TRACE_VARQ(var)
#endif

//!	trace output of whatever with line break.
#define TRACE_INF(msg)  TRACE_ADD(msg << std::endl)
//!	trace output of whatever with function / method name + line break.
#define TRACE_INFO(msg) TRACE_INF(__func__ << msg_info << msg)
//!	all kind variable "name: value" tracing.
#define TRACE_VAR(var) TRACE_INF(TRACE_VAR1(var) << var)
//!	boolean variable "name: false / true" tracing.
#define TRACE_VARB(var) TRACE_INF(TRACE_VAR1(var) << std::boolalpha << var << std::noboolalpha)
//!	variable address "name: 0x...." tracing.
#define TRACE_ADDR(var) TRACE_INF(TRACE_VAR1(var) << static_cast<const void* const>(var))
//!	error tracing.
#define TRACE_ERR(msg)  TRACE_INF(__func__ << msg_info << msg_error << msg)
//!	experimental: don't use.
#define TRACE_CLASS


// #define P_ADDR(c) msg_trc_address << static_cast<const void* const>(c)



//	compiler option -DISHOST

#ifdef ISHOST_XXX
//	dcp page tracing to dml output: begin.
	#define TRACE_PAGE_BEGIN(page) dml << c_com_open << ' ' << c_page << ' '  << page << ' ' << c_com_close << cnl;
//	dcp page tracing to dml output: end.
	#define TRACE_PAGE_END(page) dml << c_com_open << ' ' << c_end << ' '  << page << ' ' << c_com_close << cnl;
#else
	#define TRACE_PAGE_BEGIN(page)
	#define TRACE_PAGE_END(page)
#endif

#endif // _H

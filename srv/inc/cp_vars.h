//| ============================================================
//! @file
//!
//! Content processor switches.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef CP_VARS_H
#define CP_VARS_H

#include "cproc.h"
#include "vars.h"

//!	htm xml behaviour switcher
	class cp_xml : public bswitch
	{
	public:
		inline cp_xml()
		: bswitch(htm_xml)
		{}
	};

//!	htm attribute quotes switcher.
	class cp_quotes : public bswitch
	{
	public:
		inline cp_quotes()
		: bswitch(htm_quotes)
		{}
	};
//!	table autoclass switcher.
	class cp_autoclass : public bswitch
	{
	public:
		inline cp_autoclass()
		: bswitch(table_autoclass)
		{}
	};

//!	real floating point sign switch.
	class cp_fpoint : public cswitch
	{
	public:
		inline cp_fpoint()
		: cswitch(real_fpoint)
		{}
	};

//!	real number of digits (after floating point) switch
	class cp_fdigits : public iswitch
	{
	public:
		inline cp_fdigits()
		: iswitch(real_digits)
		{}
	};

#endif // _H

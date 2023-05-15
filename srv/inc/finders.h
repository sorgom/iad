//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef FINDERS_H
#define FINDERS_H

#include "stypes.h"
#include "f_io.h"
#include "dirman.h"

//!	available languages lister #cproc.
//!	a language is available if common .dat file exists.
	class languagefinder : public csbuffer, public dircaller
	{
	public:
	//!	contructor (with init call included).
		languagefinder(bool c = false);
	//!	directory lister callback.
		void operator()(t_cc);
	//!	init call.
		void operator()();
	private:
	//!	length of "common" part string.
		const unsigned lc;
	};


//!	layout existance is defined by existance of subdirectories
//!	under ini defined htmdir.
//!	the "common" directory is not listed.
	class layoutfinder : public csbuffer, public dircaller
	{
	public:
	//!	contructor (with init call included).
		layoutfinder(bool c = false);
	//!	directory lister callback.
		void operator()(t_cc);
	//!	init call.
		void operator()();
	};

//!	htm file finder.
	class htmfinder : public dirman
	{
	public:
		static const htmfinder* const instance();
		bool init();
	private:
		htmfinder() {}
	};

#endif // FINDERS_H

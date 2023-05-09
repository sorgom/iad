//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef FINDERS_HPP
#define FINDERS_HPP

#include "stypes.hpp"
#include "f_io.hpp"
#include "dirman.hpp"

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
		void operator()(void);
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
		void operator()(void);
	};

//!	htm file finder.
	class htmfinder : public dirman
	{
	public:
		static const htmfinder* const instance(void);
		bool init(void);
	private:
		htmfinder() {}
	};

#endif // FINDERS_HPP

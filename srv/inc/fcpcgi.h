//| ============================================================
//! @file
//!
//! #cproc retrieval and assignment CGI interface.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef FCPCGI_H
#define FCPCGI_H

#include "cmapper.h"

	class cproc;

//!	#cproc instance creater function type.
	typedef cproc* (ccproc)();

	class fcpair;

//	class setfcpcgi;

//!	Content processor keyword assignment registry.
	class fcpcgi : public dsb
	{
	public:
	//!	find and process: ok.
		bool ok(t_cc) const;
	//!	find and process: ok bulk.
		bool ok(sbuffer&) const;
	//!	find and process: put.
		void put(sbuffer&) const;
	//!	find and process: set.
		void set(sbuffer&) const;
	//! clear all instanciated cprocs.
		void clear();
	//!	Assign a processor creator to a keyword.
		void set(t_cc, ccproc*);
	//!	finalize assignment.
		void sort();
	//!	instance creator.
		static const fcpcgi& instance();
	//! reset all instanciated cprocs (dummy).
		void reset() const {}
	private:
		fcpcgi();
	//! provide memory for following input
        bool alloc();
    //  the storage
        fcpair** db;
    //  current buffer size
        int bs;
    //  buffer expansion step size
        int st;
	//!	internal find
		fcpair* find(t_cc k) const;
	};

//!	the commonly accessible #fcpcgi instance.
	extern const fcpcgi& fcp;

#endif // FCPCGI_H

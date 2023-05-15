//| ============================================================
//! @file
//!
//! We want to save some htm file typing:
//!	make some #cproc for that.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef CP_SAMPLES_HTM_H
#define CP_SAMPLES_HTM_H

#include "cproc.h"

/*	CPROC	cp_JS
	PUT		javascript-file name without extension
	SAMPLE	PUT(JS /prototype)
*/

//!	a javascript file loader #cproc.
//!	<script type="text/javascript" src="PARAMETER.js"></script>
//!	-> PUT(JS PARAMETER)
	class cp_JS : public cproc
	{
	public:
		void put(sbuffer&);
	};

	class cp_JE : public cproc
	{
	public:
		void put(sbuffer&);
	};

	class cp_JWS : public cproc
	{
	public:
		void put(sbuffer&);
	};

	class cp_JWE : public cproc
	{
	public:
		void put(sbuffer&);
	};


#endif // CP_SAMPLES_HTM_H

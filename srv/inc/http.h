//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef HTTP_H
#define HTTP_H

#include "stypes.h"
#include "cmapper.h"

#include <ctime>

	class rtok;

//!	tokenized request processor class prototype.
	class rproc
	{
	private:
		friend class rtok;
	//!	process http request data.
		inline virtual bool process(const cmapper&) { return true; }
	};

//!	request state and data class.
	class rdata
	{
	public:
	//!	data state.
		int state;
	//!	unfinished header buffer.
		cbuffer buf;
	//!	cmapper containing request parameters.
		cmapper cmr;
	//!	content length.
		unsigned cl;
	//!	def con.
		rdata();
	};

//!	request tokenizer class.
	class rtok
	{
	public:
	//!	con with assigned processor.
		rtok(rproc* rp);
	//!	process incoming string from TCP stream.
	//!	this can be
	//!	a complete request,
	//!	a part of a request,
	//!	a continued part of a request,
	//!	multiple requests.
		bool process(rdata*, t_ch);
	private:
	//!	assigned processor.
		rproc* proc;
	//!	no def con.
		rtok();
	//!	tokenize assembled string.
		bool tokenize(rdata&);
	//!	call processor.
		bool callpr(rdata&);
	};

//!	http header formatted date.
//!	"Sun, 06 Nov 1994 08:49:37 GMT" RFC 822, updated by RFC 1123
	t_fcc hdate(time_t);

//!	etag encoding of whatever sort of string.
//!	(16 character MD4 hex string)
	t_fcc etag(t_cc c, unsigned l = 0);


#endif // HTTP_H

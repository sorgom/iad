//| ============================================================
//! @file
//!
//!  http headers for different http responses.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef HTTH_H
#define HTTH_H

#include "stypes.h"

#include <ctime>

namespace htth
{

//!	set general parameters for
//!	server name,
//!	default content type,
//!	minimum timeout,
//!	maximum timeout.
	void defaults(t_cc svn = 0, t_cc ctp = 0, unsigned tm = 15, unsigned mx = 100);


//	HTTP/1.x 200 OK
//	Date: Sun, 29 Nov 2009 20:15:14 GMT
//	Server: Server-Name
//	Cache-Control: no-store
//	Connection: Keep-Alive
//	Keep-Alive: timeout=15 max=100
//	Content-Length: 2276
//	Content-Type: text/htm

//!	a standard reply header for generated content.
//!	includes "Cache-Control: no-store"
//!	@param l	content length
//!	@param cec	content encoding (optional)
	const cbuffer& reply(unsigned l, t_cc cec = 0);


//!	a standard text plain header for generated content.
//!	includes "Cache-Control: no-store"
//!	@param l	content length
//!	@param cec	content encoding (optional)
	const cbuffer& txt(unsigned l, t_cc cec = 0);

//	HTTP/1.x 200 OK
//	Date: Mon, 30 Nov 2009 11:10:12 GMT
//	Server: Apache/2.2.12 (Ubuntu)
//	Last-Modified: <mtm>
//	Etag: <etg>
//	? Accept-Ranges: bytes
//	? Vary: Accept-Encoding
//	[ Content-Encoding: <cec> ]
//	Content-Length: <l>
//	Keep-Alive: timeout=15, max=96
//	Connection: Keep-Alive
//	Content-Type: <ctp>

//!	a reply header for file based contents (images, js, css).
//!	@param l	content length
//!	@param ctp	content type
//!	@param tlm	last modified time
//!	@param etg	ETag content
//!	@param cec	content encoding (optional)
	const cbuffer& file(unsigned l, t_cc ctp, time_t tlm, t_cc etg, t_cc cec = 0);

//	HTTP/1.x 404 Not Found
//	Date: Mon, 30 Nov 2009 14:04:51 GMT
//	Server: Apache/2.2.12 (Ubuntu)
//	Vary: Accept-Encoding
//	Content-Encoding: gzip
//	Content-Length: 239
//	Keep-Alive: timeout=15, max=100
//	Connection: Keep-Alive
//	Content-Type: text/html; charset=iso-8859-1

//!	file not found header.
//!	@param l	content length
//!	@param ctp	content type
//!	@param cec	content encoding (optional)
	const cbuffer& notfound(unsigned l, t_cc ctp, t_cc cec = 0);

//!	Internal Server Error (500) header.
//!	@param l	content length
//!	@param ctp	content type
//!	@param cec	content encoding (optional)
	const cbuffer& error(unsigned l, t_cc ctp, t_cc cec = 0);

//!	Unsupported Media Type (415) header.
//!	@param l	content length
//!	@param ctp	content type
//!	@param cec	content encoding (optional)
	const cbuffer& badtype(unsigned l, t_cc ctp, t_cc cec = 0);

//!	HTTP/1.x 304 Not Modified
//!	Date: Tue, 08 Dec 2009 22:17:11 GMT
//!	Server: Apache/2.2.12 (Ubuntu)
//!	Connection: Keep-Alive
//!	Keep-Alive: timeout=15, max=32
//!	Etag: "73679-40-477db79738680"

//!	Not Modified (304) header.
//!	@param etg	ETag content
	const cbuffer& notmod(t_cc etg);


} // namespace htth

#endif // HTTH_H

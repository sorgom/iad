//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include <htth.h>
#include <ccs.h>
#include <http.h>
#include <stypes.h>
#include <vars.h>

namespace htth
{

	t_ch def_svn		= c_iadsrv;
	t_ch def_ctp		= c_text_html;
	unsigned def_tmo	= 15;
	unsigned def_max	= 100;

	t_cc c_CRLF = "\r\n";
	t_cc c_COL  = ": ";
	cbuffer cbh(128);
	cbuffer cbt(32);

//!	set general parameters.
//!	server name,
//!	default content type,
//!	minimum timeout,
//!	maximum timeout.
	void defaults(t_cc svn, t_cc ctp, unsigned tm, unsigned mx)
	{
		if (svn) def_svn = svn;
		if (ctp) def_ctp = ctp;
		if (tm ) def_tmo = tm;
		if (mx ) def_max = mx;
	}
//	HTTP/1.x 200 OK
//	Date: Sun, 29 Nov 2009 20:15:14 GMT
//	Server: <def_svn>
//	<cbt> CRLF
//	Connection: Keep-Alive
//	Keep-Alive: timeout=<def_tmo> max=<def_max>
//	Content-Length: <l>
//	Content-Type: <ctp>/<def_ctp>
//	[ Content-Encoding: cec ]
//!	common http header beginning.
	const cbuffer& stdh(unsigned nr, t_cc cr, unsigned l, t_cc ctp, t_cc cec = 0)
	{
		cbh.clean()
			<<	c_http1x	<< ' ' 		<< nr << ' ' << cr	<< c_CRLF
			<<	c_date		<< c_COL 	<< hdate(curr_time) << c_CRLF
			<<	c_server	<< c_COL 	<< def_svn 			<< c_CRLF
		;
	//	individual content
		if (cbt) { cbh.add(cbt); cbh	<< c_CRLF; cbt.clean(); }

		cbh	<< 	c_connection	<< c_COL	<< c_keep_alive	<< c_CRLF
			<<	c_keep_alive 	<< c_COL
				<< c_timeout	<< '=' 		<< def_tmo 		<< ' '
				<< c_max		<< '=' 		<< def_max 		<< c_CRLF
		;
		if (ctp) {
			if (cec) cbh
				<<	c_vary				<< c_COL	<< c_accept_encoding	<< c_CRLF
				<<	c_content_encoding	<< c_COL	<< cec					<< c_CRLF
			;
			cbh
				<<	c_content_length	<< c_COL	<< l	<< c_CRLF
				<<	c_content_type		<< c_COL	<< ctp	<< c_CRLF
			;
		}
		cbh	<<	c_CRLF ;
		return cbh;
	}

//	HTTP/1.x 200 OK
//	Date: Sun, 29 Nov 2009 20:15:14 GMT
//	Server: Server-Name

//	Cache-Control: no-store

//	Connection: Keep-Alive
//	Keep-Alive: timeout=15 max=100
//	Content-Length: 2276
//	Content-Type: text/html

//!	a standard reply header (generated content / html).
	const cbuffer& reply(unsigned l, t_cc cec)
	{
		cbt << c_cache_control << c_COL << c_no_store;
		return stdh(200, uc_ok, l, def_ctp, cec);
	}

//!	a standard text plain header for generated content.
	const cbuffer& txt(unsigned l, t_cc cec)
	{
		cbt << c_cache_control << c_COL << c_no_store;
		return stdh(200, uc_ok, l, c_text_plain, cec);
	}


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
	const cbuffer& file(unsigned l, t_cc ctp, time_t tlm, t_cc etg, t_cc cec)
	{
		cbt	<< c_last_modified	<< c_COL << hdate(tlm) << c_CRLF;
		cbt	<< c_etag			<< c_COL << etg;
		return stdh(200, uc_ok, l, ctp, cec);
	}

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
	const cbuffer& notfound(unsigned l, t_cc ctp, t_cc cec)
	{
		return stdh(404, c_not_found, l, ctp, cec);
	}

//!	Internal Server Error (500) header.
	const cbuffer& error(unsigned l, t_cc ctp, t_cc cec)
	{
		return stdh(500, c_internal_error, l, ctp, cec);
	}

//!	Unsupported Media Type (415) header.
	const cbuffer& badtype(unsigned l, t_cc ctp, t_cc cec)
	{
		return stdh(415, c_unsupported_type, l, ctp, cec);
	}

//!	Not Modified (304) header.
//!	@param etg	ETag content
	const cbuffer& notmod(t_cc etg)
	{
		cbt	<< c_etag << c_COL << etg;
		return stdh(304, c_not_modified, 0, 0);
	}
} // namespace htth

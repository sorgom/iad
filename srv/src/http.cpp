//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include <http.h>

#include <ccs.h>
#include <ini_io.h>
#include <md4.h>
#include <utilz.h>

// #define XDEVEL
#include <trace_macros.h>

#include <cstring>
#include <cstdlib>


namespace rq
{
//!	request processing state
	enum {
	//	nothing to process yet
		none,
	//	reading head has begun
		head,
	//	reading post data
		post
	};
}

//	============================================================
//	rproc


//	============================================================
//	rdata
	rdata::rdata()
	: state(rq::none)
	, buf(128)
	, cmr(10)
	, cl(0)
	{
		TRACE_FLOW
		TRACE_VAR(state)
	}

//	============================================================
//	rtok
	rtok::rtok(rproc* rp)
	: proc(rp)
	{}

	bool rtok::process(rdata* pdt, t_ch cc)
	{
		TRACE_FLOW
		TRACE_CLASS

		if (!pdt) return false;
		TRACE_VAR(pdt->state)
		TRACE_VARQ(cc)
		rdata& dt = *pdt;
		TRACE_VAR(dt.state)
		while (*cc) {
			char cl = 0;
			switch (dt.state) {
			//	nothing read yet:
			//	proceed to POST / GET
				case rq::none:
					TRACE_INF("rq::none")
					while (
						*cc &&
						strncmp(cc, uc_get, 3) &&
						strncmp(cc, uc_post, 4)
					) {
						cl = *cc;
						++cc;
					}
					if (!cl || cl == '\n' || cl == '\r') dt.state = rq::head;
					TRACE_INF("done")
				break;
			//	(continued) reading head data
				case rq::head:
					TRACE_INF("rq::head")
					cl = 0;
					while (*cc) {
					//	ignore CR
						if (*cc == '\r') { ++cc; continue; }
					//	check for double line break
						if (*cc == '\n' && cl == '\n') {
							++cc;
						//	if given: tokenize assembled data
							tokenize(dt);
							break;
						}
						else dt.buf.add(*cc);
						cl = *cc;
						++cc;
					}
					TRACE_INF("done")
				break;
			//	reading content (content length)
				case rq::post:
					TRACE_INF("rq::post")
					while (*cc && (*cc == '\n' || *cc == '\r')) ++cc;
					while (*cc && dt.buf.size() < dt.cl) {
						dt.buf.add(*cc);
						++cc;
					}
					if (dt.buf.size() == dt.cl) {
						dt.cmr.set(uc_query_string, dt.buf.str());
						callpr(dt);
					}
					TRACE_INF("done")
				break;
				default:
					TRACE_ERR("state")
					return false;
				break;
			}
		}
		return true;
	}
//!	tokenize assembled string.
	bool rtok::tokenize(rdata& dt)
	{
		TRACE_FLOW

		dt.cl = 0;
		cmapper& cm = dt.cmr;
		cm.clear();
		t_ch cc = dt.buf;
		if (!cc) {
			dt.state = rq::none;
			return false;
		}
		TRACE_VARQ(cc)
		TRACE_INF("split")
	//	split into GET / POST, file, protocol
		ncsbuffer sb(3);
		cc = split(cc, sb, '\n');
		if (sb.size() < 3) {
			dt.buf.clear();
			dt.state = rq::none;
			return false;
		}
	//	ckeck if this is a POST
		bool bpost = !strcmp(sb[0], uc_post);
		TRACE_VARB(bpost);

	//	if POST: mark that
		if (bpost) cm.set(uc_post, cdef);

	//	if not POST: check if we have a query string
		else {
			TRACE_INF(uc_query_string << '?')
			t_ch cf = sb[1];
			while (*cf && *cf != '?') ++cf;
		//	query string given
			if (*cf && *(cf + 1)) {
				++cf;
				TRACE_VARQ(cf)
				cm.copyv(uc_query_string, cf);
			}
		//	just file name
			else cm.copyv(uc_file, sb[1]);
		}
	//	call standard ini parser for remaining part
		TRACE_INF("ini part")
		read_ini_part(cm, cc, false);
		cm.sort();

	//	content buffer no more needed
		dt.buf.clear();

	//	if POST get Content-Length
		if (bpost) {
			TRACE_INF(c_content_length << '?')
			t_cc ccl = cm.getn(c_content_length);
			if (ccl) {
				dt.cl = atoi(ccl);
				TRACE_VAR(dt.cl)
				dt.state = rq::post;
				return true;
			}
			else {
				TRACE_ERR(c_content_length)
				dt.state = rq::none;
				cm.clear();
				return false;
			}
		}
		else return callpr(dt);
	}

//!	call processor.
	bool rtok::callpr(rdata& dt)
	{
		TRACE_FLOW
		bool b = !proc || proc->process(dt.cmr);
		dt.cmr.clear();
		dt.buf.clear();
		dt.cl = 0;
		dt.state = rq::none;
		return b;
	}

	t_cc c_http_date = "%a, %d %b %Y %H:%M:%S GMT";

//!	http header formatted date.
//!	"Sun, 06 Nov 1994 08:49:37 GMT" RFC 822, updated by RFC 1123
	t_fcc hdate(time_t tt)
	{
		TRACE_FLOW
		static char ct[30];
		struct tm* ptm = gmtime(&tt);
		strftime(ct, 30, c_http_date, ptm);
		return ct;
	}

//!	etag encoding of whatever sort of string.
//!	(16 character MD4 hex string)
	t_fcc etag(t_cc c, unsigned l)
	{
		TRACE_FLOW
		if (!c) return cdef;
		static char ct[33];
		memset(ct, 0, sizeof(ct));
		if (!l) l = strlen(c);
		unsigned char um[16];
		md4::encode(um, c, l);
		char* pc = ct;
		for (unsigned i = 0; i < 16; ++i, pc += 2) memcpy(pc, uc22h(um[i]), 2);
		return ct;
	}

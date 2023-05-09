//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include <iostream>

#include "httpsrv.hpp"

#include "bbl_mapper.hpp"
#include "ccs.hpp"
#include "cgi.hpp"
#include "dcp.hpp"
#include "deflate.hpp"
#include "defs.hpp"
#include "dflmgr.hpp"
#include "dml.hpp"
#include "f_io.hpp"
#include "fcp.hpp"
#include "hdn_vars.hpp"
#include "htth.hpp"
#include "sysctrl.hpp"
#include "utilz.hpp"
#include "val.hpp"
#include "vars.hpp"

// #define XDEVEL
#include "trace_macros.hpp"

#include <cstring>
#include <cstdlib>

//	file stat
#include <sys/types.h>
#include <sys/stat.h>


using namespace bbl;

#ifdef ISHOST
	void trh(const s_cc& k, const s_cc& v)
	{
		dml << k << '\t' << v << cnl;
	}
#endif

#define c_HTTP_DEFAULT_TTL 300

//	============================================================
//	httpsrv

//! constructor with maximum number of clients.
	httpsrv::httpsrv(unsigned s)
	: ncl(s > 0 ? s : 10)
	{
		TRACE_FLOW
		tok	 = new rtok(this);
		srv  = new tcpsrv(ncl, this);
		dmgr = new dflmgr;
		rqs  = new rdata*[ncl];
		memset(rqs,  0, ncl * sizeof(rdata*));
		srv->sttl(c_HTTP_DEFAULT_TTL);
	}
//!	clean up destructor.
	httpsrv::~httpsrv()
	{
		rdata** pd = rqs;
		for (unsigned i = ncl; i--; ++pd) if (*pd) delete *pd;
		delete rqs;
		delete tok;
		delete srv;
		delete dmgr;
	}
//!	start infinite server loop.
	int httpsrv::run(void)
	{
		TRACE_FLOW
	//	load SID from tmp file
		load_sid(s_sid);
	//	get port & ip from ini
		t_cc c = ini.getn(c_port);
		TRACE_VARQ(c)
		t_u2 p = c ? atoi(c) : 0;
		int r = srv->run(p, ini.getn(c_ip));
	//	save SID to tmp file
		save_sid(s_sid);
		return r;
	}
//	============================================================
//	httpsrv : tcpproc
//!	remove client.
	void httpsrv::remove(unsigned n)
	{
		TRACE_FLOW
		if (n < ncl && rqs[n]) {
			delete rqs[n];
			rqs[n] = 0;
		}
	}
//! process client data.
	void httpsrv::process(unsigned n, t_fcc c)
	{
		TRACE_FLOW
		TRACE_VAR(n)
		if (n < ncl) {
			if (!rqs[n]) {
				TRACE_INF("new rdata")
				rqs[n] = new rdata;
			}
			cnr = n;
			tok->process(rqs[n], c);
		}
	}

//	============================================================
//	httpsrv : rproc
//	the big one: process tokenized http request data.
	bool httpsrv::process(const cmapper& cm)
	{
		TRACE_FLOW
		TRACE_VAR(cnr)
		TRACE_VAR(cm.size());

	//	check for _CTRL in message header
		if (cm.getn(uc_ctrl)) return ctrl(cm);

		t_ch c;

	//	if header contains "Keep-Alive":
	//	set new timeout
		c = cm.getn(c_keep_alive);
		if (val::is_num(c, true, true)) {
			unsigned ka = atoi(c);
			TRACE_VAR(ka)
			srv->sttl(cnr, ka);
		}
	//	standard processing when
	//	- post
	//	- query string
	//	- no extension
	//	- extension -> default
	//	file
		t_ch cfl;
	//	extension
		t_ch cxt;
	//	outgoing content type (by extension)
		t_ch cct;
	//	post form
		bool bpost	= cm.getn(uc_post);
	//	query string
		t_ch cqs	= cm.getn(uc_query_string);
	//	post, query string
		bool bstd	= bpost || cqs;

		bool ok;

		TRACE_VARB(bstd)

	//	no extension, extension unknown, extension -> default
		if (!bstd) {
			TRACE_INF("extension check")
			cfl = cm.getn(uc_file);
			cxt = ext(cfl);
			TRACE_VARQ(cxt)
			cct = cxt ? ini.getn(cxt) : 0;
			TRACE_VARQ(cct)
			bstd = !cxt || (cct && !strcmp(cct, c_default));
		}
		TRACE_VARB(bstd)

	//	error: extension given but no outgoing content type assigned
		if (!bstd && !cct) {
		//	send Unsupported Type reply
			unsigned l = strlen(c_unsupported_type);
			const cbuffer& cbh = htth::badtype(l, c_text_plain);
			srv->send(cnr, cbh, cbh.size());
			srv->send(cnr, c_unsupported_type, l);
			return false;
		}
	//	determination of deflate
		bool df =
		//	no deflate on ajax
			!is_ajax &&
		//	general ok
			dmgr->dflok &&
		//	user agent is not MSIE
			(c = cm.getn(c_user_agent)) &&
			!lfind(c, uc_msie) &&
		//	accept encoding from client	given
			(c = cm.getn(c_accept_encoding)) &&
			lfind(c, c_deflate)
		;

		TRACE_VARB(df)

	//	========================================
	//	standard processing
		if (bstd) {
			TRACE_VARB(bpost)
			TRACE_VARQ(cqs)
		//	========================================
		//	call cgi parameters	processing
			srv_params(cqs, bpost, s_sid);

			TRACE_VARQ(curr_lang)
			TRACE_VARQ(curr_lay)
			TRACE_VARQ(call_page)
			TRACE_VARQ(curr_sid)
			TRACE_VARB(is_ajax)
		//	========================================
		//	check for language / layout change
		//	this can be done on address base

		//	ajax request with no valid SID:
		//	replay with empty header instantly
			if (is_ajax && !curr_sid) {
				const cbuffer& cbh = htth::txt(0);
				srv->send(cnr, cbh, cbh.size());
				return true;
			}

		//	set current SID if valid and not ajax
			if (!is_ajax && curr_sid) s_sid.copy(curr_sid);
			TRACE_VARQ(s_sid)

		//	set current language if valid SID or not ajax
			if (curr_sid || !is_ajax) mapper.set(curr_lang, curr_lay, curr_sid);
		//	========================================
		//	start the processing
			ok = parser.parse();
		//	reset cprocs (for "once" processing)
			fcp::reset();
		//	ok: return standard reply header & contents
			if (ok) {
			//	ajax as plain text
				if (is_ajax) {
					const cbuffer& cbh = htth::txt(dml.size());
					srv->send(cnr, cbh, cbh.size());
					srv->send(cnr, dml, dml.size());
				}
				else {
#ifdef ISHOST
					dml << c_com_open << cnl;
					cgi.traverse(&trh);
					dml << cnr << cnl;
					dml << c_com_close;
#endif
					df = df && dml.size() >= 2048;
					if (df) {
						cbuffer cbf(4096);
						if (unsigned sz = deflate(dml, dml.size(), cbf)) {
							const cbuffer& cbh = htth::reply(sz, c_deflate);
							srv->send(cnr, cbh, cbh.size());
							srv->send(cnr, cbf, sz);
						}
						else df = false;
					}
					if (!df) {
						const cbuffer& cbh = htth::reply(dml.size());
						srv->send(cnr, cbh, cbh.size());
						srv->send(cnr, dml, dml.size());
					}
				//	after processing and send:
				//	check for system control cproc calls.
					if (sctrl) {
						switch (sctrl) {
							case ctrl::shutdown:
							case ctrl::restart :
							case ctrl::sysreset:
								srv->shutdown(sctrl);
							default:
							break;
						}
						hdn::sctrl = 0;
					}
				}
			}
		//	nok: return standard text plain header & contents
			else {
				const cbuffer& cbh = htth::txt(dml.size());
				srv->send(cnr, cbh, cbh.size());
				srv->send(cnr, dml, dml.size());
			}
		}

	//	========================================
	//	file send
		else {
		//	call deflate manager
			const dflfile& dff = dmgr->dfile(cfl, df, cxt, cm.getn(c_if_none_match));

		//	not modified: send 304
			if (dff.nomod) {
				const cbuffer& cbh = htth::notmod(dff.etg);
				srv->send(cnr, cbh, cbh.size());
			}
		//	if successfull: send file part by part
			else if (dff.sz && dff.fp) {
				ok = true;
			//	send header
				const cbuffer& cbh = htth::file(dff.sz, cct, dff.tlm, dff.etg, dff.cec);
				srv->send(cnr, cbh, cbh.size());

				unsigned sz = dff.sz;
			//	create block size
				unsigned bs = bsize(sz > c_TCP_SEND_BYTES ? c_TCP_SEND_BYTES : sz);
				char* buf = new char[bs];
				char* cp;

				while (ok && sz) {
					unsigned td = sz > bs ? bs : sz;
					cp = buf;
					ok =
						ffread(cp, td, dff.fp) &&
						srv->send(cnr, buf, td);
					sz -= td;
				}
				delete buf;
				fclose(dff.fp);
			}
		//	otherwise send error
			else {
				cbf1.clean() << msg_file_not_found << cfl;
				const cbuffer& cbh = htth::notfound(cbf1.size(), c_text_plain);
				srv->send(cnr, cbh,  cbh.size());
				srv->send(cnr, cbf1, cbf1.size());
				ok = false;
			}
		}
		TRACE_INF("done cnr " << cnr)
		return ok;
	}

//!	CTRL message processing.
	bool httpsrv::ctrl(const cmapper& cm)
	{
		t_ch c = cm.get(uc_ctrl);
		return ctrl(
			bc_all(&is_numb, c) ?  atoi(c) : ctrl::unknown
		);
	}
	bool httpsrv::ctrl(int n)
	{
		bool r = true;
		s_cc sd;
		switch (n) {
			case ctrl::shutdown:
			case ctrl::restart :
			case ctrl::sysreset:
				srv->shutdown(n);
			break;
			case ctrl::stats:
			{
				const tcpstats& st = srv->stats();
				cbf1.clean()
					<< static_cast<unsigned>(curr_time - st.ts) << cnl
					<< st.ncl << cnl
					<< st.ncc << cnl
					<< st.trv;
				cbf1.over(sd);
			}
			case ctrl::hello:
			case ctrl::ok:
			break;
			default:
				r = false;
				n = ctrl::unknown;
			break;
		}
		cbf1.clean()
			<< (r ? c_ok : c_nok) << ' ' << n << cnl
			<< c_iadsrv
		;
		if (sd) cbf1 << cnl << sd;

		const cbuffer& cbh = htth::txt(cbf1.size());
		srv->send(cnr, cbh , cbh. size());
		srv->send(cnr, cbf1, cbf1.size());

		return r;
	}

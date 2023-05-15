//| ============================================================
//! @file
//!
//! HTTP server.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef HTTPSRV_H
#define HTTPSRV_H

#include "http.h"
#include "tcpsrv.h"

#include <cstdio>

//!	defaltion manager
	class dflmgr;

//!	http server class combines virtual interfaces of
//! tcp processor (tcpproc),
//!	http request processor (rproc).
	class httpsrv : public tcpproc, public rproc
	{
	public:
	//! constructor with maximum number of clients.
		httpsrv(unsigned = 0);
	//!	clean up destructor.
		~httpsrv();
	//!	start infinite server loop.
		int run();
	private:
	//	========================================
	//	tcpproc
	//!	install a new internal client representation.
		inline void create(unsigned) {}
	//!	remove client.
		void remove(unsigned);
	//! process client data.
		void process(unsigned, t_fcc);
	//	========================================
	//	rproc
	//!	process http request data.
		bool process(const cmapper&);
	//	========================================
	//	modules
	//!	request tokenizer.
		rtok* tok;
	//!	tcp server.
		tcpsrv* srv;
	//!	deflate manager
		dflmgr* dmgr;
	//	========================================
	//	other members
	//!	maximum number of request data size.
		unsigned ncl;
	//!	request data.
		rdata** rqs;
	//!	current number.
		unsigned cnr;
	//!	SID storage.
		s_cc s_sid;
	//!	_CTRL message processing.
		bool ctrl(const cmapper&);
		bool ctrl(int);
	//	========================================

	};
#endif // HTTPSRV_H

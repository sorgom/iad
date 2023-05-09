//| ============================================================
//! @file
//!
//! #cproc retrieval and assignment server interface.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifdef ISSRV

#include "fcpsrv.hpp"
#include "cpreg.hpp"

#include "trace_macros.hpp"

//!	find and process: ok
	bool fcpsrv::ok(t_cc c) const
	{
		TRACE_FLOW
		sbuffer sb(1);
		cproc* cp = findcp(c);
		return cp && cp->ok(sb);
	}
//!	find and process: ok
	bool fcpsrv::ok(sbuffer& sb) const
	{
		TRACE_FLOW
		cproc* cp = findcp(sb.shift());
		return cp && cp->ok(sb);
	}

//!	find and process: put
	void fcpsrv::put(sbuffer& sb) const
	{
		TRACE_FLOW
		while (sb) {
			TRACE_VARQ(sb[0])
			cproc* cp = findcp(sb.shift());
			if (cp) cp->put(sb);
		}
	}

//!	find and process: set
	void fcpsrv::set(sbuffer& sb) const
	{
		while (sb) {
			TRACE_VARQ(sb[0])
			cproc* cp = findcp(sb.shift());
			if (cp) cp->set(sb);
		}
	}
//! reset all instanciated cprocs.
	void fcpsrv::reset(void) const
	{
		resetcps();
	}

	fcpsrv inst_fcp;
	const fcpsrv& fcp = inst_fcp;

#endif
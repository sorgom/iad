//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include "fcp.hpp"

#include "cpreg.hpp"

#include "trace_macros.hpp"

namespace fcp
{

//!	find and process: ok
	bool ok(t_cc c)
	{
		TRACE_FLOW
		sbuffer sb(1);
		cproc* cp = findcp(c);
		return cp && cp->ok(sb);
	}
//!	find and process: ok
	bool ok(sbuffer& sb)
	{
		TRACE_FLOW
		cproc* cp = findcp(sb.shift());
		return cp && cp->ok(sb);
	}

//!	find and process: put
	void put(sbuffer& sb)
	{
		TRACE_FLOW
		while (sb) {
			TRACE_VARQ(sb[0])
			cproc* cp = findcp(sb.shift());
			if (cp) cp->put(sb);
		}
	}
//!	find and process: set
	void set(sbuffer& sb)
	{
		while (sb) {
			TRACE_VARQ(sb[0])
			cproc* cp = findcp(sb.shift());
			if (cp) cp->set(sb);
		}
	}
//! reset all instanciated cprocs.
	void reset(void)
	{
		resetcps();
	}

//! remove all instanciated cprocs.
	void remove(void)
	{
		removecps();
	}

} // namespace fcp

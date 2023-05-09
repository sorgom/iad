// ---------------------------------------------------------------------------
//
//  Module   :
//
//  Author   : Manfred Sorgo
//  Email    : ms@sorgo.de
//
// ---------------------------------------------------------------------------
//

#include "bbl_mapper.hpp"
#include "ccs.hpp"
#include "db.hpp"
#include "dcp.hpp"
#include "cgi.hpp"
#include "dml.hpp"
#include "ini.hpp"
#include "sample_db_trigger.hpp"
#include "sample_sysctrl.hpp"
#include "vars.hpp"

// #undef DEVEL
#define XDEVEL
#include "trace_macros.hpp"

#include <cstdio>

#ifdef ISHOST
	void trh(const s_cc& k, const s_cc& v)
	{
		dml << k << '\t' << v << cnl;
	}
#endif


int main(void)
{
	TRACE_FLOW
	TRACE_INF("START OF WEB")
	load_ini(c_iadcgi_ini);
	
	TRACE_INF("cgi_params")
	cgi_params();

	info_trigger trg;
	db::proc::instance().set_trigger(&trg);

	TRACE_INF("bblinit")
	bbl::bblinit();

//	parse with each time directory scan
	TRACE_INF("parser.parse")
	bool ok = parser.parse(true);

#ifdef ISHOST
	if (ok) {
		dml << c_com_open << cnl;
		cgi.traverse(&trh);
		dml << c_com_close;
	}
#endif
	TRACE_INF("dml out")
	t_cc cf = "%s: %s\n\n%s";
	printf(cf,
		c_content_type,
		(ok ? content_type : c_text_plain),
		dml.str()
	);

//	sysctrl processing
	TRACE_VAR(sctrl)
	if (sctrl) {
		simplectrl sct;
		sct.process(sctrl);
	}

	return 0;
}















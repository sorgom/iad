// ---------------------------------------------------------------------------
//
//  Module   :
//
//  Author   : Manfred Sorgo
//  Email    : ms@sorgo.de
//
// ---------------------------------------------------------------------------
//

#include <bbl_mapper.h>
#include <ccs.h>
#include <db.h>
#include <dcp.h>
#include <cgi.h>
#include <dml.h>
#include <ini.h>
#include <sample_db_trigger.h>
#include <sample_sysctrl.h>
#include <vars.h>

// #undef DEVEL
#define XDEVEL
#include <trace_macros.h>

#include <cstdio>

#ifdef ISHOST
	void trh(const s_cc& k, const s_cc& v)
	{
		dml << k << '\t' << v << cnl;
	}
#endif


int main()
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















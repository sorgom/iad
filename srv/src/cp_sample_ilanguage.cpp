//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include <cp_sample_ilanguage.h>

// #define XDEVEL
#include <cp_convenience.h>

//	============================================================
//	cp_ilanguages

	cp_ilanguages::cp_ilanguages()
	: cmp(1)
	, cma(2)
	{}

//!	traverse langs: native long name -> short sign
	void cp_ilanguages::operator()(const s_cc& lg, const s_cc& sh)
	{
		TRACE_FLOW
		dml << cnl;
	//	cast to t_cc
		t_cc cl = lg;
		t_cc cs = sh;
	//	if not current language:
	//	create link
	//	<a href=".../?lang=de" class=de title="Deutsch">&nbsp;</a>
		if (strcmp(curr_lang, cs)) {
		//	parameter
			cmp.set(c_lang,  cs);
		//	attributes
			cma.set(c_class, cs);
			cma.set(c_title, cl);
			link(c_nbsp, cmp, &cma);
		}
	//	otherwise just create a span
	//	<span class=de title="Deutsch">&nbsp;</span>
		else {
			dml << tag::span() << ta::cl(cs)
				<< ta::title(cl)
				<< c_nbsp << tte
			;
		}
	}

	void cp_ilanguages::put(sbuffer&)
	{
		TRACE_FLOW
		langs.traverse(*this);
		cmp.clear();
		cma.clear();
		dml << cnl;
	}

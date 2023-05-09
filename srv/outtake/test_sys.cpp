//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include <cstdlib>
#include "ini.hpp"

#define XDEVEL
#include "trace_macros.hpp"
#include "tutilz.hpp"
	int		ac;
	char**	av;

//!	retrieve an argument value.
//!	(first is 0, ac must be 2)
//	ac must be
	const char* avn(unsigned n)
	{
		return (ac > n + 1 ? av[n + 1] : 0);
	}

//	============================================================
//	setup & finish
	void setup(void)
	{
		TRACE_FLOW
		load_ini(c_iadcgi_ini);
	}

	void finish(void)
	{
		TRACE_FLOW
	}
//	============================================================
//	the tests
	void test01(void)
	{
		TRACE_FLOW
		TRACE_INF("system call of " << c_sysreset << " = ini value in " << c_iadcgi_ini)
		t_cc csys = ini.getn(c_sysreset);
		TRACE_VARQ(csys)
		if (csys) {
			int sysret = system(csys);
			TRACE_VAR(sysret)
		}
	}

	void test02(void)
	{
		TRACE_FLOW
		TRACE_INF("")
	}

	void test03(void)
	{
		TRACE_FLOW
		TRACE_INF("")
	}

	void test04(void)
	{
		TRACE_FLOW
		TRACE_INF("")
	}

	void test05(void)
	{
		TRACE_FLOW
		TRACE_INF("")
	}

	int main(int argc, char* argv[])
	{
		ac = argc;
		av = argv;
		setup();

		test01();
	//	test02();
	//	test03();
	//	test04();
	//	test05();

	//	finish();
		return 0;
	}

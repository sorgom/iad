//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include <cp_samples_htm.h>

#include <cp_convenience.h>

using namespace tag;
using namespace ta;
//	============================================================
//	cp_JS

	void cp_JS::put(sbuffer& sb)
	{
		if (sb) while(sb) jscript(sb.shift());
		else start_js(false);
	}

//	============================================================
//	cp_JE

	void cp_JE::put(sbuffer&)
	{
		dml << c_tcl << c_script << '>';
	}

//	============================================================
//	cp_JWS

	void cp_JWS::put(sbuffer&)
	{
		start_js(false);
		dml << c_java_write;
		script_quote = true;
		cnl = ' ';
	}
//	============================================================
//	cp_JWE

	void cp_JWE::put(sbuffer&)
	{
		script_quote = false;
		cnl = '\n';
		dml << c_java_write_end << c_tcl << c_script << '>';
	}

//| ============================================================
//! @file
//!
//! Dynamic content parser.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef DCP_HPP
#define DCP_HPP


#include "defs.hpp"
#include "types.hpp"
#include "cmapper.hpp"
#include "dml.hpp"


	class dcp
	{
	public:
		dcp();
		~dcp();
	//!	parse current page (set by cgi).
		bool parse(void);

	private:
	//!	circular file parse preventer.
		csbuffer dfs;
	//!	current page tracker stack.
		ncsbuffer pgs;
	//!	attribues mapper.
		cmapper cma;
	//!	parse attributes contents.
		static void parsecma(cmapper&, dcp*);
		static void trcma(s_cc&, s_cc&);
	//! just read and place a page to dml.
		bool placep(t_cc);
	//!	parse a page to dml with r1 state set.
		bool parsep(t_cc, bool);
	//!	common reader for parse and place.
		char* readp(t_cc);
	//!	parse sub: char*.
		void parse(t_ch, bool = false, outstream& = dml, bool = false);
	//!	parse a function statement.
	//! replace $1 ... $9 by given sbuffer [0] ... [8].
		void parsefunc(t_ch, const sbuffer&, s_cc&);
	//	general write (defined by IF, ELIF, ELSE, ENDIF)
		bool bout;
	//	in comment (defined by "<--" / "-->")
		bool bcom;
	//	summary of the above
		bool bok;
	//	if level and level states
		int ifl;
	//	level is true
		bool ifs[c_AHTM_IF_LEVELS];
	//	level has been done
		bool ifd[c_AHTM_IF_LEVELS];
	//	on error and exit
		t_ch onerror;
		bool bexit;
	//	check dcp_vars for certain keys
		void check_vars(void);
	//	arguments splitter method
		static t_cc splitv(t_ch cs, csbuffer& csb, sbuffer& sb, char ct = ')');
	};

	extern dcp parser;

#endif // _H

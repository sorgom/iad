//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include "sfunc.hpp"

#include "bbl_mapper.hpp"
#include "ccs.hpp"
#include "vars.hpp"

#include "trace_macros.hpp"

//!	output formatter for text to text.
	bool frmout(t_cc cc, outstream& os)
	{
		TRACE_FLOW
		t_ch c;
		bool q = false;
		for (c = cc; *c && !q; ++c) {
			switch (*c) {
				case '&' :
				case '"' :
				case '\n':
				case '>' :
				case '<' :
					q = true;
				default:
				break;
			}
		}
		if (!q) {
			os.add(cc);
			return false;
		}
		for (c = cc; *c; ++c) {
			switch (*c) {
				case '&' : os.add(c_amp); break;
				case '"' : os.add(c_quot); break;
				case '>' : os.add(c_gt); break;
				case '<' : os.add(c_lt); break;
				case '\n': os.add("<br"); if (htm_xml) os.add('/'); os.add('>'); break;
				case '\r': break;
				default:
					os.add(*c);
				break;
			}
		}
		return true;
	}
//!	xtag formatter stream functor function I: the class.
	class sf_format : public sfunc
	{
	public:
		t_ch str;
		void operator()(outstream& os) const
		{
			TRACE_FLOW
			frmout(str, os);
		}
		bool data(void) const { return str; }
	};
//!	xtag formatter stream functor function II: the delivering function.
	sfunc* format(t_cc str)
	{
		TRACE_FLOW
		sf_format* sff = new sf_format;
		sff->str = str;
		return sff;
	}

//!	bbl translator stream functor function I: the class.
	class sf_trans : public sfunc
	{
	public:
		t_ch k;
		t_ch d;
		void operator()(outstream& os) const
		{
			if (!k) return;
			t_cc v = bbl::mapper.getn(k);
			frmout(v ? v : d ? d : k, os);
		}
	};

//!	bbl translator stream functor function II: the delivering function.
	sfunc* trans(t_cc key, t_cc alt)
	{
		TRACE_FLOW
		sf_trans* sft = new sf_trans;
		sft->k = key;
		sft->d = alt;
		return sft;
	}

//!	bbl word by word translator stream functor function I: the class.
	class sf_wtrans : public sfunc
	{
	public:
		t_ch str;
		void operator()(outstream& os) const
		{
			if (!str) return;
			ncsbuffer sb;
			split(str, sb);
			if (!sb) return;
			cbf1.clean();
			for (unsigned i = 0; i < sb.size(); ++i) {
				if (i) cbf1.add(' ');
				cbf1.add(bbl::mapper.getk(sb[i]));
			}
			frmout(cbf1, os);
		}
	};
//!	bbl word by word translator stream functor function II: the delivering function.
	sfunc* wtrans(t_cc str)
	{
		TRACE_FLOW
		sf_wtrans* sfw = new sf_wtrans;
		sfw->str = str;
		return sfw;
	}

//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef SFUNC_H
#define SFUNC_H

#include "outstream.h"

//!	stream functor abstract class.
	class sfunc
	{
	public:
		virtual ~sfunc() {}
		virtual void operator()(outstream&) const {}
		virtual bool data() const { return true; }
	};

//! a simple string #sfunc.
	class sf_str : public sfunc
	{
	public:
		inline sf_str(t_cc cc)
		: c(cc)
		{}
		t_ch c;
		inline void operator()(outstream& os) const { if (c) os.add(c); }
		inline bool data() const { return c; }
	};

//!	general xtag formatter for #outstream.
//!	@return true if changes where made
	bool frmout(t_cc, outstream&);

//!	xtag formatter stream functor function.
	sfunc* format(t_cc);

//!	bbl translator stream functor function.
	sfunc* trans(t_cc key, t_cc alt = 0);

//!	bbl word by word translator stream functor function.
	sfunc* wtrans(t_cc str);

#endif // SFUNC_H

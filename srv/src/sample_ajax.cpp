//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include <sample_ajax.h>

#include <ccs.h>

	inob nb_ajax_n(100);
	cnob nb_ajax_c(nb_ajax_n, c_ajax);

//!	increase ajax counter and provide "ajax_NNN" nob.
	const nob& next_ajax_id()
	{
		++nb_ajax_n;
		return nb_ajax_c;
	}

//!	provide "ajax_NNN" nob with current ajax counter.
	const nob& curr_ajax_id()
	{
		return nb_ajax_c;
	}

//| ============================================================
//! @file
//!
//! Sample for unique ajax id provider (#nob) functions.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef SAMPLE_AJAX_H
#define SAMPLE_AJAX_H

#include "nob.h"

//!	increase ajax counter and provide "ajax_NNN" nob.
	const nob& next_ajax_id();

//!	provide "ajax_NNN" nob with current ajax counter.
	const nob& curr_ajax_id();


#endif // SAMPLE_AJAX_H

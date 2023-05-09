//| ============================================================
//! @file
//!
//! Sample for unique ajax id provider (#nob) functions.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef SAMPLE_AJAX_HPP
#define SAMPLE_AJAX_HPP

#include "nob.hpp"

//!	increase ajax counter and provide "ajax_NNN" nob.
	const nob& next_ajax_id(void);

//!	provide "ajax_NNN" nob with current ajax counter.
	const nob& curr_ajax_id(void);


#endif // SAMPLE_AJAX_HPP

//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include <cp_samples_ajax.h>

#include <cp_convenience.h>
#include <sample_ajax.h>

using namespace ta;
using namespace tag;

//	============================================================
//	cp_ajax_able


//	============================================================
//	pre_ajax

//	ajax container storage
	s_cc pre_ajax::ajx;

	bool pre_ajax::pre()
	{
		if (curr_sid && !is_ajax) {
			if (ajx) {
				dml << tte;
				ajx = 0;
			}
			return ok = true;
		}
		return ok = false;
	}

//	============================================================
//	cp_begin_ajax

	void cp_begin_ajax::put(sbuffer& sb)
	{
		if (pre()) {
			if (sb)	ajx.copy(sb.shift());
			else ajx = c_div;
			dml << mtt(ajx) << id(next_ajax_id());
		//	additional class?
			if (sb) dml << cl(sb.shift());
			dml << cdef;
		}
		else { sb.shift(); sb.shift(); }
	}

//	============================================================
//	cp_end_ajax
	void cp_end_ajax::put(sbuffer&)
	{
		pre();
	}

//	============================================================
//	cp_ajax_updater

//	OLD
//	new Ajax.PeriodicalUpdater(
//		$('ajax_101'),
//		'/',
//		{	method:'get',
//			frequency:1,
//			parameters:{
//				page:'ajax_clock',
//				sid:'829F77',
//				plg:'en',
//				ply:'alcor',
//				ajax:'1'
//			}
//		});

//	NEW
//	http_repeat(url, params, obj, msec)
//	http_repeat(url, params, $(id), msec)

//	output until url
	t_cc c__url = "http_repeat('";
//	output until parameters
	t_cc c__par1  = "',{";
//	parameter name to parameter
	t_cc c__par2 = ":'";
//	parameter value / next name separator
	t_cc c__par3 = "',";
//	end of parameters to object id
	t_cc c__id = "'}, $('";
//	end of id to frequency
	t_cc c__frq = "'),";
//	end of frequency and end of call
	t_cc c__end = ");";

	void cp_ajax_updater::put(sbuffer& sb)
	{
		TRACE_FLOW
		TRACE_INF("cp_ajax_updater::put")
	//	check for parameters
	//	- 1: frequency
		t_ch cfq = "1";
		if (sb && val::is_num(sb[0], true, true)) cfq = sb.shift();

		t_ch cpg = curr_page;
		if (sb) cpg = sb.shift();

	//	if this is not an ajax request
		if (pre()) {
		//	further output depends on script_name and curr_page
			if (script_name && cpg) {
			//	start javascript tag
				start_js();
				dml
				//	put out http_repeat with parameters
					<<	c__url	<< script_name
					<<	c__par1
						<< c_page	<< c__par2	<< cpg			<< c__par3
						<< c_sid	<< c__par2	<< curr_sid		<< c__par3
						<< c_plg	<< c__par2	<< curr_lang	<< c__par3
						<< c_ply	<< c__par2	<< curr_lay		<< c__par3
						<< c_ajax	<< c__par2	<< '1'
					<<	c__id	<<	curr_ajax_id()
					<<	c__frq	<<	cfq
					<<	c__end
				//	end javascript tag
					<< tte
				;
			}
		}
	}

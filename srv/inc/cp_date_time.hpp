//| ============================================================
//! @file
//!
//! Date and Time setter cprocs.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef CP_DATE_TIME_HPP
#define CP_DATE_TIME_HPP

#include "cproc.hpp"
#include "vars.hpp"
#include <ctime>

	class time_processor
	{
	public:
		inline virtual time_t	get_time(void) { return curr_time; }
		inline virtual void		set_time(time_t) {}
		inline virtual void		reset(void) {}
	};

	class time_setter : public cproc, public cp_splitter
	{
	public:
		time_setter(time_processor* = 0);
		void put(sbuffer&);
		inline virtual void reset(void) { if (ptpr) ptpr->reset(); }
	//!	digit selector (2 digits).
	//!	@param mx maximum value (up to 99)
	//!	@param ic current value (selected)
	//!	@param st setep size
	//!	@param mn minimum value
		static void digisel(unsigned mx, unsigned ic, unsigned st = 1, unsigned mn = 0);
	protected:
	//!	number of seconds to round up before output, default: 60.
	//!	it is most likely that a user wants set manual time on a straight minute.
		unsigned rnd;
	private:
		time_processor* ptpr;
	};


#endif // CP_DATE_TIME_HPP

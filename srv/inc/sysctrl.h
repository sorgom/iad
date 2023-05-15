//| ============================================================
//! @file
//!
//! system interaction and ctrl numbers.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef SYSCTRL_H
#define SYSCTRL_H

//!	CTRL messages numbers
namespace ctrl
{
	enum {
	//	-999 ...  999: return values
		unknown		= 	-2,
		error		=   -1,
		ok			=    0,
	//	1001 ... 1999: http / tcp server information requests
		hello		= 1001,
		stats		= 1002,
	//	2001 ... 2999: server control
		restart		= 2001,
		shutdown	= 2002,
	//	3001 ... 3999: system interaction
	//	(call of assigned sysctrl instance)
		sysreset	= 3001,
	//	4000: end of ctrl
		end			= 4000
	};


	class sysctrl
	{
	public:
		virtual void process(int) {}
	};

	void set(sysctrl*);

	void process(int);

} // namespace ctrl

#endif // SYSCTRL_H

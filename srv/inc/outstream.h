//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef OUTSTREAM_H
#define OUTSTREAM_H

#include "types.h"

	class outstream
	{
	public:

		virtual void add(t_cc) {}
		virtual void add(char) {}
		virtual void add(t_num) {}
		virtual void add(int) {}

//		template <class T> inline void add(T t) {}
		template <class T>
		inline outstream& operator<<(const T& t) { add(t); return *this; }
	};

#endif // OUTSTREAM_H

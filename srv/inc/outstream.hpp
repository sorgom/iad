//| ============================================================
//! @file
//!
//!
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef OUTSTREAM_HPP
#define OUTSTREAM_HPP

#include "types.hpp"

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

#endif // OUTSTREAM_HPP

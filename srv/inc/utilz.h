//| ============================================================
//! @file
//!
//!	Unclassified utilities collection.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef UTILZ_H
#define UTILZ_H

#include "types.h"

//!	type definition of a boolean character function.
	typedef bool (bc_func)(char);

//! determination of numerical chracters [0-9].
    bool is_numb(char c);

//! determination of alphabetical chracters [_A-Za-z].
    bool is_alpha(char c);

//! determination of alphanumerical chracters [_A-Za-z0-9].
    bool is_alphanum(char c);

//!	determination of alphanumerical and blank.
	bool is_alphanum_blank(char c);

//! determination of blanc.
    bool is_blank(char c);

//! determination of space (blank plus line breaks).
    bool is_space(char c);

//! determination of hex [0-9a-fA-F]
	bool is_hex(char c);

//!	apply boolean character function to a complete string.
//!	@return true if all characters match.
	bool bc_all(bc_func*, t_ch);

//!	apply boolean character function to a complete string.
//!	@return true if at least one character matches.
	bool bc_has(bc_func*, t_ch);

//!	reduce all blanks (blank or tab) of a character string to single blanks,
//!	remove leading and tailing blanks.
	char* space(char* c, bool bs = false);

//!	remove white spaces from a character string.
	inline char* shrink(char* c) { return space(c, true); }

//!	get hex base of a hexadecimal character.
	char hex_base(char c);

//!	transfer one hexadecimal character to value.
	unsigned char unhex1(char c);

//!	transfer hexadecimal two digit string to value.
	unsigned char unhex2(t_ch c);

//!	transfer a query string to readable text.
	void unqs(char* c);

//!	find word in string.
//!	returns false if string contains more alpanumerical stuff after word.
//!	@param s  string to be evaluated
//!	@param w  search word
//!	@param ct termination of search character
	bool lfind(t_ch s, t_cc w, char ct = 0);

//! 2 digit string of unsigned (e.g. for time settings).
	t_fcc u22d(unsigned);

//!	2 digit hex string of unsigned char (e.g. for mac addresses).
	t_fcc uc22h(unsigned char);

//!	file extension of a string (including dot ".").
	t_fcc ext(t_cc);


//!	overwrite a const char* const.
	void setcc(t_cc& cc, t_cc cn);


//!	stream string value tryer class.
	class ctryer
	{
	public:
		inline ctryer() : pc(0) {}
		ctryer& operator()(t_ch&);
		ctryer& operator<<(t_cc);
	private:
		t_ch* pc;
	};

//! stream style try to set string value.
//!	e.g. ctry(c) << cc1 << cc2 << cc3;
//!	c becomes first non null value.
	ctryer& ctry(t_ch&);

#endif // UTILZ_H

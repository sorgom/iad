//| ============================================================
//! @file
//!
//! The balel key value translator class.
//! (#cmapper class extended by file load)
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef BBL_MAPPER_HPP
#define BBL_MAPPER_HPP

#include "cmapper.hpp"
#include "dirman.hpp"
#include <ctime>

namespace bbl
{
//!	bbl directory manager init call.
	bool bblinit(void);
//	============================================================
//	CGI interface
//	============================================================
#ifndef ISSRV
//!	language instance of cmapper.
	extern cmapper mapper;

//	============================================================
//	HTTP server multi language manager
//	============================================================
#else

//!	HTTP server multi language manager.
	class bmapper
    {
    public:
		bmapper();

	//!	set current language and layout.
	//! @param lang language short (e.g. de)
	//! @param lay  layout   short (e.g. alcor)
	//! @param main is main client (loads full set of languages)
		bool set(t_cc lang, t_cc lay, bool main);

	//!	retreive a translation
	//!	(with key return in case of not found).
	//!	@param key key to be translated
		t_cc getk(t_cc key);

	//!	retreive a translation
	//!	(with NULL return in case of not found).
	//!	@param key key to be translated
		t_cc getn(t_cc key);

	private:
	//!	pointer to current language and layout cmapper.
		cmapper* lcm;
	//!	cmapper for current main / login language & layout.
		cmapper cmm, cml;
	//!	storage of current main / login language & layout.
		s_cc smlg, smly, sllg, slly;
	};

//!	the instance as bbl::mapper
//!	which is exchangealbe with CGI interface bbl::mapper.
	extern bmapper mapper;

#endif // CGI / HTTP server

} // namespace bbl

#endif // BBL_MAPPER_HPP

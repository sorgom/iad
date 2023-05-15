//| ============================================================
//! @file
//!
//! Incomming request processing and storage.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef CGI_H
#define CGI_H

#include "stypes.h"

//	============================================================
//	HTTP server
#ifdef ISSRV
//!	parse paramter string from server.
//!	@param cp parameter string (GET / POST)
//!	@param bp inicates POST method
//!	@param sid current SID (must be saved separately)
	void srv_params(t_cc cp, bool bp, const s_cc& sid);

//	============================================================
//	CGI interface
#else
//!	Load parameters from CGI GET / POST.
	void cgi_params();
#endif
//	============================================================
//	common

//!	check if all parameters of a masked "params" string are matched.
	bool check_params(t_cc);

//!	clear cgi container form all non system (i.e. form) entries.
	void clear_cgi();

//	============================================================
//	session id creation
//!	SID creation function prototype.
	typedef t_fcc (t_sid_func)();

//!	definition of individual SID creation function.
	void set_sid_func(t_sid_func*);

//!	cgi environment SID creation function.
	t_fcc cgi_sid();

//!	simple time based SID creation function.
	t_fcc time_sid();

//!	load SID from tmp file.
	void load_sid(s_cc&);

//!	save SID to tmp file.
	void save_sid(const s_cc&);

//	============================================================
//	password
//!	password evaluation function prototype.
	typedef bool (t_pwd_eval_func)(t_cc);

//!	password saver function prototype.
	typedef void(t_pwd_save_func)(t_cc);

//!	set password functions.
	void set_pwd_funcs(t_pwd_eval_func*, t_pwd_save_func* = 0);

//!	dummy password evaluation function.
//!	password = anything but empty
	bool pwd_eval_any(t_cc);




#endif // CGI_H

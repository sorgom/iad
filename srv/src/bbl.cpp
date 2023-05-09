//| ============================================================
//! @file
//!
//! Predefinitions of commonly used language keys.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#include "bbl.hpp"


namespace bbl
{

//	Date
	t_cc c_Date 	= "Date";
	t_cc c_Day 		= "Day";
	t_cc c_Month 	= "Month";
	t_cc c_Year 	= "Year";

//	Month names (short)
	t_cc c_MonthShort[12] = {
		"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
	};

//	Time
	t_cc c_Time 	= "Time";
	t_cc c_Hour 	= "Hour";
	t_cc c_Minute 	= "Minute";
	t_cc c_Second 	= "Second";

//	Other common
	t_cc c_Activate		= "Activate";
	t_cc c_Active		= "Active";
	t_cc c_Add			= "Add";
	t_cc c_Address		= "Address";
	t_cc c_Clear		= "Clear";
	t_cc c_Deactivate	= "Deactivate";
	t_cc c_Downstream	= "Downstream";
	t_cc c_Edit			= "Edit";
	t_cc c_Error		= "Error";
	t_cc c_Name			= "Name";
	t_cc c_No			= "No";
	t_cc c_Off			= "Off";
	t_cc c_On			= "On";
	t_cc c_Password		= "Password";
	t_cc c_Remove		= "Remove";
	t_cc c_Reset		= "Reset";
	t_cc c_Save			= "Save";
	t_cc c_Server		= "Server";
	t_cc c_Setup 		= "Setup";
	t_cc c_Successful	= "Successful";
	t_cc c_Update		= "Update";
	t_cc c_Upstream		= "Upstream";
	t_cc c_Yes			= "Yes";

//	More specialized
	t_cc c_Trusted_Nodes	= "Trusted Nodes";
} // namespace bbl

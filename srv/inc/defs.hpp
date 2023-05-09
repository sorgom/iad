//| ============================================================
//! @file
//!
//!	Centralized system behaviour relevant definitions.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef DEFS_HPP
#define DEFS_HPP

//!	dml output buffer expansion step size
#define c_BUF_STEP_DML_OUT 1024

//!	dml temp buffer expansion step size
#define c_BUF_STEP_DML_TMP 256

//!	file read buffer size.
#define uc_file_BUFFER_SIZE 1024

//! line read buffer size
#define c_LINE_BUFFER_SIZE 512

//!	parser output buffer size
#define c_OUT_BUFFER_SIZE 1024

//!	configuration key and tag buffer size.
//!	This is dangerous:
//!	if user defined key exceeds: crash of system.
//!	So: don't size too small
#define c_CONFIG_KEY_BUFFER_SIZE 128

#define c_BUF_STEP_DCP_TMP 64
#define c_BUF_STEP_BFARG 10
#define c_BUF_STEP_CONFIG_CONTENT 128

//!	NOB buffer step size (cmapper).
#define c_BUF_STEP_NOB 64

//!	html parameters cmapper step size.
#define c_BUF_STEP_HTML_IN 64

//!	file name assembly e.g. "common" "-" "de" ".dat".
#define c_BUF_STEP_FILE_NAME 32

//!	file name and directory buffer step.
#define c_BUF_STEP_FILE_PATH 64

//!	file read block size.
//	#define c_FILE_READ_BLOCK_SIZE 64
#define c_FILE_READ_BLOCK_SIZE 128

//!	directory separator.
#define c_DIR_SEP '/'

//!	language key / vale mapper expansion step size.
#define c_BUF_STEP_CMAPPER 16

//!	language key / value mapper expansion step size.
#define c_BUF_STEP_BBL 128

//!	proc finder buffer step
#define c_BUF_STEP_FCPROC 16

//!	tag attributes mapper expansion step size.
#define c_BUF_STEP_ATTRIBS 10

//!	tag attributes string expansion step size.
#define c_BUF_STEP_ATTRIBS_STR 128

//!	available languages mapper expansion step size.
#define c_BUF_STEP_LANGS 10

//!	language short name (e.g. "de") maximum size.
#define c_LANG_SHORT_MAXSIZE 5

//!	buffer size for language dependent id (e.g. "lang_en").
#define c_LANG_ID_MAXSIZE 10

//!	available layouts mapper expansion step size.
#define c_BUF_STEP_LAYOUTS 10

//!	dcp variables mapper expansion step size.
#define c_BUF_STEP_DCP_VARS 20

//!	dcp page names buffer expansion step size.
#define c_BUF_STEP_DCP_PAGES 8

//! SID (remote ip + remote port) cbuffer expansion step size.
#define c_BUF_STEP_SID 24

//!	integer string representation expansion step size.
#define c_BUF_STEP_INT 10

//!	string size enough to contain a double sprintf
#define c_SIZE_SPRINTF_DOUBLE 32

//!	database names buffer expansion step size.
#define c_BUF_STEP_DBNAMES 5

//	============================================================
//	SPECIALIZED
//! the order:
//!	if defined: lay-lang.dat (common-de.dat)
//! otherwise : lang-lay.dat (de-common.dat)
#define c_BBL_LAY_LANG

//	============================================================
//	AHTM

//!	number of CASE levels allowed in AHTM files
#define c_AHTM_IF_LEVELS 5

//!	maximum number of pages that are parallelly open.
#define c_MAX_DCP_OPEN_PAGES 8

#endif // _H

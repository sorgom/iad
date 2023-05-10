#!/bin/sh
# ---------------------------------------------------------------------------
# 
#  Module   :
# 
#  Author   : Manfred Sorgo
#  Email    : ms@sorgo.de
# 
# ---------------------------------------------------------------------------
# 

if test ! -z "$1"; then
	DBNS="$*"
else	
	DBNS="user runtime"
	ARC=dbs.tar.gz
fi	
for DBN in $DBNS; do
	DB=$DBN.db
	rm -f $DB
	echo ".read $DBN.sql" | sqlite3 $DB
	chmod 666 $DB
	DBS="$DBS $DB"
done

if test ! -z "$ARC"; then
	tar cfz $ARC $DBS
fi	


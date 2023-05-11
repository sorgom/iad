#!/bin/bash

source `dirname $0`/env.sh

for DIR in $dbdir $htmdir $langdir; do
	if test -z "$DIR"; then continue; fi
	if test ! -d $DIR; then continue; fi
	if test ! -f $DIR/reset.sh; then continue; fi
	cd $DIR
	pwd
	if test -f reset.sh; then ./reset.sh; fi
done

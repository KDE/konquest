#! /bin/sh
$EXTRACTRC `find . -name \*.rc -o -name \*.ui` >> rc.cpp
$XGETTEXT `find . -name \*.cc -o -name \*.cpp ` -o $podir/konquest.pot

#! /bin/sh
$EXTRACTRC *.rc *.ui >> rc.cpp
$XGETTEXT `find . -name \*.cc -o -name \*.cpp ` -o $podir/konquest.pot

#! /bin/sh
$EXTRACTRC *.rc *.ui >> rc.cpp
$XGETTEXT *.cc *.cpp dialogs/*.cc rc.cpp -o $podir/konquest.pot

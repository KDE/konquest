#! /bin/sh
$EXTRACTRC *.rc *.ui >> rc.cpp
$XGETTEXT *.cc *.cpp maps/*.cc dialogs/*.cc rc.cpp -o $podir/konquest.pot

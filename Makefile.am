SUBDIRS = pics 

INCLUDES= -I$(top_srcdir)/libkdegames $(all_includes)
KDE_ICON = konquest

bin_PROGRAMS = konquest
konquest_SOURCES = Konquest.cc gameboard.cc gamecore.cc int_validator.cc \
	mainwin.cc map_widget.cc minimap.cc newgamedlg.cc planet_info.cc \
	gameenddlg.cc  scoredlg.cc fleetdlg.cc
konquest_LDFLAGS = $(all_libraries) $(KDE_RPATH)
konquest_LDADD = $(LIB_KDEGAMES)
konquest_DEPENDENCIES = $(LIB_KDEGAMES_DEP)


METASOURCES = gameboard.moc gamecore.moc int_validator.moc mainwin.moc \
    map_widget.moc minimap.moc newgamedlg.moc planet_info.moc scoredlg.moc \
	gameenddlg.moc fleetdlg.moc

xdg_apps_DATA = konquest.desktop

rcdir = $(kde_datadir)/konquest
rc_DATA = konquestui.rc

messages: rc.cpp
	$(XGETTEXT) rc.cpp *.cc -o $(podir)/konquest.pot

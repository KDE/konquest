SUBDIRS = pics 

INCLUDES= -I$(top_srcdir)/libkdegames $(all_includes)
KDE_ICON = konquest

bin_PROGRAMS = konquest
konquest_SOURCES = Konquest.cc gameboard.cc gamecore.cc int_validator.cc \
	mainwin.cc map_widget.cc minimap.cc newgamedlg.cc planet_info.cc \
	gameenddlg.cc  scoredlg.cc fleetdlg.cc newGameDlg_ui.ui
konquest_LDFLAGS = $(all_libraries) $(KDE_RPATH)
konquest_LDADD = $(LIB_KDEGAMES)
konquest_DEPENDENCIES = $(LIB_KDEGAMES_DEP)


METASOURCES = AUTO

xdg_apps_DATA = konquest.desktop

rcdir = $(kde_datadir)/konquest
rc_DATA = konquestui.rc

messages: rc.cpp
	$(XGETTEXT) rc.cpp *.cc -o $(podir)/konquest.pot

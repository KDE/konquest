
# set the include path for X, qt and KDE
INCLUDES= $(all_includes)
# claim, which subdirectories you want to install
SUBDIRS = pics docs

####### This part is very khexdit specific
# you can add here more. This one gets installed 
bin_PROGRAMS = 	konquest

# Which sources should be compiled for khexdit.
konquest_SOURCES = Konquest.cc gameboard.cc gamecore.cc int_validator.cc \
	mainwin.cc map_widget.cc minimap.cc newgamedlg.cc planet_info.cc \
	gameenddlg.cc  scoredlg.cc

# the library search path
konquest_LDFLAGS = $(all_libraries) $(KDE_RPATH)

# the libraries to link against. Be aware of the order. First the libraries,
# that depend on the following ones.
konquest_LDADD   =     $(LIB_KDEUI)

# this option you can leave out. Just, if you use "make dist", you need it
noinst_HEADERS = gameboard.h gamecore.h images.h int_validator.h mainwin.h map_widget.h minimap.h newgamedlg.h planet_info.h gameenddlg.h

METASOURCES =	gameboard.moc gamecore.moc int_validator.moc mainwin.moc \
      map_widget.moc minimap.moc newgamedlg.moc planet_info.moc scoredlg.moc \
	gameenddlg.moc

messages:
	$(XGETTEXT) -C -ki18n -x $(includedir)/kde.pot $(konquest_SOURCES) && mv messages.po ../po/konquest.pot

kde_icon_DATA = konquest.xpm

appsdir = $(kde_appsdir)/Games
apps_DATA = Konquest.kdelnk

install-data-local: 
	$(mkinstalldirs) $(kde_minidir)
	$(INSTALL_DATA) $(srcdir)/mini-konquest.xpm $(kde_minidir)/konquest.xpm
	$(mkinstalldirs) $(kde_icondir)/large
	$(INSTALL_DATA) $(srcdir)/large-konquest.xpm $(kde_icondir)/large/konquest.xpm

# remove ALL you have installed in install-data-local or install-exec-local
uninstall-local:
	-rm -f $(kde_minidir)/konquest.xpm
	-rm -f $(kde_icondir)/large/konquest.xpm

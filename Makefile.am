
# this 10 paths are KDE specific. Use them:
# kde_htmldir       Where your docs should go to. (contains lang subdirs)
# kde_appsdir       Where your application file (.kdelnk) should go to. 
# kde_icondir       Where your icon should go to.
# kde_minidir       Where your mini icon should go to.
# kde_datadir       Where you install application data. (Use a subdir)
# kde_locale        Where translation files should go to.(contains lang subdirs)
# kde_cgidir        Where cgi-bin executables should go to.
# kde_confdir       Where config files should go to.
# kde_mimedir       Where mimetypes should go to.
# kde_toolbardir    Where general toolbar icons should go to.
# kde_wallpaperdir  Where general wallpapers should go to.

# just set the variable
APPSDIR = $(kde_appsdir)/Games
# set the include path for X, qt and KDE
INCLUDES= @all_includes@
# claim, which subdirectories you want to install
SUBDIRS = pics docs

####### This part is very khexdit specific
# you can add here more. This one gets installed 
bin_PROGRAMS = 	konquest

# Which sources should be compiled for khexdit.
konquest_SOURCES = Konquest.cc gameboard.cc gamecore.cc int_validator.cc \
	mainwin.cc map_widget.cc minimap.cc newgamedlg.cc planet_info.cc gameenddlg.cc  scoredlg.cc

# the library search path
konquest_LDFLAGS = $(all_libraries) $(KDE_RPATH)

# the libraries to link against. Be aware of the order. First the libraries,
# that depend on the following ones.
konquest_LDADD   = -lkdeui -lkdecore -lqt -lX11 -lXext @LIBSOCKET@     

# this option you can leave out. Just, if you use "make dist", you need it
noinst_HEADERS = gameboard.h gamecore.h images.h int_validator.h mainwin.h map_widget.h minimap.h newgamedlg.h planet_info.h gameenddlg.h

# just to make sure, automake makes them 
BUILTSOURCES =	gameboard.moc gamecore.moc int_validator.moc mainwin.moc \
      map_widget.moc minimap.moc newgamedlg.moc planet_info.moc scoredlg.moc

# if you "make distclean", this files get removed. If you want to remove
# them while "make clean", use CLEANFILES
DISTCLEANFILES = $(BUILTSOURCES)

messages:
	$(XGETTEXT) -C -ki18n -x $(includedir)/kde.pot $(konquest_SOURCES) && mv messages.po ../po/konquest.pot

# just install datas here. Use install-exec-data for scripts and etc.
# the binary itself is already installed from automake
# use mkinstalldirs, not "install -d"
# don't install a list of file. Just one file per install.
# if you have more of them, create a subdirectory with an extra Makefile 
install-data-local: 
	$(mkinstalldirs) $(APPSDIR)
	$(INSTALL_DATA) Konquest.kdelnk $(APPSDIR)
	$(mkinstalldirs) $(kde_icondir)
	$(INSTALL_DATA) konquest.xpm $(kde_icondir)
	$(mkinstalldirs) $(kde_minidir)
	$(INSTALL_DATA) mini-konquest.xpm $(kde_minidir)/konquest.xpm

# remove ALL you have installed in install-data-local or install-exec-local
uninstall-local:
	-rm -f $(APPSDIR)/khexdit.kdelnk
	-rm -f $(kde_icondir)/khexdit.xpm
	-rm -f $(kde_minidir)/khexdit.xpm

# add a dependency for every moc file to be full portable
# I've added a key binding to emacs for this. 
gameboard.cc:gameboard.moc
gameboard.moc: gameboard.h
	$(MOC) gameboard.h -o gameboard.moc

gamecore.cc:gamecore.moc
gamecore.moc: gamecore.h
	$(MOC) gamecore.h -o gamecore.moc

int_validator.cc:int_validator.moc
int_validator.moc: int_validator.h
	$(MOC) int_validator.h -o int_validator.moc

mainwin.cc:mainwin.moc
mainwin.moc: mainwin.h
	$(MOC) mainwin.h -o mainwin.moc

map_widget.cc:map_widget.moc
map_widget.moc: map_widget.h
	$(MOC) map_widget.h -o map_widget.moc

minimap.cc:minimap.moc
minimap.moc: minimap.h
	$(MOC) minimap.h -o minimap.moc

newgamedlg.cc:newgamedlg.moc
newgamedlg.moc: newgamedlg.h
	$(MOC) newgamedlg.h -o newgamedlg.moc

planet_info.cc:planet_info.moc
planet_info.moc: planet_info.h
	$(MOC) planet_info.h -o planet_info.moc

gameenddlg.cc:gameenddlg.moc
gameenddlg.moc: gameenddlg.h
	$(MOC) gameenddlg.h -o gameenddlg.moc

scoredlg.cc:scoredlg.moc
scoredlg.moc: scoredlg.h
	$(MOC) scoredlg.h -o scoredlg.moc


project(konquest)

add_subdirectory( pics ) 

include_directories( ${CMAKE_SOURCE_DIR}/libkdegames  )


########### next target ###############

set(konquest_SRCS 
   Konquest.cc 
   gameboard.cc 
   gamecore.cc 
   int_validator.cc 
   mainwin.cc 
   map_widget.cc 
   minimap.cc 
   newgamedlg.cc 
   planet_info.cc 
   gameenddlg.cc 
   scoredlg.cc 
   fleetdlg.cc )

kde4_automoc(${konquest_SRCS})

kde4_add_ui3_files(konquest_SRCS newGameDlg_ui.ui )

kde4_add_executable(konquest ${konquest_SRCS})

target_link_libraries(konquest  ${KDE4_KDEUI_LIBS} kdegames )

install_targets(/bin konquest )


########### install files ###############

install_files( ${XDG_APPS_DIR} FILES konquest.desktop )
install_files( ${DATA_INSTALL_DIR}/konquest FILES konquestui.rc )

kde4_install_icons( ${ICON_INSTALL_DIR}  hicolor )




#original Makefile.am contents follow:

#SUBDIRS = pics 
#
#INCLUDES= -I$(top_srcdir)/libkdegames $(all_includes)
#KDE_ICON = konquest
#
#bin_PROGRAMS = konquest
#konquest_SOURCES = Konquest.cc gameboard.cc gamecore.cc int_validator.cc \
#	mainwin.cc map_widget.cc minimap.cc newgamedlg.cc planet_info.cc \
#	gameenddlg.cc  scoredlg.cc fleetdlg.cc newGameDlg_ui.ui
#konquest_LDFLAGS = $(all_libraries) $(KDE_RPATH)
#konquest_LDADD = $(LIB_KDEGAMES)
#konquest_DEPENDENCIES = $(LIB_KDEGAMES_DEP)
#
#
#METASOURCES = AUTO
#
#xdg_apps_DATA = konquest.desktop
#
#rcdir = $(kde_datadir)/konquest
#rc_DATA = konquestui.rc
#
#messages: rc.cpp
#	$(XGETTEXT) rc.cpp *.cc -o $(podir)/konquest.pot

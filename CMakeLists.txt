project(konquest)

add_subdirectory( pics ) 

########### next target ###############

set(konquest_SRCS 
   Konquest.cc 
   gamelogic.cc
   planet.cc
   fleet.cc
   player.cc
   map.cc
   sector.cc
   gamecore.cc 
   mainwin.cc 
   gameview.cc
   mapview.cc 
   mapscene.cc
   mapitems.cc
   minimapview.cc 
   newgamedlg.cc 
   scoredlg.cc 
   fleetdlg.cc )

kde4_add_ui_files(konquest_SRCS newGameDialog.ui )

set(konquest_icons
	hi16-app-konquest.png
	hi32-app-konquest.png
	hi48-app-konquest.png
	hi64-app-konquest.png
	hi128-app-konquest.png
)
kde4_add_win32_app_icon(konquest_SRCS ${konquest_icons})
kde4_add_executable(konquest ${konquest_SRCS})

target_link_libraries(konquest  ${KDE4_KDEUI_LIBS} kdegames )

install(TARGETS konquest  DESTINATION ${BIN_INSTALL_DIR} )


########### install files ###############

install( FILES konquest.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR} )
install( FILES konquestui.rc  DESTINATION  ${DATA_INSTALL_DIR}/konquest )

kde4_install_icons( ${ICON_INSTALL_DIR} )


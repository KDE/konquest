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

kde4_add_app_icon(konquest_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/hi*-app-konquest.png")
kde4_add_executable(konquest ${konquest_SRCS})

target_link_libraries(konquest  ${KDE4_KDEUI_LIBS} kdegames )

install(TARGETS konquest  ${INSTALL_TARGETS_DEFAULT_ARGS} )


########### install files ###############

install( PROGRAMS konquest.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR} )
install( FILES konquestui.rc  DESTINATION  ${DATA_INSTALL_DIR}/konquest )

kde4_install_icons( ${ICON_INSTALL_DIR} )



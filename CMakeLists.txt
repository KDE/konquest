project(konquest)

add_subdirectory( pics ) 

include_directories( ${CMAKE_SOURCE_DIR}/libkdegames  )


########### next target ###############

set(konquest_SRCS 
   Konquest.cc 
   gameboard.cc 
   planet.cc
   fleet.cc
   player.cc
   gamecore.cc 
   mainwin.cc 
   map_widget.cc 
   minimap.cc 
   newgamedlg.cc 
   planet_info.cc 
   gameenddlg.cc 
   scoredlg.cc 
   fleetdlg.cc )

kde4_automoc(${konquest_SRCS})

kde4_add_ui_files(konquest_SRCS newGameDlg_ui.ui )

kde4_add_executable(konquest ${konquest_SRCS})

target_link_libraries(konquest  ${KDE4_KDEUI_LIBS} kdegames )

install(TARGETS konquest  DESTINATION ${BIN_INSTALL_DIR} )


########### install files ###############

install( FILES konquest.desktop  DESTINATION  ${XDG_APPS_DIR} )
install( FILES konquestui.rc  DESTINATION  ${DATA_INSTALL_DIR}/konquest )

kde4_install_icons( ${ICON_INSTALL_DIR} )


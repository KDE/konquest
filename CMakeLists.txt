project(konquest)

add_subdirectory( pics )

########### next target ###############

set(konquest_SRCS 
    game.cpp
    localgame.cpp

    Konquest.cc
    planet.cc
    fleet.cc
    sector.cc
    mainwin.cc
    gameview.cc
    minimapview.cc

    map/map.cc
    map/mapview.cc
    map/mapscene.cc
    map/mapitems.cc

    dialogs/newgamedlg.cc
    dialogs/scoredlg.cc
    dialogs/fleetdlg.cc

    players/computerplayer.cpp
    players/localplayer.cpp
    players/neutralplayer.cpp
    players/player.cpp
)

kde4_add_ui_files(konquest_SRCS dialogs/newGameDialog.ui )

kde4_add_app_icon(konquest_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/hi*-app-konquest.png")
kde4_add_executable(konquest ${konquest_SRCS})

target_link_libraries(konquest  ${KDE4_KDEUI_LIBS} kdegames )

install(TARGETS konquest  ${INSTALL_TARGETS_DEFAULT_ARGS} )


########### install files ###############

install( PROGRAMS konquest.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR} )
install( FILES konquestui.rc  DESTINATION  ${DATA_INSTALL_DIR}/konquest )

kde4_install_icons( ${ICON_INSTALL_DIR} )



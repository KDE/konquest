ecm_setup_version(${KONQUEST_VERSION}
    VARIABLE_PREFIX KONQUEST
    VERSION_HEADER konquest_version.h
)

add_executable(konquest)

target_sources(konquest PRIVATE
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

    players/ai/becai/becai.cpp
    players/ai/becai/becai_gui.cpp
    players/ai/default/default.cpp
    players/ai/default/weak.cpp
    players/ai/default/weak_gui.cpp
    players/ai/default/normal.cpp
    players/ai/default/normal_gui.cpp
    players/ai/default/hard.cpp
    players/ai/default/hard_gui.cpp
    players/computerplayer.cpp
    players/localplayer.cpp
    players/localplayer_gui.cpp
    players/neutralplayer.cpp
    players/player.cpp
    players/player_gui.cpp
    players/spectatorplayer.cpp
    players/spectatorplayer_gui.cpp

    view/standingswidget.cpp

    konquest.qrc
)

if(KONQUEST_EXAMPLE_PLAYER)
    message(STATUS "KONQUEST_EXAMPLE_PLAYER")
    target_compile_definitions(konquest PRIVATE -DKONQUEST_EXAMPLE_PLAYER)
    target_sources(konquest PRIVATE
        players/ai/example/example.cpp
        players/ai/example/example_gui.cpp
    )
endif()

ki18n_wrap_ui(konquest dialogs/newGameDialog.ui)

file(GLOB ICONS_SRCS "${CMAKE_SOURCE_DIR}/icons/*-apps-konquest.png")
ecm_add_app_icon(konquest ICONS ${ICONS_SRCS})

target_link_libraries(konquest
    KF5KDEGames
    KF5::CoreAddons
    KF5::Crash
    KF5::DBusAddons
    KF5::GuiAddons
    KF5::I18n
    KF5::XmlGui
    KF5::WidgetsAddons
    Qt::Svg
)

install(TARGETS konquest  ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

install(PROGRAMS org.kde.konquest.desktop  DESTINATION  ${KDE_INSTALL_APPDIR})
install(FILES org.kde.konquest.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})

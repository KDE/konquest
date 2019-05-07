project(konquest)

cmake_minimum_required (VERSION 2.8.12 FATAL_ERROR)
set (QT_MIN_VERSION "5.9.0")
set (KF5_MIN_VERSION "5.15.0")

find_package(ECM 1.7.0 REQUIRED CONFIG)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

find_package(Qt5 ${QT_MIN_VERSION} REQUIRED NO_MODULE COMPONENTS Widgets Svg)
find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    Config
    CoreAddons
    Crash
    DBusAddons
    DocTools
    GuiAddons
    I18n
    XmlGui
    WidgetsAddons
)

find_package(KF5DocTools ${KF5_MIN_VERSION})

find_package(KF5KDEGames 4.9.0 REQUIRED)

include(FeatureSummary)
include(ECMAddAppIcon)
include(ECMInstallIcons)
include(KDEInstallDirs)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)

add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)


add_subdirectory( pics )
if (KF5DocTools_FOUND)
    add_subdirectory( doc )
endif()

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
)

option(KONQUEST_EXAMPLE_PLAYER "Include example player" OFF)
if(KONQUEST_EXAMPLE_PLAYER)
    message(STATUS "KONQUEST_EXAMPLE_PLAYER")
    add_definitions(-DKONQUEST_EXAMPLE_PLAYER)
    list (APPEND konquest_SRCS
                 players/ai/example/example.cpp
                 players/ai/example/example_gui.cpp)
endif()

ki18n_wrap_ui(konquest_SRCS dialogs/newGameDialog.ui )

file(GLOB ICONS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/*-apps-konquest.png")
ecm_add_app_icon(konquest_SRCS ICONS ${ICONS_SRCS})
add_executable(konquest ${konquest_SRCS})

target_link_libraries(konquest
    Qt5::Svg
    KF5::CoreAddons
    KF5::Crash
    KF5::DBusAddons
    KF5::GuiAddons
    KF5::I18n
    KF5::XmlGui
    KF5::WidgetsAddons
    KF5KDEGames
)

install(TARGETS konquest  ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})


########### install files ###############

install(PROGRAMS org.kde.konquest.desktop  DESTINATION  ${KDE_INSTALL_APPDIR})
install(FILES konquestui.rc  DESTINATION  ${KDE_INSTALL_KXMLGUI5DIR}/konquest)
install(FILES org.kde.konquest.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})

ecm_install_icons(ICONS
    128-apps-konquest.png
    16-apps-konquest.png
    22-apps-konquest.png
    32-apps-konquest.png
    48-apps-konquest.png
    64-apps-konquest.png
    DESTINATION ${KDE_INSTALL_ICONDIR} THEME hicolor
)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

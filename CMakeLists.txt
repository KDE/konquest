project(konquest)

cmake_minimum_required (VERSION 2.8.12 FATAL_ERROR)
set (QT_MIN_VERSION "5.3.0")

find_package(ECM 1.3.0 REQUIRED CONFIG)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

find_package(Qt5 ${QT_MIN_VERSION} REQUIRED NO_MODULE COMPONENTS Widgets Qml Quick QuickWidgets Svg Test)
find_package(KF5 REQUIRED COMPONENTS 
    CoreAddons
    Config
    ItemModels
    WidgetsAddons
    WindowSystem
    Config
    DBusAddons
    I18n
    ConfigWidgets
    TextWidgets
    XmlGui
    KIO
    NotifyConfig
    NewStuff
    KDELibs4Support)

find_package(KF5KDEGames 4.9.0 REQUIRED)
find_package(Phonon4Qt5 CONFIG REQUIRED)

include_directories(BEFORE ${PHONON_INCLUDES})

include(FeatureSummary)
include(ECMInstallIcons)
include(KDEInstallDirs)
include(KDECompilerSettings)
include(KDECMakeSettings)

add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)
add_definitions(-DTRANSLATION_DOMAIN="konquest")


add_subdirectory( pics )
add_subdirectory( doc )

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

kde4_add_app_icon(konquest_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/hi*-app-konquest.png")
add_executable(konquest ${konquest_SRCS})

target_link_libraries(konquest   KF5KDEGames KF5::KDELibs4Support Qt5::Svg)

install(TARGETS konquest  ${INSTALL_TARGETS_DEFAULT_ARGS} )


########### install files ###############

install( PROGRAMS konquest.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR} )
install( FILES konquestui.rc  DESTINATION  ${DATA_INSTALL_DIR}/konquest )

ecm_install_icons( ${ICON_INSTALL_DIR} )



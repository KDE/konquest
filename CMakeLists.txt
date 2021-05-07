cmake_minimum_required (VERSION 3.5 FATAL_ERROR)

# Release Service version, managed by release script
set (RELEASE_SERVICE_VERSION_MAJOR "21")
set (RELEASE_SERVICE_VERSION_MINOR "04")
set (RELEASE_SERVICE_VERSION_MICRO "1")
set (RELEASE_SERVICE_COMPACT_VERSION "${RELEASE_SERVICE_VERSION_MAJOR}${RELEASE_SERVICE_VERSION_MINOR}${RELEASE_SERVICE_VERSION_MICRO}")

# Bump KONQUEST_BASE_VERSION once new features are added
set(KONQUEST_BASE_VERSION "2.4")
set(KONQUEST_VERSION "${KONQUEST_BASE_VERSION}.${RELEASE_SERVICE_COMPACT_VERSION}")

project(konquest VERSION ${KONQUEST_VERSION})

set (QT_MIN_VERSION "5.12.0")
set (KF5_MIN_VERSION "5.68.0")

find_package(ECM ${KF5_MIN_VERSION} REQUIRED CONFIG)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

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

find_package(KF5KDEGames 7.3.0 REQUIRED)

include(FeatureSummary)
include(ECMAddAppIcon)
include(ECMInstallIcons)
include(KDEInstallDirs)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)
include(ECMSetupVersion)

add_definitions(
    -DQT_NO_CAST_FROM_ASCII
    -DQT_NO_CAST_TO_ASCII
    -DQT_NO_CAST_FROM_BYTEARRAY
    -DQT_NO_URL_CAST_FROM_STRING
    -DQT_USE_QSTRINGBUILDER
    -DQT_NO_FOREACH
    -DQT_NO_KEYWORDS
    -DQT_STRICT_ITERATORS
    -DQT_NO_NARROWING_CONVERSIONS_IN_CONNECT
    -DQT_DISABLE_DEPRECATED_BEFORE=0x050A00
    -DQT_DEPRECATED_WARNINGS_SINCE=0x060000
    -DKF_DISABLE_DEPRECATED_BEFORE_AND_AT=0x052E00
    -DKF_DEPRECATED_WARNINGS_SINCE=0x060000
)

add_subdirectory( pics )
add_subdirectory( doc )

########### next target ###############

ecm_setup_version(${KONQUEST_VERSION}
    VARIABLE_PREFIX KONQUEST
    VERSION_HEADER konquest_version.h
)

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
    konquest.qrc
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

ki18n_install(po)
kdoctools_install(po)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

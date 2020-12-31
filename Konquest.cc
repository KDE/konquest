/*
    SPDX-FileCopyrightText: 2003 Russell Steffen <rsteffen@bayarea.net>
    SPDX-FileCopyrightText: 2003 Stephan Zehetner <s.zehetner@nevox.org>
    SPDX-FileCopyrightText: 2006 Dmitry Suzdalev <dimsuz@gmail.com>
    SPDX-FileCopyrightText: 2006 Inge Wallin <inge@lysator.liu.se>
    SPDX-FileCopyrightText: 2006 Pierre Ducroquet <pinaraf@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/


#include <KAboutData>
#include <KCrash>
#include <Kdelibs4ConfigMigrator>
#include <KDBusService>
#include <KLocalizedString>

#include <QApplication>
#include <QCommandLineParser>
#include "mainwin.h"
#include "konquest_version.h"

int
main(int argc, char **argv)
{
    // Fixes blurry icons with fractional scaling
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication app(argc, argv);

    Kdelibs4ConfigMigrator migrate(QStringLiteral("konquest"));
    migrate.setConfigFiles(QStringList() << QStringLiteral("konquestrc"));
    migrate.setUiFiles(QStringList() << QStringLiteral("konquestui.rc"));
    migrate.migrate();
    KLocalizedString::setApplicationDomain("konquest");
    KAboutData aboutData( QStringLiteral("konquest"), i18n("Konquest"),
        QStringLiteral(KONQUEST_VERSION_STRING),
        i18n("Galactic Strategy Game"),
        KAboutLicense::GPL,
        i18n("Copyright (c) 1999-2013, Developers"),
        QString(),
        QStringLiteral("https://kde.org/applications/games/org.kde.konquest") );

    aboutData.addAuthor(i18n("Russ Steffen"), QString(), QStringLiteral("rsteffen@bayarea.net"));
    aboutData.addCredit(i18n("Stephan Zehetner"), i18n("Computer/AI Player"), QStringLiteral("s.zehetner@nevox.org"));
    aboutData.addCredit(i18n("Dmitry Suzdalev"), i18n("Port to KDE4"), QStringLiteral("dimsuz@gmail.com"));
    aboutData.addCredit(i18n("Inge Wallin"), i18n("Cleaning and bugfixing"), QStringLiteral("inge@lysator.liu.se"));
    aboutData.addCredit(i18n("Pierre Ducroquet"), i18n("Cleaning and bugfixing, maintenance"), QStringLiteral("pinaraf@gmail.com"));
    aboutData.addCredit(i18n("Sean D'Epagnier"), i18n("Gameplay Enhancements"), QStringLiteral("geckosenator@gmail.com"));

    aboutData.setOrganizationDomain(QByteArray("kde.org"));

    QCommandLineParser parser;
    KAboutData::setApplicationData(aboutData);
    KCrash::initialize();
    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);
    KDBusService service;

    if (app.isSessionRestored())
        kRestoreMainWindows<MainWindow>();
    else {
        MainWindow *w = new MainWindow;
        w->show();
    }
    app.setWindowIcon(QIcon::fromTheme(QStringLiteral("konquest")));
    return app.exec();
}

/*
    Copyright 2003 Russell Steffen <rsteffen@bayarea.net>
    Copyright 2003 Stephan Zehetner <s.zehetner@nevox.org>
    Copyright 2006 Dmitry Suzdalev <dimsuz@gmail.com>
    Copyright 2006 Inge Wallin <inge@lysator.liu.se>
    Copyright 2006 Pierre Ducroquet <pinaraf@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */


#include <KLocalizedString>
#include <KAboutData>
#include <KCrash>
#include <QApplication>
#include <KLocalizedString>
#include <QCommandLineParser>
#include <KDBusService>
#include <Kdelibs4ConfigMigrator>
#include "mainwin.h"

static const char description[] = I18N_NOOP("Galactic Strategy KDE Game");
#define KONQUEST_VERSION "2.3"

int
main(int argc, char **argv)
{
    QApplication app(argc, argv);

    Kdelibs4ConfigMigrator migrate(QStringLiteral("konquest"));
    migrate.setConfigFiles(QStringList() << QStringLiteral("konquestrc"));
    migrate.setUiFiles(QStringList() << QStringLiteral("konquestui.rc"));
    migrate.migrate();
    KLocalizedString::setApplicationDomain("konquest");
    KAboutData aboutData( "konquest", i18n("Konquest"),
        KONQUEST_VERSION, i18n(description), KAboutLicense::GPL,
        i18n("Copyright (c) 1999-2013, Developers"), "http://games.kde.org/konquest" );

    aboutData.addAuthor(i18n("Russ Steffen"), QString(), "rsteffen@bayarea.net");
    aboutData.addCredit(i18n("Stephan Zehetner"), i18n("Computer/AI Player"), "s.zehetner@nevox.org");
    aboutData.addCredit(i18n("Dmitry Suzdalev"), i18n("Port to KDE4"), "dimsuz@gmail.com");
    aboutData.addCredit(i18n("Inge Wallin"), i18n("Cleaning and bugfixing"), "inge@lysator.liu.se");
    aboutData.addCredit(i18n("Pierre Ducroquet"), i18n("Cleaning and bugfixing, maintenance"), "pinaraf@gmail.com");
    aboutData.addCredit(i18n("Sean D'Epagnier"), i18n("Gameplay Enhancements"), "geckosenator@gmail.com");

    aboutData.setOrganizationDomain(QByteArray("kde.org"));

    QCommandLineParser parser;
    KAboutData::setApplicationData(aboutData);
    KCrash::initialize();
    parser.addVersionOption();
    parser.addHelpOption();
    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);
    KDBusService service;

    if (app.isSessionRestored())
        RESTORE(MainWindow)
    else {
        MainWindow *w = new MainWindow;
        w->show();
    }
    app.setWindowIcon(QIcon::fromTheme(QStringLiteral("konquest")));
    return app.exec();
}

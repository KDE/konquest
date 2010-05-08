/*
    Copyright Russell Steffen <rsteffen@bayarea.net>
    Copyright Stephan Zehetner <s.zehetner@nevox.org>
    Copyright Dmitry Suzdalev <dimsuz@gmail.com>
    Copyright Inge Wallin <inge@lysator.liu.se>
    Copyright Pierre Ducroquet <pinaraf@gmail.com>

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
#include <kapplication.h>
#include <klocale.h>
#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <kstandarddirs.h>

#include "version.h"
#include "mainwin.h"


static const char description[] = I18N_NOOP("Galactic Strategy KDE Game");


int
main(int argc, char **argv)
{
    KAboutData aboutData( "konquest", 0, ki18n("Konquest"),
        KONQUEST_VERSION, ki18n(description), KAboutData::License_GPL,
        ki18n("Copyright (c) 1999-2010, Developers"), KLocalizedString(), "http://games.kde.org/konquest" );

    aboutData.addAuthor(ki18n("Russ Steffen"), KLocalizedString(), "rsteffen@bayarea.net");
    aboutData.addCredit(ki18n("Stephan Zehetner"), ki18n("Computer/AI Player"), "s.zehetner@nevox.org");
    aboutData.addCredit(ki18n("Dimitry Suzalev"), ki18n("Port to KDE4"));
    aboutData.addCredit(ki18n("Inge Wallin"), ki18n("Cleaning and bugfixing"), "inge@lysator.liu.se");
    aboutData.addCredit(ki18n("Pierre Ducroquet"), ki18n("Cleaning and bugfixing"), "pinaraf@gmail.com");
    aboutData.addCredit(ki18n("Sean D'Epagnier"), ki18n("Gameplay Enhancements"), "geckosenator@gmail.com");
    
    KCmdLineArgs::init( argc, argv, &aboutData );

    KApplication a;
    KGlobal::locale()->insertCatalog("libkdegames");

    if (a.isSessionRestored())
        RESTORE(MainWindow)
    else {
        MainWindow *w = new MainWindow;
        w->show();
    }
    return a.exec();
}

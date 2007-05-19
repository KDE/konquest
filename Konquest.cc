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
    KAboutData aboutData( "konquest", I18N_NOOP("Konquest"),
        KONQUEST_VERSION, description, KAboutData::License_GPL,
        I18N_NOOP("Copyright (c) 1999-2006, Developers"));

    aboutData.addAuthor("Russ Steffen", 0, "rsteffen@bayarea.net");
    aboutData.addCredit("Stephan Zehetner", "Computer/AI Player", "s.zehetner@nevox.org");
    aboutData.addCredit("Dimitry Suzalev", "Port to KDE4");
    aboutData.addCredit("Inge Wallin", "Cleaning and bugfixing", "inge@lysator.liu.se");
    aboutData.addCredit("Pierre Ducroquet", "Cleaning and bugfixing", "pinaraf@gmail.com");
    
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

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

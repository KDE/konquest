#include <kapplication.h>
#include <klocale.h>
#include <kcmdlineargs.h>
#include <kaboutdata.h>

#include "version.h"
#include "mainwin.h"
#include "map_widget.h"

static const char description[] = I18N_NOOP("Galactic Strategy KDE Game");

int
main(int argc, char **argv)
{
    KAboutData aboutData( "konquest", I18N_NOOP("Konquest"),
        KONQUEST_VERSION, description, KAboutData::License_GPL,
        I18N_NOOP("Copyright (c) 1999-2001, Developers"));
    aboutData.addAuthor("Russ Steffen",0, "rsteffen@bayarea.net");
    KCmdLineArgs::init( argc, argv, &aboutData );

    KApplication a;
    QApplication::setGlobalMouseTracking( true );
    KGlobal::locale()->insertCatalogue("libkdegames");

    if (a.isRestored())
        RESTORE(MainWindow)
    else {
        MainWindow *w = new MainWindow;
        a.setMainWidget(w);
        w->show();
    }
    return a.exec();
}


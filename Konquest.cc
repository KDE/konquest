#include <qapplication.h>
#include <qpushbutton.h>
#include <qwidget.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qfont.h>

#include <kapplication.h>
#include <ktoolbar.h>
#include <klocale.h>
#include <kmenubar.h>
#include <kcmdlineargs.h>
#include <kaboutdata.h>


#include "version.h"
#include "mainwin.h"
#include "map_widget.h"

static const char *description= I18N_NOOP("Galactic Strategy KDE Game");
static const char *text = I18N_NOOP("By Russ Steffen <rsteffen@bayarea.net>\n");

int 
main(int argc, char **argv)
{
    KAboutData aboutData( "konquest", I18N_NOOP("Konquest"), 
        KONQUEST_VERSION, description, KAboutData::License_GPL, 
        "Copyright (c) 1999-2001, Developers", text);
    aboutData.addAuthor("Russ Steffen",0, "rsteffen@bayarea.net");
    KCmdLineArgs::init( argc, argv, &aboutData );

    KApplication a;

    KGlobal::locale()->insertCatalogue("libkdegames");      

    MainWindow *w = new MainWindow();

    a.setMainWidget(w);

    QApplication::setGlobalMouseTracking( true );

    return a.exec();
}


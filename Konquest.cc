#include <qapplication.h>
#include <qpushbutton.h>
#include <qwidget.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qfont.h>

#include <kapp.h>
#include <ktmainwindow.h>
#include <ktoolbar.h>
#include <klocale.h>
#include <kmenubar.h>
#include <kcmdlineargs.h>
#include <kaboutdata.h>


#include "version.h"
#include "mainwin.h"
#include "map_widget.h"

static const char *description = I18N_NOOP("KDE Game");

int 
main(int argc, char **argv)
{
    KAboutData aboutData( "konquest", I18N_NOOP("Konquest"), 
        KONQUEST_VERSION, description, KAboutData::GPL, 
        "(c) 1999-2000, DEVELOPERS");
    aboutData.addAuthor("Russ Steffen",0, "rsteffen@ia.net");
    KCmdLineArgs::init( argc, argv, &aboutData );

    KApplication a;
    
    MainWindow *w = new MainWindow();

    a.setMainWidget(w);

    return a.exec();
}


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

#include "version.h"
#include "mainwin.h"
#include "map_widget.h"

static const char *description = I18N_NOOP("KDE Game");

int 
main(int argc, char **argv)
{
    KCmdLineArgs::init(argc, argv, "konquest", description, KONQUEST_VERSION);

    KApplication a;
    
    MainWindow *w = new MainWindow();

    a.setMainWidget(w);

    return a.exec();
}


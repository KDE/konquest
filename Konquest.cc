#include <qapp.h>
#include <qpushbt.h>
#include <qwidget.h>
#include <qmenubar.h>
#include <qpopmenu.h>
#include <qfont.h>

#include <kapp.h>
#include <ktopwidget.h>
#include <ktoolbar.h>
#include <kmenubar.h>

#include "mainwin.h"
#include "map_widget.h"


int 
main(int argc, char **argv)
{

    KApplication a(argc, argv, "konquest" );
    
    MainWindow *w = new MainWindow();

    a.setMainWidget(w);

    return a.exec();
}


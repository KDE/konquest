#include <qapplication.h>
#include <qpushbutton.h>
#include <qwidget.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qfont.h>

#include <kapp.h>
#include <ktmainwindow.h>
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


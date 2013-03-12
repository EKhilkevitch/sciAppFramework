
// =========================================

#include <QApplication>
#include "exampleLoadMonitor.h"

// =========================================

int main( int argc, char **argv )
{
  QApplication app(argc,argv);

  exampleLoadMonitor Widget;
  Widget.show();

  return app.exec();  
}

// =========================================


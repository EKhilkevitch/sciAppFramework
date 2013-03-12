
#include <QtGui>

#include "sciAppFramework/waitWidget.h"

using namespace sciAppFramework;

struct testWaitWidget : public waitWidget
{
  testWaitWidget() { initWidget(); }
};

int main( int argc, char **argv )
{
  QApplication app(argc,argv);

  QWidget Widget;
  Widget.show();
  Widget.resize(500,700);

  testWaitWidget Wait;
  Wait.show();

  return app.exec(); 
}



#include <QtGui>

#include "sciAppFramework/inputWidget.h"
#include "sciAppFramework/multiInputWidget.h"
#include "sciAppFramework/mainWidget.h"
#include "sciAppFramework/controlWidget.h"
#include "sciAppFramework/outputWidget.h"
#include "sciAppFramework/measurementThread.h"

#include "scigraphics/qt4/qt4plotmanager.h"

using namespace sciAppFramework;

struct testMainWidget : public measureMainWidget
{
  testMainWidget() { initWidget(); }
      
  controlWidget* createControlWidget() 
  { 
    struct testControlWidget : public measureControlWidget
    {
      testControlWidget() { initWidget(); }
    };
    return new testControlWidget(); 
  }

  outputWidget* createOutputWidget() 
  { 
    struct testOutputWidget : public outputWidget
    {
      testOutputWidget() 
      { 
        appendOutputWidgetItem( new simpleOutputWidgetItem( new QLabel("L1"), new QLabel("S1"), "T1" ) );
        appendOutputWidgetItem( new plotManagerOutputWidgetItem( new qt4plotManager(3), "Plot" ) );
      }
    };
    return new testOutputWidget(); 
  }
  measurementThread* createMeasurementThread( const measurementParameters &Parameters ) { return new measurementThread(Parameters); }
};

#if 0
struct o1 : public QObject {};
struct w1 : public QWidget {};
struct mix : virtual public o1, virtual public w1 {  };
#endif

int main( int argc, char **argv )
{
  QApplication app(argc,argv);

  testMainWidget Widget;
  Widget.show();

  return app.exec(); 
}




#include "sciAppFramework/inputWidget.h"
#include "sciAppFramework/multiInputWidget.h"
#include "sciAppFramework/measureMainWidget.h"
#include "sciAppFramework/measureControlWidget.h"
#include "sciAppFramework/outputWidget.h"
#include "sciAppFramework/measurementThread.h"
#include "sciAppFramework/main.h"

#include "scigraphics/qt4/manager.h"

#include <QLabel>

using namespace sciAppFramework;


struct testMainWidget : public measureMainWidget
{
  testMainWidget() { initWidget(); }
  
  QString appTitle() const { return "Test application"; }

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
        appendOutputWidgetItem( new plotManagerOutputWidgetItem( new scigraphics::qt4plotManager(3), "Plot" ) );
        appendOutputWidgetItem( new simpleOutputWidgetItem( new QLabel("L1"), new QLabel("S1"), "T1" ) );
      }
    };
    return new testOutputWidget(); 
  }
  measurementThread* createMeasurementThread( const measurementParameters *Parameters ) 
  { 
    struct testThread : public measurementThread
    {
      testThread( const measurementParameters *P ) : measurementThread(P) {}
    };
    return new testThread(Parameters); 
  }
};

int main( int argc, char **argv )
{
  return sciAppFramework::main<testMainWidget>( argc, argv );
}



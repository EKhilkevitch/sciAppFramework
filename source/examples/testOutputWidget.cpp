
#include "sciAppFramework/outputWidget.h"
#include "sciAppFramework/outputSettingsWidget.h"

#include <QApplication>
#include <QLabel>

using namespace sciAppFramework;


struct testOutputWidget : public outputWidget
{

  testOutputWidget() 
  {
    appendOutputWidgetItem( new simpleOutputWidgetItem( new QLabel("L1"), new QLabel("S1"), "T1" ) );
    appendOutputWidgetItem( new simpleOutputWidgetItem( new QLabel("L2"), new QLabel("S2"), "T2" ) );
    appendOutputWidgetItem( new simpleOutputWidgetItem( new QLabel("L3"), new QLabel("S3"), "T3" ) );
  }

  ~testOutputWidget() 
  { 
  }

};

int main( int argc, char **argv )
{
  QApplication app(argc,argv);

#if 0 // This code remove linking problem in mingw - ????
  qt4plotManager(1);
  plotManagerOutputWidgetItem( NULL, "P1" );
#endif

  testOutputWidget OutputWidget;
  OutputWidget.show();

  outputSettingsWidget SettingsWidget( &OutputWidget );
  SettingsWidget.show();
  
  return app.exec(); 
}


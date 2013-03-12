
#include <QtGui>

#include "sciAppFramework/outputWidget.h"
#include "sciAppFramework/settingsWidget.h"

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

  testOutputWidget OutputWidget;
  OutputWidget.show();

  settingsWidget SettingsWidget( &OutputWidget );
  SettingsWidget.show();
  
  return app.exec(); 
}


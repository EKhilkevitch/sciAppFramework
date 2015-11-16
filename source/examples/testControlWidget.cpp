
#include <QtGui>

#include "sciAppFramework/measureControlWidget.h"
#include "sciAppFramework/multiInputWidget.h"
#include "sciAppFramework/inputWidget.h"
#include "sciAppFramework/labelInputWidget.h"

using namespace sciAppFramework;


struct testControlWidget : public measureControlWidget
{
  multiInputWidget* createMultiInputWidget()
  {
    struct testInputWidget : public multiInputWidget
    {
      testInputWidget() 
      {
        addInputWidget<labelEditWidget>( "Edit1", "Edit field1" );
        addInputWidget<labelEditWidget>( "Edit2", "Edit field2", "zyz" );
        addInputWidget<labelSpinWidget>( "Edit3", "Edit field3" );
        addInputWidget<labelEditWidget>( "Edit1", "Edit field1, replace" );
      }
    };

    return new testInputWidget();
  }
  
  QSettings* createSettings() 
  { 
    return new QSettings("./testControlWidget.conf",QSettings::IniFormat); 
  }

  QStringList saveButtonsTextAndNames() const { return QStringList(); }


  testControlWidget() : measureControlWidget()
  {
    initWidget();
    QSettings *Settings = createSettings(); 
    controlWidget::loadSettings( Settings );
    delete Settings;
  }

  ~testControlWidget() 
  { 
    QSettings *Settings = createSettings(); 
    saveSettings( Settings ); 
    delete Settings;
  }

};

int main( int argc, char **argv )
{
  QApplication app(argc,argv);

  testControlWidget Widget;
  Widget.show();
  
  return app.exec(); 
}


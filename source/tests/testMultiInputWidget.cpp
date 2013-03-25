
#include <QtGui>

#include "sciAppFramework/inputWidget.h"
#include "sciAppFramework/multiInputWidget.h"

using namespace sciAppFramework;

struct testInputWidget : public multiInputWidget
{
  QSettings* createSettings() { return new QSettings("./testMultiInputWidget.conf",QSettings::IniFormat); }
  void loadSettings() 
  { 
    QSettings *Settings = createSettings(); 
    multiInputWidget::loadSettings( Settings ); 
    delete Settings;
  }

  ~testInputWidget()  
  { 
    QSettings *Settings = createSettings(); 
    multiInputWidget::saveSettings( Settings ); 
    delete Settings;
  }
};

int main( int argc, char **argv )
{
  QApplication app(argc,argv);
  
  testInputWidget Widget;

  labelComboWidget *Combo = new labelComboWidget("Combo",&Widget);
  Combo->addItem("Item1");
  Combo->addItem("Item2");

  multiInputWidget *SubWidget = new multiInputWidget();
  SubWidget->addInputWidget<labelEditWidget>("EditS1","Sub edit 1",12);
  SubWidget->addInputWidget<labelEditWidget>("EditS2","Sub edit 2",13);

  Widget.addInputWidget( Combo );
  Widget.addInputWidget<labelEditWidget>("Edit1","This is a test input field",33);
  Widget.addInputWidget<labelEditWidget>("Edit2","This is a test input field #2",22);
  Widget.addInputWidget<labelPathEditWidget>("Path1","This is a test path field");
  Widget.addInputWidget<labelSpinWidget>("Spin1","This is a test spin",12);
  Widget.addLabel("This is a label");
  radioButtonWidget *Radio1 = Widget.addInputWidget<radioButtonWidget>("Radio1","A lot of radio buttons");
  Radio1->addItem("btn1");
  Radio1->addItem("btn2");
  Radio1->addItem("btn3");
  Radio1->addItem("btn4");
  Widget.addMultiInputWidget( "SW", "Sub widget", SubWidget );
  Widget.addInputWidget<checkBoxWidget>("Check1","This is a check box",true);
  Widget.loadSettings();
  Widget.show();

  qDebug() << "Edit1 = " << Widget.value<QString>("Edit1");
  qDebug() << "Spin1 = " << Widget.value<QString>("Spin1");
  qDebug() << "SW:EditS2 = " << Widget.value<QString>("SW:EditS2");

  return app.exec(); 
}



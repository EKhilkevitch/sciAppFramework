
#include <QtGui>

#include "sciAppFramework/inputWidget.h"
#include "sciAppFramework/multiInputWidget.h"
#include "sciAppFramework/comboMultiWidget.h"

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

  comboMultiWidget *ComboMultiWidget = new comboMultiWidget("Select widget:");
  ComboMultiWidget->setScrollArea( true );
  ComboMultiWidget->setSettingsName("ComboMW");
  ComboMultiWidget->addWidget( "L1", new QLabel("Label1") );
  ComboMultiWidget->addWidget( "L2", new QLabel("Label2\n\n\n\n\n\n\n\nText") );
  ComboMultiWidget->addWidget( "L3", inputWidget::create<labelEditWidget>("L3I","Text") );

  Widget.addInputWidget( Combo );
  Widget.addInputWidget<labelEditWidget>("Edit1","This is a test input field",33);
  Widget.addInputWidget<labelEditWidget>("Edit2","This is a test input field #2",22);
  Widget.addInputWidget<labelPathEditWidget>("Path1","This is a test path field");
  Widget.addInputWidget<labelSpinWidget>("Spin1","This is a test spin",12);
  Widget.addInputWidget<multilineEditWidget>( "MultiEdit", "Multiline edit", "This text is\nmultiline" );
  Widget.addLabel("This is a label");
  radioButtonWidget *Radio1 = Widget.addInputWidget<radioButtonWidget>("Radio1","A lot of radio buttons");
  Radio1->addItem("btn1");
  Radio1->addItem("btn2");
  Radio1->addItem("btn3");
  Radio1->addItem("btn4");


  multiInputWidget *SubWidget = new multiInputWidget();
  SubWidget->addInputWidget<labelEditWidget>("EditS1","Sub edit 1",12);
  SubWidget->addInputWidget<labelEditWidget>("EditS2","Sub edit 2",13);
  Widget.addBoxMultiInputWidget( "SW", "Sub widget", SubWidget );

  multiInputWidget *SubWidgetTab1 = new multiInputWidget();
  SubWidgetTab1->addInputWidget<labelEditWidget>("EditT1","Sub edit 1",12);
  SubWidgetTab1->addInputWidget<labelEditWidget>("EditT2","Sub edit 2",13);
  multiInputWidget *SubWidgetTab2 = new multiInputWidget();
  SubWidgetTab2->addInputWidget<labelEditWidget>("EditT3","Sub edit 3",12);
  Widget.addTabMultiInputWidget( "ST1", "Tab widget 1", SubWidgetTab1 );
  Widget.addTabMultiInputWidget( "ST2", "Tab widget 2", SubWidgetTab2 );

  Widget.addInputWidget<checkBoxWidget>( "Check1","This is a check box", true );
  Widget.addWidget( new QPushButton("Button") );
  Widget.addWidget( ComboMultiWidget );
  Widget.addLabel( "This is a label #2" );

  Widget.loadSettings();
  Widget.show();

  qDebug() << "Edit1 = " << Widget.value<QString>("Edit1");
  qDebug() << "Spin1 = " << Widget.value<QString>("Spin1");
  qDebug() << "SW:EditS2 = " << Widget.value<QString>("SW:EditS2");
  qDebug() << "Map: " << Widget.stringValues();

  return app.exec(); 
}



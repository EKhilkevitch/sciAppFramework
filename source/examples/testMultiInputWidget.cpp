
#include "sciAppFramework/inputWidget.h"
#include "sciAppFramework/multiInputWidget.h"
#include "sciAppFramework/stackedSelectorWidget.h"
#include "sciAppFramework/settingsStorage.h"

#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QDebug>

using namespace sciAppFramework;

struct testInputWidget : public multiInputWidget
{
  testInputWidget() :
    multiInputWidget( NULL, "TestMIW" )
  {
  }

  settingsStorage* createSettings() { return new fileSettingsStorage("./testMultiInputWidget.conf"); }
  void loadSettings() 
  { 
    settingsStorage *Settings = createSettings(); 
    multiInputWidget::loadSettings( Settings ); 
    delete Settings;
  }

  ~testInputWidget()  
  { 
    settingsStorage *Settings = createSettings(); 
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
  Combo->addItem("Item2 with very long text and this combo should show it full");

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
  Widget.addInputWidget<labelLongSpinWidget>("LongSpin1","This is a test long spin",12)->setMinimum( -100 );
  Widget.addInputWidget<labelDoubleSpinWidget>( "DoubleSpin1", "This is a test double spin", 1.5 )->setMinimum( -4.5 );
  Widget.addInputWidget<labelDoubleEditWidget>( "DoubleEdit1", "This is a test double edit", 3.5 )->setMinimum( -5.5 );
  Widget.addInputWidget<multilineEditWidget>( "MultiEdit", "Multiline edit", "This text is\nmultiline" );
  Widget.addLabel("This is a label");
  radioButtonWidget *Radio1 = Widget.addInputWidget<radioButtonWidget>("Radio1","A lot of radio buttons");
  Radio1->addItem("btn1");
  Radio1->addItem("btn2");
  Radio1->addItem("btn3");
  Radio1->setOrientation( Qt::Horizontal );

  multiInputWidget *SubWidget = new multiInputWidget( "SubWidget" );
  SubWidget->addInputWidget<labelEditWidget>("EditS1","Sub edit 1",12);
  SubWidget->addInputWidget<labelEditWidget>("EditS2","Sub edit 2",13);
  Widget.addBoxMultiInputWidget( "SW", "Sub widget", SubWidget );

  multiInputWidget *SubWidgetTab1 = new multiInputWidget( "SubWidgetTab1" );
  SubWidgetTab1->addInputWidget<labelEditWidget>("EditT1","Sub edit 1",12);
  SubWidgetTab1->addInputWidget<labelEditWidget>("EditT2","Sub edit 2",13);
  multiInputWidget *SubWidgetTab2 = new multiInputWidget( "SubWidgetTab2" );
  SubWidgetTab2->addInputWidget<labelEditWidget>("EditT3","Sub edit 3",14);
  Widget.addTabMultiInputWidget( "ST1", "Tab widget 1", SubWidgetTab1 );
  Widget.addTabMultiInputWidget( "ST2", "Tab widget 2", SubWidgetTab2 );
  Widget.addWidget( ComboMultiWidget );

  Widget.addInputWidget<checkBoxWidget>( "Check1","This is a check box", true );
  Widget.addWidget( new QPushButton("Button") );
  Widget.addLabel( "This is a label #2" );

  multiInputWidget *SubWidgetRb1 = new multiInputWidget( "SubWidgetRb1" );
  SubWidgetRb1->addInputWidget<labelSpinWidget>( "SpinR1", "Spin 1", 7 );
  SubWidgetRb1->addInputWidget<labelSpinWidget>( "SpinR2", "Spin 2", 8 );
  SubWidgetRb1->addInputWidget<labelSpinWidget>( "SpinR3", "Spin 3", 9 );
  multiInputWidget *SubWidgetRb2 = new multiInputWidget( "SubWidgetRb2" );
  SubWidgetRb2->addInputWidget<labelSpinWidget>( "SpinR4", "Spin 3", 10 );
  Widget.addRadioMultiInputWidget( "RBs", "Buttons", "RB1", "Radio Button 1", SubWidgetRb1 );
  Widget.addRadioMultiInputWidget( "RBs", "Buttons", "RB2", "Radio Button 2", SubWidgetRb2 );

  Widget.loadSettings();
  Widget.show();

  qDebug() << "Edit1 = " << Widget.value<QString>("Edit1");
  qDebug() << "Spin1 = " << Widget.value<QString>("Spin1");
  qDebug() << "DoubleSpin1 = " << Widget.value<double>("DoubleSpin1");
  qDebug() << "DoubleEdit1 = " << Widget.value<double>("DoubleEdit1");
  qDebug() << "RB1:SpinR2 = " << Widget.value<QString>("RB1:SpinR2");
  qDebug() << "RB1 = " << Widget.value<QString>("RB1");
  qDebug() << "SW:EditS2 = " << Widget.value<QString>("SW:EditS2");
  qDebug() << "Map: " << Widget.stringValues();

  return app.exec(); 
}



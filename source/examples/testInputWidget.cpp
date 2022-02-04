
#include "sciAppFramework/inputWidget.h"
#include "sciAppFramework/labelInputWidget.h"
#include "sciAppFramework/adaptedInputWidget.h"

#include <QApplication>
#include <QBoxLayout>
#include <QDebug>
#include <QDateTime>

using namespace sciAppFramework;

int main( int argc, char **argv )
{
  QApplication app(argc,argv);
  

  QWidget Widget;
  qDebug() << "main: Widget = " << &Widget;

  labelEditWidget *Edit1 = new labelEditWidget("Text1",&Widget), *Edit2 = new labelEditWidget("Label with very very very long text","abcd");
  labelEditWidget *Edit3 = new labelEditWidget("Text3");
  labelPathEditWidget *PathEdit1 = new labelPathEditWidget("PathEdit1");
  labelPathEditWidget *PathEdit2 = new labelPathEditWidget("PathEdit2");
  PathEdit2->setFileMode( QFileDialog::DirectoryOnly );
  PathEdit2->setText( "/tmp/evgeniy" );
  labelDoubleEditWidget *DoubleEdit1 = new labelDoubleEditWidget("DoubleEdit",0.3e-6);
  labelSpinWidget *Spin1 = new labelSpinWidget( "Spin1", 3 );
  labelDoubleSpinWidget *Spin2 = new labelDoubleSpinWidget( "Spin2", 1.3 );
  labelComboWidget *Combo1 = new labelComboWidget("Combo");
  Combo1->addItem("xxxx");
  Combo1->addItem("yyy");
  Combo1->addItem("zz");
  radioButtonWidget *Radio1 = new radioButtonWidget("Radio");
  Radio1->addItem("xxxx");
  Radio1->addItem("yyy");
  Radio1->addItem("zz");
  checkBoxWidget *Check1 = new checkBoxWidget("Check1");
  labelDateTimeWidget *DateTime1 = new labelDateTimeWidget( "Date and time", QDateTime::currentDateTime() );

  QBoxLayout *Layout = new QVBoxLayout();
  Layout->addWidget( Edit1 );
  Layout->addWidget( Edit2 );
  Layout->addWidget( PathEdit1 );
  Layout->addWidget( PathEdit2 );
  Layout->addWidget( DoubleEdit1 );
  Layout->addWidget( Spin1 );
  Layout->addWidget( Spin2 );
  Layout->addWidget( Combo1 );
  Layout->addWidget( Radio1 );
  Layout->addWidget( Check1 );
  Layout->addWidget( DateTime1 );
  
#if 0
  {
    QVBoxLayout *BoxLayout = new QVBoxLayout();
    BoxLayout->addWidget( new QRadioButton("aaa") );
    BoxLayout->addWidget( new QRadioButton("bbb") );
    BoxLayout->addWidget( new QRadioButton("ccc") );
    QGroupBox *Box = new QGroupBox();
    Box->setLayout( BoxLayout );

    QWidget *BoxWidget = new QWidget();
    QLayout *SLayout = new QVBoxLayout();
    SLayout->addWidget( Box );
    BoxWidget->setLayout( SLayout );

    Layout->addWidget( BoxWidget );
  }
#endif

  Layout->addWidget( Edit3 );
  Layout->addStretch();

  Widget.resize( 340, 600 );
  Widget.setLayout( Layout );
  Widget.show();

  return app.exec(); 
}


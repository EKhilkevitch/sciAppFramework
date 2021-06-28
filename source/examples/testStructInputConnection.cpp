
#include <QApplication>
#include <QString>
#include <QDebug>

#include "sciAppFramework/inputWidget.h"
#include "sciAppFramework/labelInputWidget.h"
#include "sciAppFramework/multiInputWidget.h"
#include "sciAppFramework/structInputConnection.h"

using namespace sciAppFramework;

struct structType
{
  int Int1;
  double Double;
  std::string StdString;
  QString QtString;
  int Int2;

  structType() :
    Int1( 3 ),
    Double( 2.8 ),
    StdString( "this is std string" ),
    QtString( "this is qt string" ),
    Int2( -4 ) {}
};

int main( int argc, char **argv )
{
  QApplication app(argc,argv);
  
  multiInputWidget Widget( "TestMIW" );
  structInputConnection StructInputConnection;

  STRUCT_INPUT_CONNECT_FIELD( &StructInputConnection, Widget.addInputWidget<labelEditWidget>( "Edit1", "Std string", "abcd" ), structType, StdString );
  STRUCT_INPUT_CONNECT_FIELD( &StructInputConnection, Widget.addInputWidget<labelEditWidget>( "Edit2", "Qt string", "xyzw" ), structType, QtString );
  STRUCT_INPUT_CONNECT_FIELD( &StructInputConnection, Widget.addInputWidget<labelSpinWidget>( "Spin1", "Int 1", 1 ), structType, Int1 );
  STRUCT_INPUT_CONNECT_FIELD( &StructInputConnection, Widget.addInputWidget<labelSpinWidget>( "Spin2", "Int 2", 2 ), structType, Int2 );
  STRUCT_INPUT_CONNECT_FIELD( &StructInputConnection, Widget.addInputWidget<labelDoubleSpinWidget>( "Spin3", "Double", 0.1 ), structType, Double );

  structType Struct;
  StructInputConnection.fillInputsFromStruct( &Struct );

  Widget.show();

  int Result = app.exec(); 

  StructInputConnection.fillStructFromInputs( &Struct );
  qDebug() << "Struct: " << Struct.Int1 << Struct.Double << QString::fromStdString(Struct.StdString) << Struct.QtString << Struct.Int2;
  return Result;
}



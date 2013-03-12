
#include <QtGui>

#include "sciAppFramework/catalogueWidget.h"

using namespace sciAppFramework;


struct testCatalogueListWidget : public catalogueListWidget
{
  testCatalogueListWidget() { initWidget(); }

  QList<QPushButton*> createButtonsList()
  {
    QPushButton *Load = new QPushButton("Load",this);
    QPushButton *Delete = new QPushButton("Delete",this);
    QList<QPushButton*> List;
    List << Load << Delete;
    return List;
  }
};

int main( int argc, char **argv )
{
  QApplication app(argc,argv);

  testCatalogueListWidget Widget;
  Widget.show();

  Widget.add( "Line1" );
  Widget.add( "Line2", false );
  Widget.add( "Line3", true );

  return app.exec(); 
}


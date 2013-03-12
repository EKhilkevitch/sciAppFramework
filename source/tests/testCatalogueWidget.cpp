
#include <QtGui>

#include "sciAppFramework/catalogueWidget.h"

using namespace sciAppFramework;


struct testCatalogueListWidget : public catalogueListWidget
{
  testCatalogueListWidget() { init(); }

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
  
  return app.exec(); 
}


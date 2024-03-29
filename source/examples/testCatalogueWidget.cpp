
#include "sciAppFramework/catalogueWidget.h"

#include <QPushButton>
#include <QApplication>

using namespace sciAppFramework;


struct testCatalogueListWidget : public catalogueListWidget
{
  testCatalogueListWidget() 
  { 
    initWidget(); 
  }
      
  QAbstractItemView::SelectionMode viewSelectionMode() const 
  { 
    return QAbstractItemView::MultiSelection; 
  }

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


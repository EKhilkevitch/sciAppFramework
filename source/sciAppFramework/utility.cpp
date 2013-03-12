
#include "sciAppFramework/utility.h"

#include <QBoxLayout>

using namespace sciAppFramework;

// ======================================================

void layoutOperations::appendStretch( QBoxLayout *Layout, int Stretch )
{
  if ( Layout != NULL )
    Layout->addStretch(Stretch);
}

// ------------------------------------------------------

bool layoutOperations::removeLastSpacing( QBoxLayout *Layout )
{
  if ( Layout == NULL )
    return false;
  if ( Layout->count() <= 0 )
    return false;

  QLayoutItem *LastLayoutItem = Layout->itemAt( Layout->count()-1 );
  if ( LastLayoutItem == NULL || LastLayoutItem->spacerItem() == NULL )
    return false;

  delete Layout->takeAt( Layout->count()-1 );
  return true;
}

// ------------------------------------------------------

void layoutOperations::moveContent( QBoxLayout *To, QBoxLayout *From )
{
  if ( To == NULL || From == NULL )
    return;

  while ( From->count() > 0 )
  {
    QLayoutItem *Item = From->takeAt(0);
    To->addItem( Item );
  }
}

// ------------------------------------------------------

void layoutOperations::replaceLayout( QWidget *Widget, QLayout *Layout )
{
  if ( Widget == NULL )
    return;

  delete Widget->layout();
  
  if ( Layout != NULL )
    Widget->setLayout( Layout );
}

// ------------------------------------------------------
      
void layoutOperations::appendLayoutItem( QWidget *Widget, QLayoutItem *Item, int Stretch )
{
  if ( Widget == NULL )
    return;

  QBoxLayout *Layout = dynamic_cast<QBoxLayout*>( Widget->layout() );
  appendLayoutItem( Layout, Item, Stretch );
}

// ------------------------------------------------------
      
void layoutOperations::appendLayoutItem( QBoxLayout *Layout, QLayoutItem *Item, int Stretch )
{
  if ( Layout == NULL || Item == NULL )
    return;
  
  removeLastSpacing( Layout );
  Layout->addItem( Item );
  appendStretch( Layout, Stretch );
}

// ======================================================


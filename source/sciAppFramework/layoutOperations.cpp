
#include "sciAppFramework/layoutOperations.h"

#include <QWidget>
#include <QBoxLayout>
#include <QLayoutItem>
#include <QDebug>

using namespace sciAppFramework;

// ======================================================

void layoutOperations::clearLayout( QLayout *Layout )
{
  if ( Layout == NULL )
    return;

  while ( true )
  {
    QLayoutItem *Item = Layout->takeAt(0);

    //qDebug() << "clearLayout: item = " << Item;

    if ( Item == NULL )
      break;
    
    //qDebug() << "clearLayout: this is " << " " << Item->layout() << Item->spacerItem() << Item->widget();
   
    QLayout *ItemLayout = Item->layout();
    QSpacerItem *ItemSpacer = Item->spacerItem();
    QWidget *ItemWidget = Item->widget();
    
    if ( ItemLayout != NULL )
    {
      clearLayout( ItemLayout );
      ItemLayout->deleteLater();
      delete Item;
    } else if ( ItemSpacer != NULL )
    {
      delete Item;
    } else if ( ItemWidget != NULL )
    {
      ItemWidget->deleteLater();
      delete Item;
    }

  }
}

// ------------------------------------------------------

void layoutOperations::appendStretch( QBoxLayout *Layout, int Stretch )
{
  if ( Layout != NULL )
    Layout->addStretch(Stretch);
}

// ------------------------------------------------------

bool layoutOperations::removeLastSpacing( QLayout *Layout )
{
  if ( Layout == NULL )
    return false;

  int Count = Layout->count();

  if ( Count <= 0 )
    return false;

  QLayoutItem *LastLayoutItem = Layout->itemAt( Count-1 );
  if ( LastLayoutItem == NULL || LastLayoutItem->spacerItem() == NULL )
    return false;

  delete Layout->takeAt( Count-1 );
  return true;
}

// ------------------------------------------------------

QWidget* layoutOperations::lastWidgetAfterSpacing( QLayout* Layout )
{
  if ( Layout == NULL )
    return NULL;

  int Count = Layout->count();

  if ( Count <= 0 )
    return NULL;

  QLayoutItem *LastLayoutItem = Layout->itemAt( Count-1 );
  if ( LastLayoutItem == NULL )
    return NULL;

  if ( LastLayoutItem->spacerItem() != NULL && Count > 1 )
    LastLayoutItem = Layout->itemAt( Count-2 );

  if ( LastLayoutItem == NULL )
    return NULL;

  return LastLayoutItem->widget();
}

// ------------------------------------------------------

void layoutOperations::moveContent( QLayout *To, QLayout *From )
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

// ------------------------------------------------------

// Need becouse appendLayoutItem(QBoxLayout*,QLayoutItem*) and QBoxLayout::additem() not work some times. Bug in Qt?
void layoutOperations::appendLayoutItem( QBoxLayout *Layout, QWidget *Widget, int Stretch )
{
  if ( Layout == NULL || Widget == NULL )
    return;
  
  removeLastSpacing( Layout );
  Layout->addWidget( Widget );
  appendStretch( Layout, Stretch );
}
    
// ------------------------------------------------------

void layoutOperations::insertLayoutItem( int Index, QWidget *Widget, QLayoutItem *Item, int Stretch )
{
  if ( Widget == NULL )
    return;

  QBoxLayout *Layout = dynamic_cast<QBoxLayout*>( Widget->layout() );
  if ( Layout == NULL )
    return;

  do
  {
    QWidget *ItemWidget = Item->widget();
    if ( ItemWidget != NULL )
    {
      Layout->insertWidget( Index, ItemWidget, Stretch );
      break;
    }

    QLayout *ItemLayout = Item->layout();
    if ( ItemLayout != NULL )
    {
      Layout->insertLayout( Index, ItemLayout, Stretch );
      break;
    }

    QSpacerItem *ItemSpace = Item->spacerItem();
    if ( ItemSpace != NULL )
    {
      Layout->insertSpacerItem( Index, ItemSpace );
      break;
    }
  } while ( false );
}

// ======================================================


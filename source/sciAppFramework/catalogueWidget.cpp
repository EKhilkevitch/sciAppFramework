
// ======================================================

#include "sciAppFramework/catalogueWidget.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QAbstractItemView>
#include <QListWidget>
#include <QKeyEvent>
#include <QDebug>

using namespace sciAppFramework;

// ======================================================

QWidget* catalogueWidget::createButtonsWidget()
{
  QList<QPushButton*> Buttons = createButtonsList();

  QBoxLayout *Layout = new QHBoxLayout();
  Layout->setContentsMargins( 0, 0, 0, 0 );
  
  foreach( QPushButton *Btn, Buttons )
    Layout->addWidget( Btn );

  QWidget *Widget = new QWidget(this);
  Widget->setLayout(Layout);
  return Widget;
}

// -----------------------------------------

void catalogueWidget::keyPressEvent( QKeyEvent *Event )
{
  Q_ASSERT( Event != NULL );

  if ( Event->key() == Qt::Key_Delete )
    emit deleteCurrentItem();

  QWidget::keyPressEvent( Event );
}

// -----------------------------------------

QList<QPushButton*> catalogueWidget::createButtonsList()
{
  return QList<QPushButton*>();
}

// -----------------------------------------

void catalogueWidget::initWidget()
{
  Q_ASSERT( ButtonsWidget == NULL );
  Q_ASSERT( ItemView == NULL );

  ButtonsWidget = createButtonsWidget();
  ItemView = createItemView();

  QBoxLayout *Layout = new QVBoxLayout();
  Layout->setContentsMargins( 0, 0, 0, 0 );

  if ( putButtonsonTheTop() )
  {
    Layout->addWidget( ButtonsWidget );
    Layout->addWidget( ItemView );
  } else {
    Layout->addWidget( ItemView );
    Layout->addWidget( ButtonsWidget );
  }
  setLayout( Layout );

  ItemView->setSelectionMode( viewSelectionMode() );
  enableSelectionSignalItemView();
}

// -----------------------------------------

void catalogueWidget::loadSettings( QSettings * )
{
}

// -----------------------------------------

void catalogueWidget::saveSettings( QSettings * ) const
{
}

// -----------------------------------------
      
void catalogueWidget::emitSelectionChanged() 
{ 
  emit selectionChanged(); 
}

// -----------------------------------------
      
catalogueWidget::catalogueWidget( QWidget *Parent, const QString &SettingsName ) :
  QWidget(Parent),
  settingsObject( dynamic_cast<settingsObject*>(Parent), SettingsName ),
  ButtonsWidget(NULL),
  ItemView(NULL) 
{
}

// -----------------------------------------

catalogueWidget::~catalogueWidget() 
{
}

// =========================================
      
catalogueListWidget::catalogueListWidget( QWidget *Parent, const QString &SettingsName ) : 
  catalogueItemViewTemplateWidget<QListWidget,QListWidgetItem>(Parent,SettingsName) 
{
}

// -----------------------------------------

catalogueListWidget::~catalogueListWidget()
{
}

// -----------------------------------------

QListWidgetItem* catalogueListWidget::add( const QString &Title, const QVariant &Data, bool Selected )
{
  if ( ! isItemViewInited() )
    return NULL;

  disableSelectionSignalItemView();

  QListWidget *ListWidget = itemViewCast();
  Q_ASSERT( ListWidget != NULL );

  QListWidgetItem *Item = new QListWidgetItem( ListWidget );
  Item->setText( Title );
  Item->setData( Qt::UserRole, Data );
  
  ListWidget->addItem( Item );
  Item->setSelected( Selected );
  Q_ASSERT( isSelected( ListWidget->count()-1 ) == Selected );

  enableSelectionSignalItemView();

  return Item;
}

// -----------------------------------------

void catalogueListWidget::clear()
{
  if ( ! isItemViewInited() )
    return;
  disableSelectionSignalItemView();
  itemViewCast()->clear();
  enableSelectionSignalItemView();
}

// -----------------------------------------
      
bool catalogueListWidget::isSelected( unsigned Index ) const
{
  if ( ! isItemViewInited() )
    return false;

  const QListWidget *ListWidget = itemViewCast();
  if ( (int)Index >= ListWidget->count() )
    return false;

  return isSelected( ListWidget->item(Index) );
}

// -----------------------------------------

bool catalogueListWidget::isSelected( const QVariant &Data ) const
{
  if ( ! isItemViewInited() )
    return false;

  const QListWidget *ListWidget = itemViewCast();
  for ( int i = 0; i < ListWidget->count(); i++ )
  {
    const QListWidgetItem *Item = ListWidget->item(i);
    if ( Item == NULL )
      continue;
    if ( Item->data( Qt::UserRole ) == Data )
      return isSelected( Item );
  }

  return false;
}

// -----------------------------------------

bool catalogueListWidget::isSelected( const QListWidgetItem* Item ) const
{
  if ( Item == NULL )
    return false;
  return Item->isSelected();
}

// -----------------------------------------

QListWidget* catalogueListWidget::createItemView()
{
  return new QListWidget(this);
}

// ======================================================

catalogueTableWidget::catalogueTableWidget( QWidget *Parent, const QString &SettingsName ) : 
  catalogueItemViewTemplateWidget<QTableWidget,QTableWidgetItem>(Parent,SettingsName) 
{
}

// ------------------------------------------------------

catalogueTableWidget::~catalogueTableWidget()
{
}

// ------------------------------------------------------

QTableWidget* catalogueTableWidget::createItemView()
{
  return new QTableWidget(this);
}

// ------------------------------------------------------
      
QTableWidgetItem* catalogueTableWidget::setItem( int Row, int Column, const QString& Title, const QVariant& Data )
{
  QTableWidgetItem *Item = new QTableWidgetItem( Title );
  Item->setText( Title );
  Item->setData( Qt::UserRole, Data );
  return setItem( Row, Column, Item );
}

// ------------------------------------------------------
      
QTableWidgetItem* catalogueTableWidget::setItem( int Row, int Column, QTableWidgetItem *Item )
{
  if ( Item == NULL )
    return NULL;
  itemViewCast()->setItem( Row, Column, Item );
  return Item;
}

// ======================================================



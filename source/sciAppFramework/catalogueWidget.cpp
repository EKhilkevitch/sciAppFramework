
// =========================================

#include "sciAppFramework/catalogueWidget.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QAbstractItemView>
#include <QListWidget>

using namespace sciAppFramework;

// =========================================

QWidget* catalogueWidget::createButtonsWidget()
{
  QList<QPushButton*> Buttons = createButtonsList();

  QBoxLayout *Layout = new QHBoxLayout();
  
  foreach( QPushButton *Btn, Buttons )
    Layout->addWidget( Btn );

  QWidget *Widget = new QWidget(this);
  Widget->setLayout(Layout);
  return Widget;
}

// -----------------------------------------

QList<QPushButton*> catalogueWidget::createButtonsList()
{
  return QList<QPushButton*>();
}

// -----------------------------------------

void catalogueWidget::init()
{
  Q_ASSERT( ButtonsWidget == NULL );
  Q_ASSERT( ItemView == NULL );

  ButtonsWidget = createButtonsWidget();
  ItemView = createItemView();

  QBoxLayout *Layout = new QVBoxLayout();
  Layout->addWidget( ButtonsWidget );
  Layout->addWidget( ItemView );
  setLayout( Layout );

  enableSelectionSignalItemView();
}

// -----------------------------------------

void catalogueWidget::loadSettings( QSettings * )
{
}

// -----------------------------------------

void catalogueWidget::saveSettings( QSettings * )
{
}

// -----------------------------------------
      
void catalogueWidget::emitSelectionChanged() 
{ 
  emit selectionChanged(); 
}

// -----------------------------------------
      
catalogueWidget::catalogueWidget( QWidget *Parent ) :
  QWidget(Parent),
  ButtonsWidget(NULL),
  ItemView(NULL) 
{
}

// =========================================
      
catalogueListWidget::catalogueListWidget( QWidget *Parent ) : 
  catalogueItemViewTemplateWidget<QListWidget>(Parent) 
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

QAbstractItemView* catalogueListWidget::createItemView()
{
  QListWidget *ListWidget = new QListWidget(this);
  ListWidget->setSelectionMode( QAbstractItemView::MultiSelection );
  return ListWidget;
}

// =========================================


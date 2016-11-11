
// ======================================================

#include "sciAppFramework/catalogueWidget.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QAbstractItemView>
#include <QListWidget>
#include <QKeyEvent>
#include <QSettings>
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
      
bool catalogueWidget::putButtonsOnTheTop() const 
{ 
  return true; 
}

// -----------------------------------------

QAbstractItemView::SelectionMode catalogueWidget::viewSelectionMode() const 
{ 
  return QAbstractItemView::SingleSelection; 
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

  if ( putButtonsOnTheTop() )
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

// -----------------------------------------

catalogueWidget::~catalogueWidget() 
{
}

// =========================================
      
catalogueListWidget::catalogueListWidget( QWidget *Parent ) : 
  catalogueItemViewTemplateWidget<QListWidget,QListWidgetItem>(Parent) 
{
}

// -----------------------------------------

catalogueListWidget::~catalogueListWidget()
{
}

// -----------------------------------------

QListWidgetItem* catalogueListWidget::add( const QString &Title ) 
{ 
  return add( Title, QVariant(), true ); 
}

// -----------------------------------------

QListWidgetItem* catalogueListWidget::add( const QString &Title, bool Selected ) 
{ 
  return add( Title, QVariant(), Selected ); 
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

QListWidgetItem* catalogueListWidget::item( int Row ) const 
{ 
  return itemViewCast()->item(Row); 
}
      
// -----------------------------------------

QListWidgetItem* catalogueListWidget::takeItem( int Row )
{ 
  return itemViewCast()->takeItem(Row); 
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

void catalogueListWidget::saveListInSettings( QSettings *Settings, const QString &Name ) const
{
  if ( Settings == NULL )
    return;

  Settings->beginGroup( Name );
  for ( int Row = 0; Row < count(); ++Row )
  {
    QListWidgetItem *Item = item( Row );
    if ( Item == NULL )
      continue;
    Settings->beginGroup( QString::number(Row) );
    Settings->setValue( "Text", Item->text() );
    Settings->setValue( "Data", Item->data(Qt::UserRole) );
    Settings->endGroup();
  }

  for ( int Row = count(); true; ++Row )
  {
    Settings->beginGroup( QString::number(Row) );
    if ( ! Settings->contains( "Text" ) )
    {
      Settings->endGroup(); 
      break;
    }
    Settings->remove( "" );
    Settings->endGroup(); 
  }

  Settings->endGroup();
}

// -----------------------------------------

void catalogueListWidget::loadListFromSettings( QSettings *Settings, const QString &Name )
{
  if ( Settings == NULL )
    return;

  clear();
  Settings->beginGroup( Name );
  for ( int Row = 0; true; ++Row )
  {
    Settings->beginGroup( QString::number(Row) );
    if ( ! Settings->contains( "Text" ) )
    {
      Settings->endGroup();
      break;
    }
    QString Text = Settings->value( "Text" ).toString();
    QVariant Data = Settings->value( "Data" );
    Settings->endGroup();
    add( Text, Data, false );
  } 
  Settings->endGroup();

}

// -----------------------------------------

QListWidget* catalogueListWidget::createItemView()
{
  return new QListWidget(this);
}

// -----------------------------------------
      
void catalogueListWidget::deleteSelectedItems()
{
  QList<QListWidgetItem*> Selected = selectedItems();
  foreach ( QListWidgetItem *Item, Selected )
    delete Item;
}

// ======================================================

catalogueTableWidget::catalogueTableWidget( QWidget *Parent ) : 
  catalogueItemViewTemplateWidget<QTableWidget,QTableWidgetItem>(Parent) 
{
}

// ------------------------------------------------------

catalogueTableWidget::~catalogueTableWidget()
{
}

// ------------------------------------------------------

void catalogueTableWidget::setColumnCount( int Count ) 
{ 
  itemViewCast()->setColumnCount( Count ); 
}

// ------------------------------------------------------

int catalogueTableWidget::columnCount() const 
{ 
  return itemViewCast()->columnCount(); 
}

// ------------------------------------------------------

void catalogueTableWidget::setColumnWidth( int Column, int Width ) 
{ 
  itemViewCast()->setColumnWidth( Column ,Width ); 
}

// ------------------------------------------------------

void catalogueTableWidget::setRowCount( int Count ) 
{ 
  itemViewCast()->setRowCount( Count );
}

// ------------------------------------------------------

int catalogueTableWidget::rowCount() const 
{ 
  return itemViewCast()->rowCount(); 
}

// ------------------------------------------------------

void catalogueTableWidget::setRowHeight( int Row, int Height ) 
{ 
  itemViewCast()->setRowHeight( Row, Height ); 
}

// ------------------------------------------------------

void catalogueTableWidget::enableSelectionSignalItemView()
{
  catalogueItemViewTemplateWidget<QTableWidget,QTableWidgetItem>::enableSelectionSignalItemView();

  connect( itemViewCast(), SIGNAL( currentItemChanged(QTableWidgetItem*, QTableWidgetItem*) ), SIGNAL(currentChanged(QTableWidgetItem*, QTableWidgetItem*)) );
  connect( itemViewCast(), SIGNAL(cellClicked(int,int)), SIGNAL(cellClicked(int,int)) );
  connect( itemViewCast(), SIGNAL(cellChanged(int,int)), SIGNAL(cellChanged(int,int)) );
  connect( itemViewCast(), SIGNAL(cellEntered(int,int)), SIGNAL(cellEntered(int,int)) );
}

// ------------------------------------------------------

QTableWidget* catalogueTableWidget::createItemView()
{
  return new QTableWidget(this);
}

// ------------------------------------------------------
      
QHeaderView* catalogueTableWidget::horizontalHeader() const 
{ 
  return itemViewCast()->horizontalHeader(); 
}

// ------------------------------------------------------

QHeaderView* catalogueTableWidget::verticalHeader() const 
{ 
  return itemViewCast()->verticalHeader(); 
}

// ------------------------------------------------------

void catalogueTableWidget::setHorizontalHeaderLabels( const QStringList &Labels )
{
  itemViewCast()->setHorizontalHeaderLabels( Labels );
}

// ------------------------------------------------------

void catalogueTableWidget::setHorizontalHeaderItem( int Column, QTableWidgetItem *Item )
{
  itemViewCast()->setHorizontalHeaderItem( Column, Item );
}

// ------------------------------------------------------

void catalogueTableWidget::setVerticalHeaderItem( int Row, QTableWidgetItem *Item )
{
  itemViewCast()->setVerticalHeaderItem( Row, Item );
}

// ------------------------------------------------------

void catalogueTableWidget::setVerticalHeaderLabels( const QStringList &Labels )
{
  itemViewCast()->setVerticalHeaderLabels( Labels );
}

// ------------------------------------------------------
      
void catalogueTableWidget::setEditTriggers( EditTriggers Triggers )
{
  itemViewCast()->setEditTriggers( Triggers );
}

// ------------------------------------------------------

void catalogueTableWidget::incrementRowCoumt() 
{ 
  int RowCount = rowCount();
  setRowCount( RowCount + 1 ); 
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

QTableWidgetItem* catalogueTableWidget::setItem( int Row, int Column, const QString& Title )
{
  return setItem( Row, Column, Title, QVariant() );
}

// ------------------------------------------------------
      
QTableWidgetItem* catalogueTableWidget::setItem( int Row, int Column, QTableWidgetItem *Item )
{
  if ( Item == NULL )
    return NULL;
  itemViewCast()->setItem( Row, Column, Item );
  return Item;
}

// -----------------------------------------

QTableWidgetItem* catalogueTableWidget::item( int Row, int Column ) const 
{ 
  return itemViewCast()->item( Row, Column ); 
}

// -----------------------------------------

QTableWidgetItem* catalogueTableWidget::takeItem( int Row, int Column ) 
{ 
  return itemViewCast()->takeItem( Row, Column ); 
}

// -----------------------------------------

void catalogueTableWidget::deleteSelectedItems()
{
  QList<QTableWidgetItem*> Selected = selectedItems();
  foreach ( QTableWidgetItem *Item, Selected )
    delete Item;

  for ( int i = 0, j = 0; true; i++, j++ )
  {
    while ( j < rowCount() && item( j, 0 ) == NULL )
      j++;

    if ( j >= rowCount() )
    {
      setRowCount( i );
      break;
    }

    if ( i != j )
    {
      QTableWidgetItem *Item = takeItem( j, 0 );
      setItem( i, 0, Item );
    }
  }
}

// ======================================================




// ======================================================

#include "sciAppFramework/comboMultiWidget.h"
#include "sciAppFramework/inputWidget.h"

#include <QVBoxLayout>
#include <QStackedWidget>
#include <QScrollArea>
#include <QDebug>

using namespace sciAppFramework;

// ======================================================

comboMultiWidget::comboMultiWidget( const QString &ComboLabel, QWidget *Parent, const QString &SettingsName ) :
  QWidget( Parent ),
  multiSettingsObject( dynamic_cast<multiSettingsObject*>(Parent), settingsObject::normolizeToSettingsName(SettingsName) ),
  ComboWidget( NULL ),
  StackedWidget( NULL )
{
  ComboWidget = inputWidget::create<labelComboWidget>( this, "Combo", ComboLabel, 0 );
  StackedWidget = new QStackedWidget( this );
  StackedWidget->layout()->setAlignment( Qt::AlignTop );
  connect( ComboWidget, SIGNAL(currentIndexChanged(int)), StackedWidget, SLOT(setCurrentIndex(int)) );

  QVBoxLayout *Layout = new QVBoxLayout();
  Layout->addWidget( ComboWidget );
  Layout->addSpacing( 5 );
  Layout->addWidget( StackedWidget );
  setLayout( Layout );
}

// ------------------------------------------------------
      
void comboMultiWidget::addWidget( const QString &Name, QWidget *Widget )
{
  Q_ASSERT( ComboWidget != NULL );
  Q_ASSERT( StackedWidget != NULL );
  Q_ASSERT( ComboWidget->count() == StackedWidget->count() );

  if ( Widget == NULL )
    return;

  ComboWidget->addItem( Name );
  StackedWidget->addWidget( Widget );

  settingsObject *SettingsObject = dynamic_cast<settingsObject*>(Widget);
  if ( SettingsObject != NULL )
    SettingsObject->setSettingsParent( this );

  if ( StackedWidget->currentIndex() < 0 )
  {
    ComboWidget->setCurrentIndex( 0 );
    StackedWidget->setCurrentIndex( 0 );
  }
}

// ------------------------------------------------------
      
void comboMultiWidget::setScrollArea( bool Scroll )
{
  Q_ASSERT( layout()->count() > 0 );
  QScrollArea *ScrollArea = dynamic_cast<QScrollArea*>( layout()->itemAt(layout()->count()-1)->widget() );

  if ( ( ScrollArea != NULL ) == Scroll )
    return;
    
  if ( Scroll )
  {
    Q_ASSERT( ScrollArea == NULL );
    
    layout()->removeWidget( StackedWidget );
    ScrollArea = new QScrollArea(this);
    ScrollArea->setWidget( StackedWidget );
    ScrollArea->setWidgetResizable( true );
    layout()->addWidget( ScrollArea );
  } else {
    layout()->removeWidget( ScrollArea );
    ScrollArea->takeWidget();
    delete ScrollArea;
    layout()->addWidget( StackedWidget );
  }
}

// ------------------------------------------------------

int comboMultiWidget::count() const
{
  Q_ASSERT( ComboWidget->count() == StackedWidget->count() );
  return ComboWidget->count();
}

// ------------------------------------------------------

QWidget* comboMultiWidget::currentWidget() const
{
  return StackedWidget->currentWidget();
}

// ------------------------------------------------------

int comboMultiWidget::currentIndex() const
{
  return ComboWidget->currentIndex();
}

// ------------------------------------------------------

QString comboMultiWidget::currentText() const
{
  return ComboWidget->currentText();
}

// ======================================================



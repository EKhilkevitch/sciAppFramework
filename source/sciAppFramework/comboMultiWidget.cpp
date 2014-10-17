
// ======================================================

#include "sciAppFramework/comboMultiWidget.h"
#include "sciAppFramework/inputWidget.h"

#include <QVBoxLayout>
#include <QStackedLayout>

using namespace sciAppFramework;

// ======================================================

comboMultiWidget::comboMultiWidget( const QString &ComboLabel, QWidget *Parent, const QString &SettingsName ) :
  QWidget( Parent ),
  multiSettingsObject( dynamic_cast<multiSettingsObject*>(Parent), settingsObject::normolizeToSettingsName(SettingsName) ),
  ComboWidget( NULL ),
  ComboLayout( NULL )
{
  ComboWidget = inputWidget::create<labelComboWidget>( this, "Combo", ComboLabel, 0 );
  recreateComboLayout();

  QVBoxLayout *Layout = new QVBoxLayout();
  Layout->addWidget( ComboWidget );
  Layout->addSpacing( 5 );
  Layout->addLayout( ComboLayout );
  setLayout( Layout );
}

// ------------------------------------------------------

void comboMultiWidget::recreateComboLayout()
{
  Q_ASSERT( ComboWidget != NULL );

  QStackedLayout *NewLayout = new QStackedLayout();
  connect( ComboWidget, SIGNAL(currentIndexChanged(int)), NewLayout, SLOT(setCurrentIndex(int)) );

  if ( ComboLayout != NULL )
  {
    while ( true )
    {
      QLayoutItem *Item = ComboLayout->takeAt(0);
      if ( Item == NULL )
        break;
      NewLayout->addItem( Item );
    }
  }

  delete ComboLayout;
  ComboLayout = NewLayout;
}

// ------------------------------------------------------
      
void comboMultiWidget::addWidget( const QString &Name, QWidget *Widget )
{
  Q_ASSERT( ComboWidget->count() == ComboLayout->count() );

  if ( Widget == NULL )
    return;

  ComboWidget->addItem( Name );
  ComboLayout->addWidget( Widget );

  settingsObject *SettingsObject = dynamic_cast<settingsObject*>(Widget);
  if ( SettingsObject != NULL )
    SettingsObject->setSettingsParent( this );

  if ( ComboLayout->currentIndex() < 0 )
  {
    ComboWidget->setCurrentIndex( 0 );
    ComboLayout->setCurrentIndex( 0 );
  }
}

// ------------------------------------------------------
      
void comboMultiWidget::setScrollArea( bool Scroll )
{
  Q_ASSERT( layout()->count() > 0 );

  QScrollArea *ScrollArea = NULL;
  ScrollArea = dynamic_cast<QScrollArea*>( layout()->itemAt(layout()->count()-1)->widget() );

  if ( ( ScrollArea != NULL ) == Scroll )
    return;
    
  recreateComboLayout();

  if ( Scroll )
  {
    Q_ASSERT( ScrollArea == NULL );

    QWidget *Widget = new QWidget(this);
    Widget->setLayout( ComboLayout );
    
    ScrollArea = new QScrollArea(this);
    ScrollArea->setWidget( Widget );

    layout()->addWidget( ScrollArea );
  } else {
    Q_ASSERT( ScrollArea != NULL );
    delete layout()->takeAt( layout()->count()-1 );

    QBoxLayout *BoxLayout = dynamic_cast<QBoxLayout*>( layout() );
    Q_ASSERT( BoxLayout != NULL );
    BoxLayout->addLayout( ComboLayout );
    
    delete ScrollArea->takeWidget();
    delete ScrollArea;
  }
}

// ------------------------------------------------------

int comboMultiWidget::count() const
{
  Q_ASSERT( ComboWidget->count() == ComboLayout->count() );
  return ComboWidget->count();
}

// ------------------------------------------------------

QWidget* comboMultiWidget::currentWidget() const
{
  return ComboLayout->currentWidget();
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



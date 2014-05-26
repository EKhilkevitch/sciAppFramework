
// ======================================================

#include "sciAppFramework/comboMultiWidget.h"
#include "sciAppFramework/inputWidget.h"

#include <QVBoxLayout>
#include <QStackedLayout>

using namespace sciAppFramework;

// ======================================================

comboMultiWidget::comboMultiWidget( const QString &ComboLabel, QWidget *Parent, const QString &SettingsName ) :
  QWidget( Parent ),
  multiSettingsObject( dynamic_cast<multiSettingsObject*>(Parent), settingsObject::normolizeToSettingsName(SettingsName) )
{
  ComboWidget = inputWidget::create<labelComboWidget>( this, "Combo", ComboLabel, 0 );
  ComboLayout = new QStackedLayout();

  connect( ComboWidget, SIGNAL(currentIndexChanged(int)), ComboLayout, SLOT(setCurrentIndex(int)) );

  QVBoxLayout *Layout = new QVBoxLayout();
  Layout->addWidget( ComboWidget );
  Layout->addSpacing( 5 );
  Layout->addLayout( ComboLayout );
  setLayout( Layout );
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



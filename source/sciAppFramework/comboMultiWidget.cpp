
// ======================================================

#include "sciAppFramework/comboMultiWidget.h"
#include "sciAppFramework/inputWidget.h"
#include "sciAppFramework/utility.h"

#include <QVBoxLayout>
#include <QStackedLayout>
#include <QDebug>

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
}

// ------------------------------------------------------

int comboMultiWidget::count() const
{
  Q_ASSERT( ComboWidget->count() == ComboLayout->count() );
  return ComboWidget->count();
}

// ------------------------------------------------------

QWidget* comboMultiWidget::currentWidget()
{
  return ComboLayout->currentWidget();
}

// ======================================================



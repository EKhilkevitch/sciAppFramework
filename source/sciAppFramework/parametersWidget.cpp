

// =========================================

#include "sciAppFramework/parametersWidget.h"
#include "sciAppFramework/multiInputWidget.h"

#include <QStackedLayout>
#include <QSettings>

using namespace sciAppFramework;

// =========================================

parametersWidget::parametersWidget( QWidget *Parent, multiInputWidget *MIW ) : 
  QGroupBox( "Parameters", Parent ),
  MultiInputWidget( MIW )
{
  Q_ASSERT( MultiInputWidget != NULL );

  connect( MultiInputWidget, SIGNAL(changed()), SIGNAL(changed()) );

  QStackedLayout *Layout = new QStackedLayout();
  Layout->addWidget( MultiInputWidget );
  setLayout(Layout);

  setMinimumWidth(10);
}

// -----------------------------------------

parametersWidget::~parametersWidget()
{
}

// -----------------------------------------
    
QString parametersWidget::settingsGroupName()
{
  return "Parameters";
}

// -----------------------------------------

void parametersWidget::loadSettings( QSettings *Settings )
{
  Q_ASSERT( Settings != NULL );
  Q_ASSERT( MultiInputWidget != NULL );
  Settings->beginGroup( settingsGroupName() );
  MultiInputWidget->loadSettings( Settings );
  Settings->endGroup();
}

// -----------------------------------------

void parametersWidget::saveSettings( QSettings *Settings )
{
  Q_ASSERT( Settings != NULL );
  Q_ASSERT( MultiInputWidget != NULL );
  Settings->beginGroup( settingsGroupName() );
  MultiInputWidget->saveSettings( Settings );
  Settings->endGroup();
}

// -----------------------------------------

QVariant parametersWidget::getVariantValue( const QString &Name ) const
{
  Q_ASSERT( MultiInputWidget != NULL );
  return MultiInputWidget->getVariantValue( Name );
}

// =========================================



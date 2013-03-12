
// =========================================

#include "sciAppFramework/settingsWidget.h"
#include "sciAppFramework/outputWidget.h"

#include <QStackedLayout>

using namespace sciAppFramework;

// =========================================

settingsWidget::settingsWidget( QWidget *Parent ) : QWidget(Parent)
{
}

// -----------------------------------------
      
settingsWidget::settingsWidget( outputWidget *OutputWidget, QWidget *Parent ) : QWidget(Parent)
{
  if ( OutputWidget != NULL )
  {
    addSettingsWidgets( OutputWidget->listOfSettingsWidgets() );
    connect( OutputWidget, SIGNAL(currentOutputChanged(int)), SLOT(setCurrentSettings(int)) );
  }
}

// -----------------------------------------

settingsWidget::~settingsWidget()
{
}

// -----------------------------------------

QStackedLayout* settingsWidget::getCurrentLayout()
{
  return dynamic_cast<QStackedLayout*>( layout() );
}

// -----------------------------------------

void settingsWidget::addSettingsWidget( QWidget *Widget )
{
  if ( layout() == NULL )
    setLayout( new QStackedLayout() );
  if ( Widget != NULL )
    getCurrentLayout()->addWidget( Widget );
}

// -----------------------------------------

void settingsWidget::addSettingsWidgets( const QList<QWidget*> &Widgets )
{
  foreach ( QWidget *W, Widgets )
    addSettingsWidget( W );
}

// -----------------------------------------

void settingsWidget::setCurrentSettings( int Index )
{
  getCurrentLayout()->setCurrentIndex( Index );
}

// =========================================



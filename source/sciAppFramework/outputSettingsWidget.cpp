
// =========================================

#include "sciAppFramework/outputSettingsWidget.h"
#include "sciAppFramework/outputWidget.h"

#include <QStackedLayout>

using namespace sciAppFramework;

// =========================================

outputSettingsWidget::outputSettingsWidget( QWidget *Parent ) : QWidget(Parent)
{
}

// -----------------------------------------
      
outputSettingsWidget::outputSettingsWidget( outputWidget *OutputWidget, QWidget *Parent ) : QWidget(Parent)
{
  if ( OutputWidget != NULL )
  {
    addSettingsWidgets( OutputWidget->listOfSettingsWidgets() );
    connect( OutputWidget, SIGNAL(currentOutputChanged(int)), SLOT(setCurrentSettings(int)) );
  }
}

// -----------------------------------------

outputSettingsWidget::~outputSettingsWidget()
{
}

// -----------------------------------------

QStackedLayout* outputSettingsWidget::getCurrentLayout()
{
  return dynamic_cast<QStackedLayout*>( layout() );
}

// -----------------------------------------

void outputSettingsWidget::addSettingsWidget( QWidget *Widget )
{
  if ( layout() == NULL )
    setLayout( new QStackedLayout() );
  if ( Widget != NULL )
    getCurrentLayout()->addWidget( Widget );
}

// -----------------------------------------

void outputSettingsWidget::addSettingsWidgets( const QList<QWidget*> &Widgets )
{
  foreach ( QWidget *W, Widgets )
    addSettingsWidget( W );
}

// -----------------------------------------

void outputSettingsWidget::setCurrentSettings( int Index )
{
  getCurrentLayout()->setCurrentIndex( Index );
}

// =========================================




// ======================================================

#include "sciAppFramework/inputWidget.h"

using namespace sciAppFramework;

// ======================================================

inputWidget::inputWidget( QWidget *Parent, const QString &SettingsName ) : 
  QWidget( Parent ),
  singleSettingsObject( Parent, settingsObject::normolizeToSettingsName(SettingsName) ) 
{
}

// ------------------------------------------------------
      
inputWidget::~inputWidget() 
{
}

// ------------------------------------------------------
      
void inputWidget::setEnabled( int Enabled ) 
{ 
  QWidget::setEnabled( Enabled != 0 ); 
}

// ------------------------------------------------------
      
QVariant inputWidget::valueToSettings() const 
{ 
  return getVariantValue(); 
}

// ------------------------------------------------------

void inputWidget::setValueFromSettings( const QVariant& Value ) 
{ 
  setVariantValue( Value ); 
}

// ======================================================



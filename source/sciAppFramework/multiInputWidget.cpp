
// ======================================================

#include "sciAppFramework/multiInputWidget.h"
#include "sciAppFramework/inputWidget.h"
#include "sciAppFramework/utility.h"

#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QDebug>

using namespace sciAppFramework;

// ======================================================

multiInputWidget::multiInputWidget( QWidget *Parent, const QString &SettingsName ) : 
  QWidget(Parent),
  multiSettingsObject( dynamic_cast<multiSettingsObject*>(Parent), settingsObject::normolizeToSettingsName(SettingsName) )
{
  QBoxLayout *Layout = createBoxLayout();
  setLayout( Layout );
}

// ------------------------------------------------------

QBoxLayout* multiInputWidget::createBoxLayout()
{
  return new QVBoxLayout();
}

// ------------------------------------------------------

inputWidget* multiInputWidget::addInputWidget( inputWidget *Input )
{
  if ( Input == NULL )
    return NULL;

  setupSettingsObject( Input );
  registerInputWidget( Input );
  addWidget( Input );
  
  return Input;
}

// ------------------------------------------------------

void multiInputWidget::setupSettingsObject( settingsObject *Object, const QString &Name )
{
  if ( Object == NULL )
    return;

  Object->setSettingsParent( this );

  if ( ! Name.isEmpty() )
    Object->setSettingsName( Name );
}

// ------------------------------------------------------

inputWidget* multiInputWidget::registerInputWidget( inputWidget *Input )
{
  if ( Input == NULL )
    return NULL;

  QString InputName = Input->settingsName();
  return registerInputWidget( InputName, Input );
}

// ------------------------------------------------------

inputWidget* multiInputWidget::registerInputWidget( const QString &InputName, inputWidget *Input )
{
  if ( Input == NULL )
    return NULL;

  if ( LabelInputMap.value( InputName, NULL ) == Input )
    return Input;

  delete LabelInputMap.value( InputName, NULL );
  LabelInputMap[ InputName ] = Input;
  connect( Input, SIGNAL(changed()), SIGNAL(changed()) );

  return Input;
}

// ------------------------------------------------------

void  multiInputWidget::addWidgetToLayout( QWidget *Widget )
{
  if ( Widget == NULL )
    return;
  
  Widget->setParent( this );
  QWidgetItem *WidgetItem = new QWidgetItem(Widget);
  layoutOperations::appendLayoutItem( this, WidgetItem, 1 );
}

// ------------------------------------------------------

QGroupBox* multiInputWidget::addMultiInputWidget( const QString &Name, const QString &Label, multiInputWidget *Widget )
{
  if ( Widget == NULL )
    return NULL;

  QBoxLayout *Layout = inputWidget::createLayoutWithoutMargins<QVBoxLayout>();
  Layout->addWidget( Widget );
  
  QGroupBox *Box = new QGroupBox( Label, this );
  Box->setLayout( Layout );
  addWidgetToLayout( Box );
  
  setupSettingsObject( Widget, Name );

  for ( labelInputMap::iterator i = Widget->LabelInputMap.begin(); i != Widget->LabelInputMap.end(); ++i )
    registerInputWidget( Name + ":" + i.key(), i.value() );

  return Box;
}

// ------------------------------------------------------

void multiInputWidget::addWidget( QWidget *Widget )
{
  if ( Widget == NULL )
    return;

  addWidgetToLayout( Widget );
  setupSettingsObject( dynamic_cast<settingsObject*>(Widget) );
}

// ------------------------------------------------------

void multiInputWidget::addSpacing( int Spacing )
{
  layoutOperations::appendLayoutItem( this, new QSpacerItem( Spacing, Spacing ) );
}

// ------------------------------------------------------

void multiInputWidget::addLabel( const QString &Label )
{
  QLabel *LabelWidget = new QLabel( Label );
  addWidgetToLayout( LabelWidget );
}

// ------------------------------------------------------

void  multiInputWidget::addLayout( QLayout *Layout )
{
  if ( Layout != NULL )
    layoutOperations::appendLayoutItem( this, Layout, 1 );
}

// ------------------------------------------------------

QVariant multiInputWidget::getVariantValue( const QString &Name ) const
{
  inputWidget *Input = LabelInputMap.value( Name, NULL );
  if ( Input == NULL )
    return QVariant();
  return Input->getVariantValue();
}

// ------------------------------------------------------

void multiInputWidget::setVariantValue( const QString &Name, const QVariant &Value )
{
  inputWidget *Input = LabelInputMap.value( Name, NULL );
  if ( Input != NULL )
    Input->setVariantValue( Value );
}

// ======================================================


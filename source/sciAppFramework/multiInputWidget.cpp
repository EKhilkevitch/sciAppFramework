
// ======================================================

#include "sciAppFramework/multiInputWidget.h"
#include "sciAppFramework/inputWidget.h"
#include "sciAppFramework/utility.h"

#include <QVBoxLayout>
#include <QSettings>
#include <QGroupBox>
#include <QLabel>
#include <QDebug>

using namespace sciAppFramework;

// ======================================================

multiInputWidget::multiInputWidget( QWidget *Parent ) : QWidget(Parent)
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

  registerInputWidget( Input );
  addWidget( Input );
  
  return Input;
}

// ------------------------------------------------------

inputWidget* multiInputWidget::registerInputWidget( inputWidget *Input )
{
  if ( Input == NULL )
    return NULL;

  QString InputName = Input->name();
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

QGroupBox* multiInputWidget::addMultiInputWidget( const QString &Name, const QString &Label, multiInputWidget *Widget )
{
  if ( Widget == NULL )
    return NULL;

  QBoxLayout *Layout = inputWidget::createLayoutWithoutMargins<QVBoxLayout>();
  Layout->addWidget( Widget );
  
  QGroupBox *Box = new QGroupBox( Label, this );
  Box->setLayout( Layout );
  addWidget( Box );

  for ( labelInputMap::iterator i = Widget->LabelInputMap.begin(); i != Widget->LabelInputMap.end(); ++i )
    registerInputWidget( Name + ":" + i.key(), i.value() );

  return Box;
}

// ------------------------------------------------------

void multiInputWidget::addSpacing( int Spacing )
{
  layoutOperations::appendLayoutItem( this, new QSpacerItem( Spacing, Spacing ) );
}

// ------------------------------------------------------

void  multiInputWidget::addWidget( QWidget *Widget )
{
  if ( Widget != NULL )
    layoutOperations::appendLayoutItem( this, new QWidgetItem(Widget), 1 );
}

// ------------------------------------------------------

void multiInputWidget::addLabel( const QString &Label )
{
  QLabel *LabelWidget = new QLabel( Label, this );
  addWidget( LabelWidget );
}

// ------------------------------------------------------

void  multiInputWidget::addLayout( QLayout *Layout )
{
  if ( Layout != NULL )
    layoutOperations::appendLayoutItem( this, Layout, 1 );
}

// ------------------------------------------------------

void multiInputWidget::saveSettings( QSettings *Settings )
{
  if ( Settings == NULL )
    return;

  QList< inputWidget* > ListOfLabelInputs = findChildren<inputWidget*>();
  foreach( inputWidget *Input, ListOfLabelInputs )
    Input->saveSettings( Settings );
}

// ------------------------------------------------------

void multiInputWidget::loadSettings( QSettings *Settings )
{
  if ( Settings == NULL )
    return;

  QList< inputWidget* > ListOfLabelInputs = findChildren<inputWidget*>();
  foreach( inputWidget *Input, ListOfLabelInputs )
    Input->loadSettings( Settings );
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


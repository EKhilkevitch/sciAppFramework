
// ======================================================

#include "sciAppFramework/multiInputWidget.h"
#include "sciAppFramework/inputWidget.h"
#include "sciAppFramework/layoutOperations.h"
#include "sciAppFramework/stackedSelectorWidget.h"

#include <QVBoxLayout>
#include <QGroupBox>
#include <QTabWidget>
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

void multiInputWidget::insertWidgetToLayout( int Index, QWidget *Widget )
{
  if ( Widget == NULL )
    return;
  
  Widget->setParent( this );
  QWidgetItem *WidgetItem = new QWidgetItem(Widget);
  layoutOperations::insertLayoutItem( Index, this, WidgetItem, 1 );
}

// ------------------------------------------------------

void multiInputWidget::addSubwidgetInputsToMap( const QString &Name, multiInputWidget *Widget )
{
  if ( Widget == NULL )
    return;
  
  if ( ! Name.isEmpty() )
    setupSettingsObject( Widget, Name );

  for ( labelInputMap::iterator i = Widget->LabelInputMap.begin(); i != Widget->LabelInputMap.end(); ++i )
  {
    const QString FullName = Name.isEmpty() ? i.key() : Name + ":" + i.key();
    inputWidget *Value = i.value();
    registerInputWidget( FullName, Value );
  }
}

// ------------------------------------------------------

QGroupBox* multiInputWidget::addBoxMultiInputWidget( const QString &Name, const QString &Label, multiInputWidget *Widget )
{
  if ( Widget == NULL )
    return NULL;

  QBoxLayout *Layout = inputWidget::createLayoutWithoutMargins<QVBoxLayout>();
  Layout->addWidget( Widget );
  addSubwidgetInputsToMap( Name, Widget );

  QGroupBox *Box = new QGroupBox( Label, this );
  Box->setLayout( Layout );
  addWidgetToLayout( Box );

  return Box;
}

// ------------------------------------------------------

QTabWidget* multiInputWidget::addTabMultiInputWidget( const QString &Name, const QString &Label, multiInputWidget *Widget )
{
  if ( Widget == NULL )
    return NULL;
  
  QBoxLayout *Layout = inputWidget::createLayoutWithoutMargins<QVBoxLayout>();
  Layout->addWidget( Widget );
  addSubwidgetInputsToMap( Name, Widget );

  QTabWidget *TabWidget = dynamic_cast<QTabWidget*>( layoutOperations::lastWidgetAfterSpacing(layout()) );
  if ( TabWidget == NULL )
  {
    TabWidget = new QTabWidget(this);
    addWidgetToLayout( TabWidget );
  }

  QWidget *TabPage = new QWidget( this );
  TabPage->setLayout( Layout );
  TabWidget->addTab( TabPage, Label );

  return TabWidget;
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

// ------------------------------------------------------

QMap<QString,QVariant> multiInputWidget::variantValues() const
{
  QMap<QString,QVariant> Map;

  foreach ( inputWidget *Input, LabelInputMap )
  {
    Q_ASSERT( Input != NULL );
    QString Name = Input->settingsName();
    QVariant Value = Input->getVariantValue();
    Map[ Name ] = Value;
  }

  return Map;
}

// ------------------------------------------------------

QMap<QString,QString> multiInputWidget::stringValues() const
{
  const QMap<QString,QVariant> &VariantMap = variantValues();
  QMap<QString,QString> StringMap;
  for ( QMap<QString,QVariant>::const_iterator i = VariantMap.begin(); i != VariantMap.end(); ++i )
    StringMap[ i.key() ] = i.value().toString();
  return StringMap;
}

// ------------------------------------------------------
      
std::map<std::string,std::string> multiInputWidget::stdStringValues() const
{
  const QMap<QString,QVariant> &VariantMap = variantValues();
  std::map<std::string,std::string> StdStringMap;
  for ( QMap<QString,QVariant>::const_iterator i = VariantMap.begin(); i != VariantMap.end(); ++i )
    StdStringMap[ i.key().toStdString() ] = i.value().toString().toStdString();
  return StdStringMap;
}

// ======================================================


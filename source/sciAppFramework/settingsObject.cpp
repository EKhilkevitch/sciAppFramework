
// =========================================

#include <QSettings>
#include <QList>
#include <QRegExp>
#include <QDebug>

#include "sciAppFramework/settingsObject.h"

using namespace sciAppFramework;


// =========================================

unsigned settingsObject::NumberOfCreatedSettingsObjects = 0;

// -----------------------------------------

settingsObject::settingsObject() :
  Parent( NULL )
{
  initSettingsObject();
}

// -----------------------------------------

settingsObject::settingsObject( settingsObject *Pnt, const QString &Name ) : 
  Parent(Pnt),
  SettingsName(Name)  
{
  initSettingsObject();
}

// -----------------------------------------

settingsObject::~settingsObject()
{
  while ( ! Children.isEmpty() )
    Children.front()->setSettingsParent( NULL );

  setSettingsParent( this );
}

// -----------------------------------------

void settingsObject::initSettingsObject()
{
  if ( Parent != NULL )
    Parent->Children.append(this);

  if ( SettingsName.isEmpty() )
    SettingsName = "SettingsObject_" + QString::number(NumberOfCreatedSettingsObjects);

  NumberOfCreatedSettingsObjects += 1;
}

// -----------------------------------------

void settingsObject::setSettingsParent( settingsObject *NewParent )
{
  if ( Parent != NULL )
    Parent->Children.removeAll( this );

  if ( NewParent != NULL )
    NewParent->Children.append( this );

  Parent = NewParent;
}

// -----------------------------------------
      
QString settingsObject::normolizeToSettingsName( QString String )
{
  return String.replace( QRegExp("[\\s:+*=\\-]"), "_" );
}

// =========================================


void singleSettingsObject::saveSettings( QSettings *Settings ) const
{
  if ( Settings == NULL )
    return;

  if ( settingsName().isEmpty() )
  {
    qDebug() << "singleSettingsObject::saveSettings: Warning: settingsName() is empty!";
    return;
  }

  Settings->setValue( settingsName(), valueToSettings() );
}

// -----------------------------------------

void singleSettingsObject::loadSettings( QSettings *Settings )
{
  if ( Settings == NULL )
    return;

  if ( settingsName().isEmpty() )
  {
    qDebug() << "singleSettingsObject::loadSettings: Warning: settingsName() is empty!";
    return;
  }
  
  QVariant Value = Settings->value( settingsName() );
  if ( Value.isValid() )
    setValueFromSettings( Value );
}

// =========================================

void multiSettingsObject::saveSettings( QSettings *Settings ) const
{
  if ( Settings == NULL )
    return;

  Settings->beginGroup( settingsName() );

  foreach ( const settingsObject* Object, settingsChildren() )
  {
    Q_ASSERT( Object->settingsParent() == this );
    Object->saveSettings(Settings);
  }

  Settings->endGroup();
}

// -----------------------------------------

void multiSettingsObject::loadSettings( QSettings *Settings )
{
  if ( Settings == NULL )
    return;

  Settings->beginGroup( settingsName() );

  foreach ( settingsObject* Object, settingsChildren() )
  {
    Q_ASSERT( Object->settingsParent() == this );
    Object->loadSettings(Settings);
  }

  Settings->endGroup();
}

// =========================================



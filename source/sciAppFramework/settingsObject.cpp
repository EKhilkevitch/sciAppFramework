
// =========================================

#include <QVariant>
#include <QList>
#include <QRegExp>
#include <QDebug>

#include "sciAppFramework/settingsObject.h"
#include "sciAppFramework/settingsStorage.h"

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

  setSettingsParent( NULL );
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
      
void settingsObject::setSettingsName( const QString &Name ) 
{ 
  SettingsName = Name; 
}

// -----------------------------------------

const QString& settingsObject::settingsName() const 
{ 
  return SettingsName; 
}

// -----------------------------------------
      
const QList<settingsObject*>& settingsObject::settingsChildren() const 
{ 
  return Children; 
}

// -----------------------------------------

const settingsObject* settingsObject::settingsParent() const 
{ 
  return Parent; 
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

// -----------------------------------------

#if 0
void settingsObject::copySettings( settingsObject *Destination ) const
{
  if ( Destination == NULL )
    return;

  QTemporaryFile SettingsFile;

  if ( ! SettingsFile.open() )
  {
    qWarning() << "settingsObject::copySettings() : can not open temporary file";
    return;
  }

  QSettings SettingsBuffer( SettingsFile.fileName(), QSettings::IniFormat );

  saveSettings( &SettingsBuffer );
  Destination->loadSettings( &SettingsBuffer );
}
#endif

// =========================================

singleSettingsObject::singleSettingsObject( settingsObject *Parent, const QString &Name ) : 
  settingsObject( Parent, Name ) 
{
}

// -----------------------------------------

void singleSettingsObject::saveSettings( settingsStorage *Settings ) const
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

void singleSettingsObject::loadSettings( settingsStorage *Settings )
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
      
multiSettingsObject::multiSettingsObject( settingsObject *Parent, const QString &Name ) :
  settingsObject( Parent, Name ) 
{
}

// -----------------------------------------
  
void multiSettingsObject::saveSettings( settingsStorage *Settings ) const
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

void multiSettingsObject::loadSettings( settingsStorage *Settings )
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



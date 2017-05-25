
// =========================================

#include <QSettings>
#include <QVariant>
#include <QString>
#include <QStringList>
#include <QDebug>

#include <QDomDocument>
#include <QDomElement>
#include <QDomText>

#include "sciAppFramework/settingsStorage.h"

using namespace sciAppFramework;

// =========================================

settingsStorage::~settingsStorage()
{
}

// =========================================

fileSettingsStorage::fileSettingsStorage( const QString &FileName ) :
  Settings( new QSettings( FileName, QSettings::IniFormat ) )
{
}

// -----------------------------------------

fileSettingsStorage::~fileSettingsStorage()
{
  delete Settings;
}

// -----------------------------------------
      
void fileSettingsStorage::setValue( const QString &Key, const QVariant &Value )
{
  Settings->setValue( Key, Value );
}

// -----------------------------------------
      
QVariant fileSettingsStorage::value(const QString &Key, const QVariant &Default ) const
{
  return Settings->value( Key, Default );
}

// -----------------------------------------

void fileSettingsStorage::remove( const QString &Key )
{
  Settings->remove( Key );
}

// -----------------------------------------
      
bool fileSettingsStorage::contains( const QString &Key ) const
{
  return Settings->contains( Key );
}

// -----------------------------------------
      
void fileSettingsStorage::beginGroup( const QString &Prefix )
{
  Settings->beginGroup( Prefix );
}

// -----------------------------------------
      
void fileSettingsStorage::endGroup()
{
  Settings->endGroup();
}

// -----------------------------------------

QStringList fileSettingsStorage::childGroups() const
{
  return Settings->childGroups();
}

// -----------------------------------------
      
QStringList fileSettingsStorage::childKeys() const
{
  return Settings->childKeys();
}

// =========================================

#if 0
xmlSettingsStorage::element::element()
{
}

// -----------------------------------------
#endif

xmlSettingsStorage::element::element( const QString &K, const QVariant &V ) :
  Key(K),
  Value(V)
{
}

// -----------------------------------------

xmlSettingsStorage::element::~element()
{
}

// -----------------------------------------

xmlSettingsStorage::group::group( group *P, const QString &K ) :
  Parent( P ),
  Key( K )
{
  if ( Parent == NULL )
    Parent = this;
}

// -----------------------------------------

xmlSettingsStorage::group::~group()
{
  while ( ! Groups.isEmpty() )
    delete Groups.last();

  Parent->Groups.removeAll(this);
}

// -----------------------------------------

const QString& xmlSettingsStorage::group::key() const
{
  return Key;
}

// -----------------------------------------

const QVariant& xmlSettingsStorage::group::value( const QString &Key, const QVariant &Default ) const
{
  foreach ( const element &E, Elements )
  {
    if ( E.key() == Key )
      return E.value();
  }
  return Default;
}

// -----------------------------------------

QStringList xmlSettingsStorage::group::childGroups() const
{
  QStringList Result;
  foreach ( const group *G, Groups )
    Result.append( G->key() );
  return Result;
}

// -----------------------------------------

QStringList xmlSettingsStorage::group::childKeys() const
{
  QStringList Result;
  foreach ( const element &E, Elements )
    Result.append( E.key() );
  return Result;
}

// -----------------------------------------

bool xmlSettingsStorage::group::contains( const QString &Key ) const
{
  foreach ( const element &E, Elements )
  {
    if ( E.key() == Key )
      return true;
  }

  foreach ( const group *G, Groups )
  {
    if ( G->key() == Key )
      return true;
  }

  return false;
}

// -----------------------------------------
          
void xmlSettingsStorage::group::remove( const QString &Key )
{
  if ( Key.isEmpty() )
  {
    Elements.clear();
    while ( ! Groups.isEmpty() )
      delete Groups.last();
    return;
  }

  QList<element>::iterator ElementsIterator = Elements.begin();
  while ( ElementsIterator != Elements.end() )
  {
    if ( ElementsIterator->key() == Key )
    { 
      ElementsIterator = Elements.erase(ElementsIterator);
    } else {
      ++ElementsIterator;
    }
  }

  QList<group*>::iterator GroupsIterator = Groups.begin();
  while ( GroupsIterator != Groups.end() )
  {
    group *Current = *GroupsIterator;
    if ( Current->key() == Key )
    {
      GroupsIterator = Groups.erase(GroupsIterator);
      delete Current;
    } else {
      ++GroupsIterator;
    }
  }

}

// -----------------------------------------
          
void xmlSettingsStorage::group::addElement( const QString &Key, const QVariant &Value )
{
  for ( int i = 0; i < Elements.size(); i++ )
  {
    element &E = Elements[i];
    if ( E.key() == Key )
    {
      E = element( Key, Value );
      return;
    }
  }

  Elements.append( element( Key, Value ) );
}

// -----------------------------------------

xmlSettingsStorage::group* xmlSettingsStorage::group::addGroup( const QString &Key )
{
  foreach( group *G, Groups )
  {
    if ( G->key() == Key )
      return G;
  }

  Groups.append( new group(this,Key) );
  return Groups.last();
}

// -----------------------------------------
          
const xmlSettingsStorage::group* xmlSettingsStorage::group::findGroup( const QString &Key ) const
{
  foreach( group *G, Groups )
  {
    if ( G->key() == Key )
      return G;
  }
  return NULL;
}

// -----------------------------------------

xmlSettingsStorage::group* xmlSettingsStorage::group::parent()
{
  return Parent;
}

// -----------------------------------------
      
xmlSettingsStorage::xmlSettingsStorage() :
  Root( NULL, QString() ),
  Current( &Root )
{
}

// -----------------------------------------

xmlSettingsStorage::~xmlSettingsStorage()
{
}

// -----------------------------------------

void xmlSettingsStorage::setValue( const QString &Key, const QVariant &Value )
{
  Current->addElement( Key, Value );
}

// -----------------------------------------

QVariant xmlSettingsStorage::value(const QString &Key, const QVariant &Default ) const
{
  return Current->value( Key, Default );
}

// -----------------------------------------

void xmlSettingsStorage::remove( const QString &Key )
{
  Current->remove( Key );
}

// -----------------------------------------

bool xmlSettingsStorage::contains( const QString &Key ) const
{
  return Current->contains( Key );
}

// -----------------------------------------

void xmlSettingsStorage::beginGroup( const QString &Prefix )
{
  Current = Current->addGroup( Prefix );
}

// -----------------------------------------

void xmlSettingsStorage::endGroup()
{
  Current = Current->parent();
}

// -----------------------------------------

QStringList xmlSettingsStorage::childGroups() const
{
  return Current->childGroups();
}

// -----------------------------------------

QStringList xmlSettingsStorage::childKeys() const
{
  return Current->childKeys();
}

// -----------------------------------------
      
void xmlSettingsStorage::storeGroupToElement( QDomDocument *Document, QDomElement *DomElement, const group *Group )
{
  const QVariant Empty;

  unsigned Index = 0;

  QStringList ChildKeys = Group->childKeys();
  foreach ( const QString &Key, ChildKeys )
  {
    const QVariant &Value = Group->value( Key, Empty );
    QDomElement Element = Document->createElement( "Element_" + QString::number(Index++) );
    Element.setAttribute( "key", Key );
    Element.setAttribute( "value", Value.toString() );
    DomElement->appendChild(Element);
  }

  QStringList ChildGroups = Group->childGroups();
  foreach ( const QString &Key, ChildGroups )
  {
    const group *SubGroup = Group->findGroup( Key );
    QDomElement Element = Document->createElement( "Group_" + QString::number(Index++) );
    Element.setAttribute( "key", SubGroup->key() );
    DomElement->appendChild( Element );
    storeGroupToElement( Document, &Element, SubGroup );
  }
}

// -----------------------------------------
      
void xmlSettingsStorage::loadGroupFromElement( const QDomNode &DomNode, group *Group )
{
  Group->remove( QString() );

  for ( QDomNode Child = DomNode.firstChild(); ! Child.isNull(); Child = Child.nextSibling() )
  {
    if ( ! Child.isElement() )
      continue;
    
    QDomElement Element = Child.toElement();
    if ( Element.isNull() )
      continue;

    QString TagName = Element.tagName();
    if ( TagName.startsWith( "Element_" ) )
    {
      QString Key = Element.attribute( "key" );
      QString Value = Element.attribute( "value" );
      Group->addElement( Key, Value );
    }

    if ( TagName.startsWith( "Group_" ) ) 
    {
      QString Key = Element.attribute( "key" );
      group *SubGroup = Group->addGroup( Key );
      loadGroupFromElement( Child, SubGroup );
    }
  }
}
      
// -----------------------------------------

QString xmlSettingsStorage::toXml() const
{
  QDomDocument Document;

  QDomElement RootElement = Document.createElement( "Root" );
  Document.appendChild( RootElement );

  storeGroupToElement( &Document, &RootElement, &Root );

  return Document.toString();
}

// -----------------------------------------

bool xmlSettingsStorage::fromXml( const QString &Xml )
{
  QString ErrorMessage;
  int ErrorLine;
  int ErrorColumn;

  QDomDocument Document;
  bool Ok = Document.setContent( Xml, &ErrorMessage, &ErrorLine, &ErrorColumn );
  if ( ! Ok )
  {
    qDebug() << "xmlSettingsStorage::fromXml: " << ErrorMessage << ", Line " << ErrorLine << ", Column " << ErrorColumn; 
    return false;
  }

  QDomElement RootElement = Document.documentElement();
  loadGroupFromElement( RootElement, &Root );

  return true;
}

// =========================================


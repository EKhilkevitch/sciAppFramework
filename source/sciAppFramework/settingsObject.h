
#pragma once

// =========================================

#include <QList>
#include <QString>
#include <QVariant>

class QSettings;

// =========================================

namespace sciAppFramework
{
  
  // =========================================
 
  class settingsObject 
  {
    private:
      QList<settingsObject*> Children;
      settingsObject* Parent;

      QString SettingsName;

      static unsigned NumberOfCreatedSettingsObjects;

    public:
      settingsObject( settingsObject *Parent, const QString &Name = QString() );
      virtual ~settingsObject();

      void setSettingsName( const QString &Name ) { SettingsName = Name; }
      const QString& settingsName() const { return SettingsName; }
      
      const QList<settingsObject*> settingsChildren() const { return Children; }
      const settingsObject* settingsParent() const { return Parent; }

      void setSettingsParent( settingsObject *Parent );
      
      virtual void saveSettings( QSettings *Settings ) const = 0;
      virtual void loadSettings( QSettings *Settings ) = 0;

      static QString normolizeToSettingsName( QString String );
  };

  // =========================================
  
  class singleSettingsObject : public settingsObject
  {
    protected:
      virtual QVariant valueToSettings() const = 0;
      virtual void setValueFromSettings( const QVariant& Value ) = 0;

    public:
      singleSettingsObject( settingsObject *Parent, const QString &Name = QString() ) : 
        settingsObject( Parent, Name ) {}

      void saveSettings( QSettings *Settings ) const;
      void loadSettings( QSettings *Settings );
  };
  
  // =========================================
  
  class multiSettingsObject : public settingsObject
  {
    public:
      multiSettingsObject( settingsObject *Parent, const QString &Name = QString() ) :
        settingsObject( Parent, Name ) {}
      
      void saveSettings( QSettings *Settings ) const;
      void loadSettings( QSettings *Settings );
  };
  
  // =========================================

}


#pragma once

// =========================================

#include <QList>
#include <QString>

class QVariant;

// =========================================

namespace sciAppFramework
{
  
  // =========================================
  
  class settingsStorage;
  
  // =========================================
 
  class settingsObject 
  {
    private:
      QList<settingsObject*> Children;
      settingsObject* Parent;

      QString SettingsName;

      static unsigned NumberOfCreatedSettingsObjects;

    private:
      void initSettingsObject();

    private:
      settingsObject( const settingsObject& );
      settingsObject& operator=( const settingsObject& );

    public:
      settingsObject();
      settingsObject( settingsObject *Parent, const QString &Name );
      template <class T> inline settingsObject( T *Parent, const QString &Name );
      virtual ~settingsObject();

      void setSettingsName( const QString &Name );
      const QString& settingsName() const;
      
      const QList<settingsObject*>& settingsChildren() const;
      const settingsObject* settingsParent() const;

      void setSettingsParent( settingsObject *Parent );
      
      virtual void saveSettings( settingsStorage *Settings ) const = 0;
      virtual void loadSettings( settingsStorage *Settings ) = 0;

      //void copySettings( settingsObject *Destination ) const;

      static QString normolizeToSettingsName( QString String );
  };

  // =========================================
  
  class singleSettingsObject : public settingsObject
  {
    protected:
      virtual QVariant valueToSettings() const = 0;
      virtual void setValueFromSettings( const QVariant& Value ) = 0;

    public:
      explicit singleSettingsObject( settingsObject *Parent, const QString &Name ); 
      template <class T> explicit inline singleSettingsObject( T *Parent, const QString &Name );

      void saveSettings( settingsStorage *Settings ) const;
      void loadSettings( settingsStorage *Settings );
  };
  
  // =========================================
  
  class multiSettingsObject : public settingsObject
  {
    public:
      explicit multiSettingsObject( settingsObject *Parent, const QString &Name );
      template <class T> explicit inline multiSettingsObject( T *Parent, const QString &Name );
      
      void saveSettings( settingsStorage *Settings ) const;
      void loadSettings( settingsStorage *Settings );
  };
  
  // =========================================
  
  template <class T> settingsObject::settingsObject( T *Pnt, const QString &Name ) :
    Parent( dynamic_cast< settingsObject* >( Pnt ) ),
    SettingsName( Name )
  {
    initSettingsObject();
  }
  
  // -----------------------------------------
 
  template <class T> singleSettingsObject::singleSettingsObject( T *Pnt, const QString &Name ) :
    settingsObject( Pnt, Name )
  {
  }

  // -----------------------------------------
  
  template <class T> multiSettingsObject::multiSettingsObject( T *Pnt, const QString &Name ) :
    settingsObject( Pnt, Name )
  {
  }
  
  // =========================================

}


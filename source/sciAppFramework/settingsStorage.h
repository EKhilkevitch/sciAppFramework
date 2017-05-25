
#pragma once

// =========================================

#include <QList>
#include <QString>
#include <QVariant>
#include <QList>

class QVariant;
class QString;
class QStringList;
class QSettings;
class QDomDocument;
class QDomElement;
class QDomNode;

// =========================================

namespace sciAppFramework
{
  
  // =========================================
  
  class settingsStorage
  {
    public:
      virtual ~settingsStorage();
      
      virtual void setValue( const QString &Key, const QVariant &Value ) = 0;
      virtual QVariant value(const QString &Key, const QVariant &Default = QVariant() ) const = 0;
      virtual void remove( const QString &Key ) = 0;
      virtual bool contains( const QString &Key ) const = 0;
      
      virtual void beginGroup( const QString &Prefix ) = 0;
      virtual void endGroup() = 0;

      virtual QStringList childGroups() const = 0;
      virtual QStringList childKeys() const = 0;
  };
  
  // =========================================

  class fileSettingsStorage : public settingsStorage
  {
    private:
      QSettings *Settings;

    private:
      fileSettingsStorage( const fileSettingsStorage& );
      fileSettingsStorage& operator=( const fileSettingsStorage& );

    public:
      fileSettingsStorage( const QString &FileName );
      ~fileSettingsStorage();

      void setValue( const QString &Key, const QVariant &Value );
      QVariant value(const QString &Key, const QVariant &Default = QVariant() ) const;
      void remove( const QString &Key );
      bool contains( const QString &Key ) const;
      
      void beginGroup( const QString &Prefix );
      void endGroup();

      QStringList childGroups() const;
      QStringList childKeys() const;
  };

  // =========================================
  
  class xmlSettingsStorage : public settingsStorage
  {
    private:
      class element
      {
        private:
          QString Key;
          QVariant Value;

        public:
          element( const QString &Key, const QVariant &Value );
          ~element();

          const QString& key() const { return Key; }
          const QVariant& value() const { return Value; }


      };

      class group
      {
        private:
          group *Parent;
          QString Key;
          QList< group* > Groups;
          QList< element > Elements;

        private:
          group( const group& );
          group& operator=( const group& );

        public:
          group( group *Parent, const QString &Key );
          ~group();

          const QString& key() const;
          const QVariant& value( const QString &Key, const QVariant &Default ) const;
          QStringList childGroups() const;
          QStringList childKeys() const;

          void addElement( const QString &Key, const QVariant &Value );
          group* addGroup( const QString &Key );
          const group* findGroup( const QString &Key ) const;
          group* parent();
          void remove( const QString &Key );
          bool contains( const QString &Key ) const;
      };

    private:
      group Root;
      group *Current;

    private:
      xmlSettingsStorage( const xmlSettingsStorage& );
      xmlSettingsStorage& operator=( const xmlSettingsStorage& );

      static void storeGroupToElement( QDomDocument *Document, QDomElement *DomElement, const group *Group );
      static void loadGroupFromElement( const QDomNode &DomNode, group *Group );

    public:
      xmlSettingsStorage();
      ~xmlSettingsStorage();

      void setValue( const QString &Key, const QVariant &Value );
      QVariant value(const QString &Key, const QVariant &Default = QVariant() ) const;
      void remove( const QString &Key );
      bool contains( const QString &Key ) const;
      
      void beginGroup( const QString &Prefix );
      void endGroup();

      QStringList childGroups() const;
      QStringList childKeys() const;

      QString toXml() const;
      bool fromXml( const QString &Xml );
  };
  
  // =========================================

}

// =========================================


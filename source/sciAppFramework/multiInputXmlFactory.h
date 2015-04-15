
#pragma once

// ======================================================

#include <QMap>
#include <QString>

class QStringList;
class QWidget;
class QDomElement;
class QDomNode;
class QDomNodeList;
class QDomDocument;

// ======================================================

namespace sciAppFramework
{

  // ======================================================
  
  class multiInputWidget;
  
  // ======================================================

  class multiInputWidgetXmlFactory
  {
    public:
      class modifierOfMultiInputWidget
      {
        protected:
          static QString attribute( const QDomElement &Element, const QString &AttrName, const QString &Default = QString() );
          static QStringList text( const QDomElement &Element );
          static QStringList text( const QDomNode &Node );
          static QStringList text( const QDomNodeList &Nodes );
          static QStringList text( const QDomElement &Element, const QString &NodeName );

        public:
          virtual QString tag() const = 0;
          virtual void addToMultiInputWidget( multiInputWidget *Widget, const QDomElement &Element ) const = 0;
          virtual ~modifierOfMultiInputWidget() {}
      };

      friend class modifierOfMultiInputWidget;

      typedef QMap< QString, modifierOfMultiInputWidget* > modifierOfMultiInputWidgetMap;
      
    private:
      QString ErrorString;
      QDomDocument *Doculemt;
      modifierOfMultiInputWidgetMap *Modifiers;

    private:
      multiInputWidgetXmlFactory( const multiInputWidgetXmlFactory& );
      multiInputWidgetXmlFactory& operator=( const multiInputWidgetXmlFactory& );

    private: 
      static QDomDocument* createDomDocument( const QString &Xml, QString *ErrorString = NULL );
      
    public:
      explicit multiInputWidgetXmlFactory( const QString &Xml );
      ~multiInputWidgetXmlFactory();

      bool isError() const { return ! ErrorString.isEmpty(); }
      const QString& errorMessage() const { return ErrorString; }


      multiInputWidget* create( QWidget *Parent = NULL ) const;
      void addItemsToMultiInputWidget( multiInputWidget* Widget ) const;

      static multiInputWidget* create( const QString &Xml, QWidget *Parent = NULL );
      static QString xmlRootName( const QString &Xml );
      
      static modifierOfMultiInputWidgetMap* createModifiersMap();
      static void deleteModifiersMap( modifierOfMultiInputWidgetMap *ModifiersMap );
      static void addModifierOfMultiInputWidget( modifierOfMultiInputWidget *Mod, modifierOfMultiInputWidgetMap *ModifiersMap );
      static void addNextItemToMultiInputWidget( multiInputWidget *Widget, const modifierOfMultiInputWidgetMap &ModifiersMap, const QDomElement &Element );
      static void setSettingsName( settingsObject *SettingsObject, const QDomElement &Element );
  };

  // ======================================================
  

}


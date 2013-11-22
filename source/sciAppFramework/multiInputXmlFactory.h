
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

    private:
      QDomDocument *Doculemt;
      QString ErrorString;

      QMap< QString, modifierOfMultiInputWidget* > Modifiers;

    private:
      multiInputWidgetXmlFactory( const multiInputWidgetXmlFactory& );
      multiInputWidgetXmlFactory& operator=( const multiInputWidgetXmlFactory& );

    private:
      static void setSettingsName( settingsObject *SettingsObject, const QDomElement &Element );
      void addNextItemToMultiInputWidget( multiInputWidget *Widget, const QDomElement &Element ) const;

      void initDocument( const QString &Xml );
      void initModifiers();

    public:
      multiInputWidgetXmlFactory( const QString &Xml );
      ~multiInputWidgetXmlFactory();

      bool isError() const { return ! ErrorString.isEmpty(); }
      const QString& errorMessage() const { return ErrorString; }

      void addModifierOfMultiInputWidget( modifierOfMultiInputWidget *Mod );

      multiInputWidget* create( QWidget *Parent = NULL ) const;
      void addItemsToMultiInputWidget( multiInputWidget* Widget ) const;

      static multiInputWidget* create( const QString &Xml, QWidget *Parent = NULL );
  };

  // ======================================================
  

}



#pragma once

// ======================================================

#include <QWidget>
#include <QVariant>

#include "sciAppFramework/settingsObject.h"

// ======================================================

namespace sciAppFramework
{

  // ======================================================

  class inputWidget : public QWidget, public singleSettingsObject
  {
    Q_OBJECT

    protected:
      QVariant valueToSettings() const;
      void setValueFromSettings( const QVariant& Value );

    public:
      explicit inputWidget( QWidget *Parent, const QString &SettingsName = QString() );
      virtual ~inputWidget();
   
      virtual const QString label() const = 0;
      
      virtual QVariant getVariantValue() const = 0;
      virtual void setVariantValue( const QVariant &Value ) = 0;

      template <class input> static input* create( QWidget *Parent, const QString &Name, const QString &Label, const QVariant &Value = QVariant() );
      template <class input> static input* create( const QString &Name, const QString &Label, const QVariant &Value = QVariant() );
      template <class layoutType> static layoutType* createLayoutWithoutMargins();

    signals:
      void changed();

    public slots:
      void setEnabled( int Enabled );
  };

  // ======================================================
  
  template <class layoutType> layoutType* inputWidget::createLayoutWithoutMargins()
  {
    layoutType *Layout = new layoutType();
    Layout->setContentsMargins( 0, 0, 0, 0 );
    return Layout;
  }
      
  template <class input> input* inputWidget::create( QWidget *Parent, const QString &Name, const QString &Label, const QVariant &Value )
  {
    input *Input = new input( Label, Parent );
    Input->setVariantValue( Value );
    Input->setSettingsName( Name );
    Input->setSettingsParent( dynamic_cast<multiSettingsObject*>(Parent) );
    return Input;
  }

  template <class input> input* inputWidget::create( const QString &Name, const QString &Label, const QVariant &Value )
  {
    return create<input>( NULL, Name, Label, Value );
  }
  
  // ======================================================

}

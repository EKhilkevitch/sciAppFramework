
#pragma once

// ======================================================

#include <QWidget>
#include <QMap>
#include <QVariant>

#include "sciAppFramework/inputWidget.h"
#include "sciAppFramework/settingsObject.h"

class QSettings;
class QLayout;
class QBoxLayout;
class QGroupBox;

// ======================================================

namespace sciAppFramework
{

  // ======================================================

  class multiInputWidget : public QWidget, public multiSettingsObject
  {
    Q_OBJECT

    private:
      typedef QMap<QString,inputWidget*>  labelInputMap;
      labelInputMap LabelInputMap;

    private:
      QBoxLayout* createBoxLayout();

    public:
      multiInputWidget( QWidget *Parent = NULL, const QString &SettingsName = QString() );

      void setupSettingsObject( settingsObject *Object, const QString &Name = QString() );
      inputWidget* registerInputWidget( inputWidget *Input ); 
      inputWidget* registerInputWidget( const QString &InputName, inputWidget *Input ); 

      inputWidget* addInputWidget( inputWidget *Input );
      template <class inp> inp* addInputWidget( const QString &Name, const QString &Label, const QVariant &Value = QVariant() );
      
      QGroupBox* addMultiInputWidget( const QString &Name, const QString &Label, multiInputWidget *Widget );

      void addSpacing( int Spacing );
      void addWidget( QWidget *Widget );
      void addLayout( QLayout *Layout );
      void addLabel( const QString &Label );

      QVariant getVariantValue( const QString &Name ) const;
      void setVariantValue( const QString &Name, const QVariant &Value );

      template <class T> T value( const QString &Name ) const;
      template <class T> T value( const QString &Name, const T& Default ) const;

      template <class inp> inp* input( const QString &Name );
      template <class inp> const inp* input( const QString &Name ) const;
   
    signals:
      void changed();
  };

  // ======================================================
  
  template <class inp> inp* multiInputWidget::addInputWidget( const QString &Name, const QString &Label, const QVariant &Value )
  {
    inp *Input = inputWidget::create<inp>( this, Name, Label, Value ); 
    addInputWidget( Input );
    return Input;
  }
  
  // ------------------------------------------------------
  
  template <class T> T multiInputWidget::value( const QString &Name ) const 
  { 
    return getVariantValue(Name).value<T>(); 
  }
  
  // ------------------------------------------------------
      
  template <class T> T multiInputWidget::value( const QString &Name, const T& Default ) const 
  {
    QVariant Value = getVariantValue(Name);
    if ( Value.isValid() && Value.canConvert<T>() )
      return Value.value<T>(); 
    return Default;
  }
  
  // ------------------------------------------------------
      
  template <class inp> inp* multiInputWidget::input( const QString &Name )
  {
    return dynamic_cast<inp*>( LabelInputMap.value(Name,NULL) );
  }
  
  // ------------------------------------------------------
  
  template <class inp> const inp* multiInputWidget::input( const QString &Name ) const
  {
    return dynamic_cast<const inp*>( LabelInputMap.value(Name,NULL) );
  }

  // ======================================================

}

// ======================================================



#pragma once

// ======================================================

#include <QWidget>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QScrollArea>

#include "sciAppFramework/labelInputWidget.h"
#include "sciAppFramework/adaptedInputWidget.h"
#include "sciAppFramework/settingsObject.h"

class QStackedWidget;

// ======================================================

namespace sciAppFramework
{
  
  // ======================================================

  template <class selectorWidget> class stackedSelectorWidget : public QWidget, public multiSettingsObject
  {
    private:
      selectorWidget *SelectorWidget;
      QStackedWidget *StackedWidget;

    public:
      explicit stackedSelectorWidget( const QString &Label, QWidget *Parent, const QString &SettingsName );
      virtual ~stackedSelectorWidget() {};
      
      stackedSelectorWidget& addWidget( const QString &Name, QWidget *Widget ); 
      stackedSelectorWidget& addWidget( const QString &Name, const QVariant &Value, QWidget *Widget ); 
      int count() const;
      
      stackedSelectorWidget& setScrollArea( bool Scroll );
      stackedSelectorWidget& setLabel( const QString &Label );

      selectorWidget* selector();

      QWidget* currentWidget() const;
      QVariant currentData() const;
      int currentIndex() const;
      QString currentText() const;
      
      void loadSettings( QSettings *Settings );
  };
  
  // ======================================================
  
  class comboMultiWidget : public stackedSelectorWidget<labelComboWidget>
  {
    public:
      comboMultiWidget( const QString &Label, QWidget *Parent = NULL, const QString &SettingsName = QString() );
  };
 
  // ------------------------------------------------------

  class radioMultiWidget : public stackedSelectorWidget<radioButtonWidget>
  {
    public:
      radioMultiWidget( const QString &Label, QWidget *Parent = NULL, const QString &SettingsName = QString() );
  };
  
  // ======================================================
  
  template <class selectorWidget> stackedSelectorWidget<selectorWidget>::stackedSelectorWidget( const QString &Label, QWidget *Parent, const QString &SettingsName ) :
    QWidget( Parent ),
    multiSettingsObject( Parent, settingsObject::normolizeToSettingsName(SettingsName) ),
    SelectorWidget( NULL ),
    StackedWidget( NULL )
  {
    SelectorWidget = inputWidget::create<selectorWidget>( this, "Selector", Label, 0 );
    StackedWidget = new QStackedWidget( this );
    StackedWidget->layout()->setAlignment( Qt::AlignTop );
    connect( SelectorWidget, SIGNAL(currentIndexChanged(int)), StackedWidget, SLOT(setCurrentIndex(int)) );

    QVBoxLayout *Layout = new QVBoxLayout();
    Layout->addWidget( SelectorWidget );
    Layout->addSpacing( 5 );
    Layout->addWidget( StackedWidget );
    setLayout( Layout );
  }

  // ------------------------------------------------------
        
  template <class selectorWidget> stackedSelectorWidget<selectorWidget>& stackedSelectorWidget<selectorWidget>::addWidget( const QString &Name, const QVariant &Value, QWidget *Widget )
  {
    Q_ASSERT( SelectorWidget != NULL );
    Q_ASSERT( StackedWidget != NULL );
    Q_ASSERT( SelectorWidget->count() == StackedWidget->count() );

    if ( Widget == NULL )
      return *this;

    SelectorWidget->addItem( Name, Value );
    StackedWidget->addWidget( Widget );

    settingsObject *SettingsObject = dynamic_cast<settingsObject*>(Widget);
    if ( SettingsObject != NULL )
      SettingsObject->setSettingsParent( this );

    if ( StackedWidget->currentIndex() < 0 )
    {
      SelectorWidget->setCurrentIndex( 0 );
      StackedWidget->setCurrentIndex( 0 );
    }

    return *this;
  }

  // ------------------------------------------------------
  
  template <class selectorWidget> stackedSelectorWidget<selectorWidget>& stackedSelectorWidget<selectorWidget>::addWidget( const QString &Name, QWidget *Widget )
  {
    int Count = SelectorWidget->count();
    return addWidget( Name, Count, Widget );
  }
  
  // ------------------------------------------------------
        
  template <class selectorWidget> stackedSelectorWidget<selectorWidget>& stackedSelectorWidget<selectorWidget>::setScrollArea( bool Scroll )
  {
    Q_ASSERT( layout()->count() > 0 );
    QScrollArea *ScrollArea = dynamic_cast<QScrollArea*>( layout()->itemAt(layout()->count()-1)->widget() );

    if ( ( ScrollArea != NULL ) == Scroll )
      return *this;
      
    if ( Scroll )
    {
      Q_ASSERT( ScrollArea == NULL );
      
      layout()->removeWidget( StackedWidget );
      ScrollArea = new QScrollArea(this);
      ScrollArea->setWidget( StackedWidget );
      ScrollArea->setWidgetResizable( true );
      layout()->addWidget( ScrollArea );
    } else {
      layout()->removeWidget( ScrollArea );
      ScrollArea->takeWidget();
      delete ScrollArea;
      layout()->addWidget( StackedWidget );
    }

    return *this;
  }

  // ------------------------------------------------------
  
  template <class selectorWidget> stackedSelectorWidget<selectorWidget>& stackedSelectorWidget<selectorWidget>::setLabel( const QString &Label )
  {
    SelectorWidget->setLabel( Label );
    return *this;
  }
  
  // ------------------------------------------------------
  
  template <class selectorWidget> selectorWidget* stackedSelectorWidget<selectorWidget>::selector()
  {
    return SelectorWidget;
  }

  // ------------------------------------------------------

  template <class selectorWidget> int stackedSelectorWidget<selectorWidget>::count() const
  {
    Q_ASSERT( SelectorWidget->count() == StackedWidget->count() );
    return SelectorWidget->count();
  }

  // ------------------------------------------------------

  template <class selectorWidget> QWidget* stackedSelectorWidget<selectorWidget>::currentWidget() const
  {
    return StackedWidget->currentWidget();
  }

  // ------------------------------------------------------
  
  template <class selectorWidget> QVariant stackedSelectorWidget<selectorWidget>::currentData() const
  {
    return SelectorWidget->currentData();
  }
  
  // ------------------------------------------------------

  template <class selectorWidget> int stackedSelectorWidget<selectorWidget>::currentIndex() const
  {
    return SelectorWidget->currentIndex();
  }

  // ------------------------------------------------------

  template <class selectorWidget> QString stackedSelectorWidget<selectorWidget>::currentText() const
  {
    return SelectorWidget->currentText();
  }
  
  // ------------------------------------------------------
      
  template <class selectorWidget> void stackedSelectorWidget<selectorWidget>::loadSettings( QSettings *Settings )
  {
    multiSettingsObject::loadSettings( Settings );
    StackedWidget->setCurrentIndex( SelectorWidget->currentIndex() );
  }

  // ======================================================

}


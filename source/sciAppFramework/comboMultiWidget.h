
#pragma once

// ======================================================

#include <QWidget>

#include "sciAppFramework/settingsObject.h"

class QStackedLayout;

// ======================================================

namespace sciAppFramework
{
  
  // ======================================================

  class labelComboWidget;

  // ======================================================

  class comboMultiWidget : public QWidget, public multiSettingsObject
  {
    Q_OBJECT

    private:
      labelComboWidget *ComboWidget;
      QStackedLayout *ComboLayout;
      
    private:
      void recreateComboLayout();

    public:
      explicit comboMultiWidget( const QString &ComboLabel, QWidget *Parent = NULL, const QString &SettingsName = QString() );

      void addWidget( const QString &Name, QWidget *Widget ); 
      int count() const;
      
      void setScrollArea( bool Scroll );

      QWidget* currentWidget() const;
      int currentIndex() const;
      QString currentText() const;
  };
  
  // ======================================================

}


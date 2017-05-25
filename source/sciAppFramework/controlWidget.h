
#pragma once

// =========================================

#include <QWidget>

// =========================================

namespace sciAppFramework
{
  
  // =========================================

  class multiInputWidget;
  class measurementParameters;
  class settingsStorage;

  // =========================================

  class controlWidget : public QWidget
  {
    Q_OBJECT

    private:
      multiInputWidget *ParametersWidget;

    private:
      QWidget* createParametersWidget();

    protected:
      virtual QWidget* createBtnWidget();
      virtual multiInputWidget* createMultiInputWidget();

      multiInputWidget& getParameters();
    
      void initWidget();

    public:
      explicit controlWidget( QWidget *Parent = NULL );
      virtual ~controlWidget() = 0;

      void saveSettings( settingsStorage *Settings );
      void loadSettings( settingsStorage *Settings );
      
      const multiInputWidget& getParameters() const;
      
    signals:
      void changed();
  };

  // =========================================
  

}


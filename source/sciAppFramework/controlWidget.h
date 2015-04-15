
#pragma once

// =========================================

#include <QWidget>

class QSettings;

// =========================================

namespace sciAppFramework
{
  
  // =========================================

  class multiInputWidget;
  class measurementParameters;

  // =========================================

  class controlWidget : public QWidget
  {
    Q_OBJECT

    private:
      multiInputWidget *ParametersWidget;

    private:
      virtual QWidget* createBtnWidget();
      virtual multiInputWidget* createMultiInputWidget();

      QWidget* createParametersWidget();

    protected:
      multiInputWidget& getParameters() { return *ParametersWidget; }
    
    protected:
      void initWidget();

    public:
      controlWidget( QWidget *Parent = NULL );
      virtual ~controlWidget() = 0;

      void saveSettings( QSettings* );
      void loadSettings( QSettings* );
      
      const multiInputWidget& getParameters() const { return *ParametersWidget; }
      
    signals:
      void changed();
  };

  // =========================================
  

}


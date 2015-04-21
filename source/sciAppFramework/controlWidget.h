
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
      QWidget* createParametersWidget();

    protected:
      virtual QWidget* createBtnWidget();
      virtual multiInputWidget* createMultiInputWidget();

      multiInputWidget& getParameters();
    
      void initWidget();

    public:
      explicit controlWidget( QWidget *Parent = NULL );
      virtual ~controlWidget() = 0;

      void saveSettings( QSettings* );
      void loadSettings( QSettings* );
      
      const multiInputWidget& getParameters() const { return *ParametersWidget; }
      
    signals:
      void changed();
  };

  // =========================================
  

}


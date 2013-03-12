
#pragma once

// =========================================

#include <QWidget>

class QSettings;
class QPushButton;
class QStackedLayout;

// =========================================

namespace sciAppFramework
{

  class multiInputWidget;
  class parametersWidget;
  class measurementParameters;

  // =========================================

  class controlWidget : public QWidget
  {
    Q_OBJECT

    private:
      parametersWidget *ParametersWidget;

    private:
      virtual QWidget* createBtnWidget();
      virtual multiInputWidget* createMultiInputWidget();

      QWidget* createParametersWidget();

    protected:
      parametersWidget& getParametersWidget() { return *ParametersWidget; }
    
    protected:
      void initWidget();

    public:
      controlWidget( QWidget *Parent = NULL );
      ~controlWidget();

      void saveSettings( QSettings* );
      void loadSettings( QSettings* );
      
      const measurementParameters* getMeasurementParameters() const;
      
    signals:
      void changed();

  };

  // =========================================
  
  class measureControlWidget : public controlWidget
  {
    Q_OBJECT

    private:
      QStackedLayout *StartStopLayout;
      QStackedLayout *PauseContLayout;

    private slots:
      void doStop();
      void doStart();
      void doPause();
      void doContinue();
      
      QWidget* createBtnWidget();
      
    protected:
      static void setEnableLayout( QLayout *Layout, bool Enable );
    
    public:
      measureControlWidget( QWidget *Parent = NULL );

      virtual void prepareToStart();
      virtual void prepareToStop();
      virtual void prepareToPause();
      virtual void prepareToContinue();

    signals:
      void start();
      void stop();
      void pause();
      void cont();
  };
  
  // =========================================

}


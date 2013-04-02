
#pragma once

// =========================================

#include <QWidget>

#include "sciAppFramework/measurementThread.h"

class QSettings;
class QPushButton;
class QStackedLayout;

// =========================================

namespace sciAppFramework
{

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
      multiInputWidget& getParametersWidget() { return *ParametersWidget; }
    
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
  
  class measureControlWidget : public controlWidget, public measurementParameters
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
      virtual ~measureControlWidget() = 0;

      virtual void prepareToStart();
      virtual void prepareToStop();
      virtual void prepareToPause();
      virtual void prepareToContinue();
      
      QVariant getVariantValue( const QString &Name ) const;

    signals:
      void start();
      void stop();
      void pause();
      void cont();
  };
  
  // =========================================

}


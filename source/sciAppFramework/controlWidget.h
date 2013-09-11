
#pragma once

// =========================================

#include <QWidget>

#include "sciAppFramework/measurementThread.h"

class QSettings;
class QPushButton;
class QLayout;
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
      void doSave();
     
    private:
      QWidget* createBtnWidget();

      virtual bool enableStartStopButtons() const { return true; }
      virtual bool enablePauseContinueButtons() const { return true; }

      virtual QString startButtonText() const { return "Start"; }
      virtual QString stopButtonText() const { return "Stop"; }
      virtual QString pauseButtonText() const { return "Puase"; }
      virtual QString continueButtonText() const { return "Continue"; }
      virtual QStringList saveButtonsTextAndNames() const;

      void makeStartStopButtonsLayout();
      void makePauseContinueButtonsLayout();
      QWidget* createControlButtonsWidget();
      QWidget* createSaveButtonsWidget();

      static QStackedLayout* createStackedLayout( QWidget *FirstWidget, QWidget *SecondWidget );

      static QString buttonsTextFromString( const QString &NameAndText );
      static QString buttonsNameFromString( const QString &NameAndText );
      
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

      void save( QString );
  };
  
  // =========================================

}


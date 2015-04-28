
#pragma once

// =========================================

#include "sciAppFramework/controlWidget.h"
#include "sciAppFramework/measurementParameters.h"

class QLayout;
class QStackedLayout;

// =========================================

namespace sciAppFramework
{
  
  // =========================================

  class measurementParameters;

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
     
    protected:
      QWidget* createBtnWidget();

      virtual bool enableStartStopButtons() const { return true; }
      virtual bool enablePauseContinueButtons() const { return true; }

      virtual QString startButtonText() const { return "Start"; }
      virtual QString stopButtonText() const { return "Stop"; }
      virtual QString pauseButtonText() const { return "Puase"; }
      virtual QString continueButtonText() const { return "Continue"; }
      virtual QStringList saveButtonsTextAndNames() const;

      virtual int buttonsWidgetHeight() const { return 60; }

    private:
      void makeStartStopButtonsLayout();
      void makePauseContinueButtonsLayout();
      QWidget* createControlButtonsWidget();
      QWidget* createSaveButtonsWidget();

      static QStackedLayout* createStackedLayout( QWidget *FirstWidget, QWidget *SecondWidget );

      static QString buttonsTextFromString( const QString &NameAndText );
      static QString buttonsNameFromString( const QString &NameAndText );
      
    protected:
      static void setEnableLayout( QLayout *Layout, bool Enable );
      
    protected:
      QVariant getVariantValue( const QString &Name ) const;
    
    public:
      explicit measureControlWidget( QWidget *Parent = NULL );
      virtual ~measureControlWidget() = 0;

      virtual void prepareToStart();
      virtual void prepareToStop();
      virtual void prepareToPause();
      virtual void prepareToContinue();
      

    signals:
      void start();
      void stop();
      void pause();
      void cont();

      void save( QString );
  };
  
  // =========================================

}


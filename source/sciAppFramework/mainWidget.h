

#pragma once

// =========================================

#include <QMainWindow>

class QSettings;
class QTimer;
 
// =========================================

namespace sciAppFramework
{

  class inputWidget;
  class outputWidget;
  class controlWidget;
  class settingsWidget;
  class measurementThread;
  class measurementParameters;

  // =========================================

  class mainWidget : public QMainWindow
  {
    Q_OBJECT

    private:
      outputWidget *OutputWidget;
      settingsWidget *SettingsWidget;
      controlWidget *ControlWidget;

      QString CurrentDir;

    private:
      static QSettings* createSettingsObject();
      QString getSaveFileName();

    protected:
      virtual void setupOutputWidget();
      virtual void setupSettingsWidget();
      virtual void setupControlWidget();
      virtual void setupThisWindow();

    private:
      virtual controlWidget* createControlWidget() = 0;
      virtual outputWidget* createOutputWidget() = 0;
      virtual settingsWidget* createSettingsWidget();
      virtual QString appTitle() const;
      virtual QIcon appIcon() const;

    protected:
      void initWidget();
      virtual void doInitWidget();
      
      template <class output> output* mainOutputWidget() { return dynamic_cast< output* >( OutputWidget ); }
      template <class control> control* mainControlWidget() { return dynamic_cast< control* >( ControlWidget ); }

      virtual void doSaveSettings( QSettings *Settings );
      virtual void doLoadSettings( QSettings *Settings );

    public:
      mainWidget();
      virtual ~mainWidget();

      void saveSettings();
      void loadSettings();
  };

  // =========================================
  
  class measureMainWidget : public mainWidget
  {
    Q_OBJECT

    private:
      measurementThread *MeasurementThread;
      QTimer *MeasurementTimer;

    protected:
      virtual void setupControlWidget();
      virtual void setupMeasurementThread();
      virtual void setupMeasurementTimer();
      virtual void processMeasurementError();
      
    private:
      virtual measurementThread* createMeasurementThread( const measurementParameters &Parameters ) = 0;
      virtual void outputMeasurementData() { }
      

    protected:
      virtual void doInitWidget();
      
      template <class thr> thr* mainMeasurementThread() { return dynamic_cast< thr* >( MeasurementThread ); }

    public:
      measureMainWidget();
      ~measureMainWidget();

    public slots:
      void showMeasurementData();
      void checkMeasurementState();

      void startMeasurement();
      void stopMeasurement();
      void pauseMeasurement();
      void contMeasurement();
  };
  
  // =========================================

}


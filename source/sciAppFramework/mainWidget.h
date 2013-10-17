

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
  class outputSettingsWidget;
  class measurementThread;
  class measurementParameters;

  // =========================================

  class mainWidget : public QMainWindow
  {
    Q_OBJECT

    private:
      outputWidget *OutputWidget;
      outputSettingsWidget *OutputSettingsWidget;
      controlWidget *ControlWidget;

      QString CurrentDir;

    private:
      static QSettings* createSettingsObject();

    protected:
      virtual void setupOutputWidget();
      virtual void setupOutputSettingsWidget();
      virtual void setupControlWidget();
      virtual void setupThisWindow();

    private:
      virtual controlWidget* createControlWidget() { return NULL; }
      virtual outputWidget* createOutputWidget() { return NULL; }
      virtual outputSettingsWidget* createOutputSettingsWidget();
      virtual QString appTitle() const;
      virtual QIcon appIcon() const;

    protected:
      void initWidget();
      virtual void doInitWidget();
      
      template <class output> output* mainOutputWidget() { return dynamic_cast< output* >( OutputWidget ); }
      template <class control> control* mainControlWidget() { return dynamic_cast< control* >( ControlWidget ); }

      virtual void doSaveSettings( QSettings *Settings );
      virtual void doLoadSettings( QSettings *Settings );
      
      QString getSaveFileName( const QString &Filter, const QString &DefaultSuffix );

      virtual void showErrorMessage( const QString &Message );

    public:
      mainWidget();
      virtual ~mainWidget() = 0;

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
      virtual measurementThread* createMeasurementThread( const measurementParameters &Parameters ) { return NULL; }
      virtual void outputMeasurementData() { }
      virtual void saveMeasurementData( const QString &DataName ) { }
      virtual void saveMeasurementScreen();

    protected:
      virtual void doInitWidget();
      
      template <class thr> thr* mainMeasurementThread() { return dynamic_cast< thr* >( MeasurementThread ); }
      
      QString getSaveDataFileName();
      QString getSaveImageFileName();

    public:
      measureMainWidget();
      ~measureMainWidget() = 0;

    public slots:
      void showMeasurementData();
      void checkMeasurementState();

      void startMeasurement();
      void stopMeasurement();
      void pauseMeasurement();
      void contMeasurement();

      void saveMeasurement( QString  );
  };
  
  // =========================================

}


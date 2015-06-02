
#pragma once

// =========================================

#include "sciAppFramework/mainWidget.h"

class QTimer;
 
// =========================================

namespace sciAppFramework
{

  class measurementThread;
  class measurementParameters;

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
      
      virtual measurementThread* createMeasurementThread( const measurementParameters *Parameters ) { return NULL; }
      virtual void outputMeasurementData() { }
      virtual void saveMeasurementData( const QString &DataName ) { }
      virtual void saveMeasurementScreen();
      
      virtual void doInitWidget();

      void startMeasurementTimer();

    protected:
      template <class thr> thr* mainMeasurementThread() { return dynamic_cast< thr* >( MeasurementThread ); }
      
      QString getSaveDataFileName();
      QString getSaveImageFileName();

    public:
      measureMainWidget();
      ~measureMainWidget() = 0;

    public slots:
      void showMeasurementData();
      void checkMeasurementState();

      virtual void startMeasurement();
      virtual void stopMeasurement();
      virtual void pauseMeasurement();
      virtual void contMeasurement();

      void saveMeasurement( QString );
  };
  
  // =========================================

}


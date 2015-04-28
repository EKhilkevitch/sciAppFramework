
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

    private:
      virtual void setupControlWidget();
      virtual void setupMeasurementThread();
      virtual void setupMeasurementTimer();
      virtual void processMeasurementError();
      
      virtual measurementThread* createMeasurementThread( const measurementParameters *Parameters ) { return NULL; }
      virtual void outputMeasurementData() { }
      virtual void saveMeasurementData( const QString &DataName ) { }
      virtual void saveMeasurementScreen();
      
      virtual void doInitWidget();

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

      void startMeasurement();
      void stopMeasurement();
      void pauseMeasurement();
      void contMeasurement();

      void saveMeasurement( QString  );
  };
  
  // =========================================

}




#pragma once

// =========================================

#include <QThread>
#include <QMutex>
#include <QVariant>

// =========================================

namespace sciAppFramework
{

  // =========================================

  class measurementParameters 
  {
    public:
      virtual QVariant getVariantValue( const QString &Name ) const = 0;
      virtual ~measurementParameters() {}
  };

  // =========================================

  class measurementThread : public QThread
  {
    Q_OBJECT

    private:
      const measurementParameters &Parameters;

      mutable QMutex ProcessMutex;
      mutable QMutex DataMutex;
      
      volatile bool NeedToStop;
      volatile bool NeedToPause;
      mutable volatile bool ExistNewData;

      volatile bool ErrorOccurs;
      QString ErrorString;

      volatile unsigned CountOfMeasurements;

    private:
      void resetFlags();
      void resetError();
      void setErrorNoLock( const QString &String );
      void setError( const QString &String );
      void waitForPause();
      void doMeasurement();
      void prepareForMeasurement();
      void cleanAfterMeasurement();
      bool needToStopMeasurement();
      bool needToPauseMeasurement();
      
      void run();
       
    protected: 
      virtual void prepare();
      virtual void measure();
      virtual void clean();
      
      void waitForTime( double WaitTime );

      void lockDataMutex() const;
      void unlockDataMutex() const;

      void setExistNewData() { ExistNewData = true; }
      void clearExistNewData() const { ExistNewData = false; }

      void incrementCountOfMeasurements();

      virtual double timeForWaitingBetweenMeasurements() const;

      void waitWhileRunning( unsigned MaxWaitTimeMs = 1000 );
      void stopAndCleanOnDestruction();

      template <class T> T parameter( const QString &Name ) const;       
      template <class T> T parameter( const QString &Name, const T &Default ) const; 

      template <class T> T returnLockedValue( const T &Value ) const;
      template <class T> void copyLockedValue( T *To, const T &From ) const;
      
      virtual int maxCountOfMeasurements() const;

    public:
      explicit measurementThread( const measurementParameters &Parameters );
      virtual ~measurementThread() = 0;
      
      bool isExistNewData() const { return ExistNewData; }

      unsigned countOfMeasurements() const;

      bool isError() const { return ErrorOccurs; }
      QString errorString() const;

      static QString waitTimeSecondsName() { return "WaitTimeSeconds"; }
      static QString maxCountOfMeasurementsName() { return "MaxCountOfMeasurements"; }

    public slots:
      void start();
      void stop()  { NeedToStop = true;   }
      void pause() { NeedToPause = true;  }
      void cont()  { NeedToPause = false; }
  };

  // =========================================
      
  template <class T> T measurementThread::parameter( const QString &Name ) const 
  { 
    return Parameters.getVariantValue(Name).value<T>(); 
  }

  // -----------------------------------------
      
  template <class T> T measurementThread::parameter( const QString &Name, const T &Default ) const
  { 
    QVariant Value = Parameters.getVariantValue(Name);
    return ( Value.isValid() && Value.canConvert<T>() ) ? Value.value<T>() : Default; 
  }
  
  // -----------------------------------------
      
  template <class T> T measurementThread::returnLockedValue( const T &Value ) const
  {
    lockDataMutex();
    T CopyOfValue( Value );
    unlockDataMutex();
    return CopyOfValue;
  }
  
  // -----------------------------------------

  template <class T> void measurementThread::copyLockedValue( T *To, const T &From ) const
  {
    if ( To == NULL )
      return;

    lockDataMutex();
    *To = From;
    unlockDataMutex();
  }
  
  // =========================================

}


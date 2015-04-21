

// =========================================

#include "sciAppFramework/measurementThread.h"
#include <stdexcept>

#include <QTime>

using namespace sciAppFramework;

// =========================================

measurementThread::measurementThread( const measurementParameters &P ) :
  Parameters(P)
{
  resetFlags();
  resetError();
}

// -----------------------------------------

measurementThread::~measurementThread()
{
}

// -----------------------------------------

void measurementThread::start()
{
  resetFlags();
  resetError();

  QThread::start();
}

// -----------------------------------------

void measurementThread::run()
{
  prepareForMeasurement();

  while ( ! needToStopMeasurement() )
  {
    incrementCountOfMeasurements();
    doMeasurement();
    waitForTime( timeForWaitingBetweenMeasurements() );
    if ( needToPauseMeasurement() )
      waitForPause();
  }

  cleanAfterMeasurement();
}

// -----------------------------------------

void measurementThread::resetFlags()
{
  NeedToStop = false;
  NeedToPause = false;
  ExistNewData = false;
  CountOfMeasurements = 0;
}

// -----------------------------------------

void measurementThread::resetError()
{
  ProcessMutex.lock();
  ErrorString = QString();
  ErrorOccurs = false;
  ProcessMutex.unlock();
}

// -----------------------------------------

void measurementThread::setError( const QString &String ) 
{
  ProcessMutex.lock();
  setErrorNoLock( String );
  ProcessMutex.unlock();
}

// -----------------------------------------

void measurementThread::setErrorNoLock( const QString &String )
{
  ErrorString = String;
  ErrorOccurs = true;
}

// -----------------------------------------

QString measurementThread::errorString() const
{
  ProcessMutex.lock();
  QString Result = ErrorString;
  ProcessMutex.unlock();
  return Result;
}

// -----------------------------------------
    
bool measurementThread::needToStopMeasurement()
{
  if ( NeedToStop )
    return true;
  if ( ErrorOccurs )
    return true;

  int MaxCountOfMeasurements = maxCountOfMeasurements();
  if ( MaxCountOfMeasurements > 0 && (int)countOfMeasurements() >= MaxCountOfMeasurements )
    return true;

  return false;
}

// -----------------------------------------

bool measurementThread::needToPauseMeasurement()
{
  if ( needToStopMeasurement() )
    return false;
  if ( NeedToPause )
    return true;
  return false;
}

// -----------------------------------------

void measurementThread::waitForTime( double WaitTime )
{
  const unsigned OneSleepTime = 2;

  QTime Time;
  Time.start();
  while ( Time.elapsed() * 1e-3 < WaitTime && !needToStopMeasurement() )
    msleep( OneSleepTime );
}

// -----------------------------------------

void measurementThread::waitForPause()
{
  const unsigned OneSleepTime = 2;
 
  while ( needToPauseMeasurement() )
    msleep( OneSleepTime );
}

// -----------------------------------------

void measurementThread::waitWhileRunning( unsigned MaxWaitTimeMs )
{
  const unsigned OneSleepTime = 20;

  QTime Time;
  Time.start();
  while ( Time.elapsed() * 1e-3 < MaxWaitTimeMs && isRunning() )
    msleep( OneSleepTime );
}
      
// -----------------------------------------

void measurementThread::stopAndCleanOnDestruction()
{
  stop();
  waitWhileRunning();
  clean();
}
      
// -----------------------------------------

void measurementThread::incrementCountOfMeasurements() 
{ 
  CountOfMeasurements += 1; 
}

// -----------------------------------------
      
unsigned measurementThread::countOfMeasurements() const
{
  return CountOfMeasurements;
}

// -----------------------------------------
      
double measurementThread::timeForWaitingBetweenMeasurements() const
{
  return parameter<double>( waitTimeSecondsName() );
}

// -----------------------------------------

int measurementThread::maxCountOfMeasurements() const 
{ 
  return parameter<int>( maxCountOfMeasurementsName() ); 
}

// -----------------------------------------

void measurementThread::measure()
{
}
    
// -----------------------------------------

void measurementThread::prepare()
{
}

// -----------------------------------------

void measurementThread::clean()
{
}

// -----------------------------------------

void measurementThread::doMeasurement()
{
  ProcessMutex.lock();
  try
  {
    measure();
  } catch ( std::exception &Exception )
  {
    setErrorNoLock( Exception.what() );
  }
  ProcessMutex.unlock();
}

// -----------------------------------------

void measurementThread::prepareForMeasurement()
{
  ProcessMutex.lock();
  try
  {
    prepare();
  } catch ( std::exception &Exception )
  {
    setErrorNoLock( Exception.what() );
  }
  ProcessMutex.unlock();
}

// -----------------------------------------

void measurementThread::cleanAfterMeasurement()
{
  ProcessMutex.lock();
  try
  {
    clean();
  } catch ( std::exception &Exception )
  {
    setErrorNoLock( Exception.what() );
  }
  ProcessMutex.unlock();
}

// =========================================



// =========================================

#include "sciAppFramework/measurementThread.h"
#include <exception>

#include <QTime>

using namespace sciAppFramework;

// =========================================

measurementThread::measurementThread( const measurementParameters *P ) :
  Parameters(P)
{
  Q_ASSERT( Parameters != NULL );

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

void measurementThread::startWithPreviousPoint()
{
  unsigned CurrentCount = CountOfMeasurements;
  resetFlags();
  resetError();
  CountOfMeasurements = ( CurrentCount == 0 ) ? 0 : CurrentCount - 1;

  QThread::start();
}

// -----------------------------------------

void measurementThread::run()
{
  prepareForMeasurement();

  while ( ! needToStopMeasurement() )
  {
    doMeasurement();
    waitForTime( timeForWaitingBetweenMeasurements() );
    if ( needToPauseMeasurement() )
      waitForPause();
    incrementCountOfMeasurements();
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
  StateMutex.lock();
  ErrorString = QString();
  ErrorOccurs = false;
  StateMutex.unlock();
}

// -----------------------------------------

void measurementThread::setError( const QString &String ) 
{
  StateMutex.lock();
  setErrorNoLock( String );
  StateMutex.unlock();
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
  StateMutex.lock();
  QString Result = ErrorString;
  StateMutex.unlock();
  return Result;
}

// -----------------------------------------
    
bool measurementThread::needToStopMeasurement()
{
  if ( NeedToStop )
    return true;

  if ( ErrorOccurs )
    return true;

  if ( countOfMeasurements() == 0 )
    return false;

  if ( isLastMeasurementCompleate() )
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

bool measurementThread::isLastMeasurementCompleate() const
{
  if ( countOfMeasurements() == 0 )
    return true;

  int MaxCountOfMeasurements = maxCountOfMeasurements();
  if ( MaxCountOfMeasurements > 0 && static_cast<int>(countOfMeasurements()) >= MaxCountOfMeasurements )
    return true;
  return false;
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
    bool NeedToContinue = false;
    try
    {
      NeedToContinue = processMeasurementException( Exception );
    } catch ( ... ) {}

    if ( ! NeedToContinue )
      setError( Exception.what() );
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
    setError( Exception.what() );
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
    setError( Exception.what() );
  }
  ProcessMutex.unlock();
}

// -----------------------------------------
      
void measurementThread::lockDataMutex() const
{
  DataMutex.lock();
}

// -----------------------------------------

void measurementThread::unlockDataMutex() const
{
  DataMutex.unlock();
}
      
// -----------------------------------------

QString measurementThread::waitTimeSecondsName() 
{ 
  return "WaitTimeSeconds"; 
}

// -----------------------------------------

QString measurementThread::maxCountOfMeasurementsName() 
{ 
  return "MaxCountOfMeasurements"; 
}

// -----------------------------------------
      
bool measurementThread::processMeasurementException( const std::exception & )
{
  return false;
}

// =========================================


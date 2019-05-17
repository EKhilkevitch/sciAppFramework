
// =========================================

#include "sciAppFramework/measureMainWidget.h"
#include "sciAppFramework/measurementThread.h"
#include "sciAppFramework/measureControlWidget.h"

#include <QTimer>
#include <QMessageBox>
#include <QPixmap>
#include <QDebug>

using namespace sciAppFramework;

// =========================================
    
measureMainWidget::measureMainWidget() : 
  MeasurementThread(NULL),
  MeasurementTimer(NULL)
{
}

// -----------------------------------------

measureMainWidget::~measureMainWidget()
{
  delete MeasurementThread;
  delete MeasurementTimer;
}

// -----------------------------------------

measurementThread* measureMainWidget::createMeasurementThread( const measurementParameters * ) 
{ 
  return NULL; 
}

// -----------------------------------------

void measureMainWidget::outputMeasurementData() 
{
}

// -----------------------------------------

void measureMainWidget::saveMeasurementData( const QString& ) 
{
}

// -----------------------------------------

void measureMainWidget::doInitWidget()
{
  mainWidget::doInitWidget();
  setupMeasurementThread();
  setupMeasurementTimer();
  
  Q_ASSERT( MeasurementThread != NULL );
}

// -----------------------------------------

void measureMainWidget::setupControlWidget()
{
  mainWidget::setupControlWidget();

  measureControlWidget *ControlWidget = mainControlWidget<measureControlWidget>();
  if ( ControlWidget == NULL )
    return;
  
  connect( ControlWidget, SIGNAL(start()), SLOT(startMeasurement()) );
  connect( ControlWidget, SIGNAL(stop()),  SLOT(stopMeasurement()) );
  connect( ControlWidget, SIGNAL(pause()), SLOT(pauseMeasurement()) );
  connect( ControlWidget, SIGNAL(cont()),  SLOT(contMeasurement()) );
  connect( ControlWidget, SIGNAL(save(QString)), SLOT(saveMeasurement(QString)) );
}

// -----------------------------------------

void measureMainWidget::setupMeasurementThread()
{
  const measureControlWidget *ControlWidget = mainControlWidget<measureControlWidget>();
  if ( ControlWidget == NULL )
    return;
  MeasurementThread = createMeasurementThread( ControlWidget );
}

// -----------------------------------------

void measureMainWidget::setupMeasurementTimer()
{
  MeasurementTimer = new QTimer(this);
  connect( MeasurementTimer, SIGNAL(timeout()), SLOT(checkMeasurementState()) );
}

// -----------------------------------------

void measureMainWidget::showMeasurementData()
{
  outputMeasurementData();
}
    
// -----------------------------------------

void measureMainWidget::checkMeasurementState()
{
  if ( MeasurementThread == NULL )
    return;

  if ( MeasurementThread->isError() || !MeasurementThread->isRunning() )
    stopMeasurement();
  else if ( MeasurementThread->isExistNewData() )
    showMeasurementData();
}

// -----------------------------------------

void measureMainWidget::processMeasurementError()
{
  QMessageBox::warning( this, "Error", "Error while mreasurement:\n" + 
    ( MeasurementThread == NULL ? QString("NO THREAD!") : MeasurementThread->errorString() ) );
}

// -----------------------------------------

void measureMainWidget::startMeasurement()
{
  if ( MeasurementThread == NULL )
    return;

  startMeasurementTimer();
  MeasurementThread->start();   
}

// -----------------------------------------

void measureMainWidget::stopMeasurement()
{
  if ( MeasurementThread == NULL )
    return;

  MeasurementTimer->stop();
  MeasurementThread->stop();
  showMeasurementData();

  measureControlWidget *ControlWidget = mainControlWidget<measureControlWidget>();
  if ( ControlWidget != NULL )
    ControlWidget->prepareToStop();
  
  if ( MeasurementThread->isError() )
    processMeasurementError();
}

// -----------------------------------------

void measureMainWidget::pauseMeasurement()
{
  if ( MeasurementThread != NULL )
    MeasurementThread->pause();
}

// -----------------------------------------

void measureMainWidget::contMeasurement()
{
  if ( MeasurementThread != NULL )
    MeasurementThread->cont();
}
      
// -----------------------------------------

void measureMainWidget::saveMeasurement( QString Name )
{
  if ( Name == "Screen" )
    saveMeasurementScreen();
  else
    saveMeasurementData(Name);
}

// -----------------------------------------

void measureMainWidget::saveMeasurementScreen()
{
  const QString &FileName = getSaveImageFileName();
  if ( FileName.isEmpty() )
    return;

  QPixmap Pixmap( width(), height() );
  render( &Pixmap );
  if ( ! Pixmap.save( FileName ) )
     showErrorMessage( "Error while saving screen image.\nCheck your system and try again." );
}

// -----------------------------------------

QString measureMainWidget::getSaveDataFileName()
{
  return getSaveFileName( "Dat files (*.dat);; All files (*)", "dat" );
}

// -----------------------------------------

QString measureMainWidget::getSaveImageFileName()
{
  return getSaveFileName( "Image files (*.png);; All files (*)", "png" );
}

// -----------------------------------------
      
void measureMainWidget::startMeasurementTimer()
{
  Q_ASSERT( MeasurementTimer != NULL );
  MeasurementTimer->start( 30 );
}

// =========================================



// =========================================

#include "sciAppFramework/mainWidget.h"
#include "sciAppFramework/outputWidget.h"
#include "sciAppFramework/outputSettingsWidget.h"
#include "sciAppFramework/controlWidget.h"
#include "sciAppFramework/measurementThread.h"
#include "sciAppFramework/parametersWidget.h"

#include <QTimer>
#include <QSettings>
#include <QDockWidget>
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QIcon>
#include <QDebug>

using namespace sciAppFramework;

// =========================================
    
mainWidget::mainWidget() : 
  OutputWidget(NULL),
  OutputSettingsWidget(NULL),
  ControlWidget(NULL)
{
}

// -----------------------------------------

mainWidget::~mainWidget()
{
  saveSettings();
}

// -----------------------------------------

void mainWidget::initWidget()
{
  setupThisWindow();
 
  doInitWidget();

  Q_ASSERT( OutputWidget != NULL );
  Q_ASSERT( OutputSettingsWidget != NULL );
  Q_ASSERT( ControlWidget != NULL );
  
  loadSettings();
}

// -----------------------------------------

void mainWidget::doInitWidget()
{
  setupOutputWidget();
  setupOutputSettingsWidget();
  setupControlWidget();
}

// -----------------------------------------

outputSettingsWidget* mainWidget::createOutputSettingsWidget()
{
  return new outputSettingsWidget( this );
}

// -----------------------------------------

void mainWidget::setupOutputWidget()
{
  OutputWidget = createOutputWidget();
  setCentralWidget( OutputWidget );
}

// -----------------------------------------

void mainWidget::setupOutputSettingsWidget()
{
  Q_ASSERT( OutputWidget != NULL );

  OutputSettingsWidget = createOutputSettingsWidget();
  OutputSettingsWidget->addSettingsWidgets( OutputWidget->listOfSettingsWidgets() );
  connect( OutputWidget, SIGNAL(currentOutputChanged(int)), OutputSettingsWidget, SLOT(setCurrentSettings(int)) );

  QDockWidget *DockWidget = new QDockWidget( "Settings", this );
  DockWidget->setAllowedAreas( Qt::BottomDockWidgetArea );
  DockWidget->setWidget( OutputSettingsWidget );
  DockWidget->setFeatures( QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable );
  addDockWidget( Qt::BottomDockWidgetArea, DockWidget );  
}

// -----------------------------------------

void mainWidget::setupControlWidget()
{
  ControlWidget = createControlWidget();

  QDockWidget *DockWidget = new QDockWidget( "Control", this );
  DockWidget->setAllowedAreas( Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea );
  DockWidget->setWidget( ControlWidget );
  DockWidget->setFeatures( QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable );
  addDockWidget( Qt::LeftDockWidgetArea, DockWidget );
}

// -----------------------------------------

void mainWidget::setupThisWindow()
{
  setWindowTitle( appTitle() );
  QApplication::setWindowIcon( appIcon() );
}

// -----------------------------------------

QString mainWidget::appTitle() const
{
  return QFileInfo( QCoreApplication::applicationName() ).baseName();
}

// -----------------------------------------

QIcon mainWidget::appIcon() const
{
  return QApplication::windowIcon();
}

// -----------------------------------------

QSettings* mainWidget::createSettingsObject()
{
  QString AppName = QFileInfo( QCoreApplication::applicationFilePath() ).baseName();
  QString FileName = QCoreApplication::applicationDirPath() + "/" + AppName + ".conf";
  return new QSettings( FileName, QSettings::IniFormat );  
}

// -----------------------------------------

void mainWidget::saveSettings()
{
  QSettings *Settings = createSettingsObject();
  doSaveSettings( Settings );
  delete Settings;
}

// -----------------------------------------

void mainWidget::doSaveSettings( QSettings *Settings )
{
  Q_ASSERT( Settings != NULL );
  
  Settings->setValue( "Size", size() );
  Settings->setValue( "Pos", pos() );   
  Settings->setValue( "CurrentDir", CurrentDir ); 

  OutputWidget->saveSettings( Settings );
  OutputSettingsWidget->saveSettings( Settings );
  ControlWidget->saveSettings( Settings );
}

// -----------------------------------------

void mainWidget::loadSettings()
{
  QSettings *Settings = createSettingsObject();
  doLoadSettings( Settings );
  delete Settings;
}

// -----------------------------------------

void mainWidget::doLoadSettings( QSettings *Settings )
{
  Q_ASSERT( Settings != NULL );

  CurrentDir = Settings->value("CurrentDir",QCoreApplication::applicationDirPath()).toString();
  resize( Settings->value( "Size", size() ).toSize() );
  move( Settings->value( "Pos", pos() ).toPoint() );
  move( qMax(x(),5), qMax(y(),5) );
  
  OutputWidget->loadSettings( Settings );
  OutputSettingsWidget->loadSettings( Settings );
  ControlWidget->loadSettings( Settings );
}

// -----------------------------------------

QString mainWidget::getSaveFileName()
{
  QString FileName = QFileDialog::getSaveFileName(this, "Save File",
                           CurrentDir,
                           "Dat files (*.dat);; All files (*)");

  if ( ! FileName.isEmpty() )
    CurrentDir = QFileInfo(FileName).dir().path();

  return FileName;
}

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
}

// -----------------------------------------

void measureMainWidget::setupMeasurementThread()
{
  const measureControlWidget *ControlWidget = mainControlWidget<measureControlWidget>();
  if ( ControlWidget == NULL )
    return;
  const measurementParameters *Parameters = ControlWidget->getMeasurementParameters();
  MeasurementThread = createMeasurementThread( *Parameters );
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
  if ( MeasurementThread->isError() || !MeasurementThread->isRunning() )
    stopMeasurement();
  else if ( MeasurementThread->isExistNewData() )
    showMeasurementData();
}

// -----------------------------------------

void measureMainWidget::processMeasurementError()
{
  QMessageBox::warning( this, "Error", "Error while mreasurement:\n" + MeasurementThread->errorString() );
}

// -----------------------------------------

void measureMainWidget::startMeasurement()
{
  MeasurementTimer->start(30);
  MeasurementThread->start();   
}

// -----------------------------------------

void measureMainWidget::stopMeasurement()
{
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
  MeasurementThread->pause();
}

// -----------------------------------------

void measureMainWidget::contMeasurement()
{
  MeasurementThread->cont();
}

// =========================================



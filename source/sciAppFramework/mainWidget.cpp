
// =========================================

#include "sciAppFramework/mainWidget.h"
#include "sciAppFramework/outputWidget.h"
#include "sciAppFramework/outputSettingsWidget.h"
#include "sciAppFramework/controlWidget.h"
#include "sciAppFramework/measurementThread.h"

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
  if ( OutputWidget != NULL )
    setCentralWidget( OutputWidget );
}

// -----------------------------------------

void mainWidget::setupOutputSettingsWidget()
{
  OutputSettingsWidget = createOutputSettingsWidget();
  
  if ( OutputSettingsWidget != NULL && OutputWidget != NULL )
  {
    OutputSettingsWidget->addSettingsWidgets( OutputWidget->listOfSettingsWidgets() );
    connect( OutputWidget, SIGNAL(currentOutputChanged(int)), OutputSettingsWidget, SLOT(setCurrentSettings(int)) );
  }

  if ( OutputSettingsWidget != NULL )
  {
    QDockWidget *DockWidget = new QDockWidget( "Settings", this );
    DockWidget->setAllowedAreas( Qt::BottomDockWidgetArea );
    DockWidget->setWidget( OutputSettingsWidget );
    DockWidget->setFeatures( QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable );
    addDockWidget( Qt::BottomDockWidgetArea, DockWidget );  
  }
}

// -----------------------------------------

void mainWidget::setupControlWidget()
{
  ControlWidget = createControlWidget();
  if ( ControlWidget == NULL )
    return;

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
  Settings->setValue( "LastFileName", LastFileName ); 

  if ( OutputWidget != NULL )
    OutputWidget->saveSettings( Settings );

  if ( OutputSettingsWidget != NULL )
    OutputSettingsWidget->saveSettings( Settings );
  
  if ( ControlWidget != NULL )
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

  LastFileName = Settings->value("LastFileName",QCoreApplication::applicationDirPath()).toString();
  resize( Settings->value( "Size", size() ).toSize() );
  move( Settings->value( "Pos", pos() ).toPoint() );
  move( qMax(x(),5), qMax(y(),5) );
 
  if ( OutputWidget != NULL )
    OutputWidget->loadSettings( Settings );

  if ( OutputSettingsWidget != NULL )
    OutputSettingsWidget->loadSettings( Settings );

  if ( ControlWidget != NULL )
    ControlWidget->loadSettings( Settings );
}

// -----------------------------------------

QString mainWidget::getSaveFileName( const QString &Filter, const QString &DefaultSuffix )
{
  QFileDialog Dialog( this, "Saving in file...", LastFileName );
  Dialog.setFilter( Filter );
  Dialog.setDefaultSuffix( DefaultSuffix );
  Dialog.setAcceptMode( QFileDialog::AcceptSave );

  bool OK = Dialog.exec();
  if ( ! OK )
    return QString();

  const QStringList &Files = Dialog.selectedFiles();

  if ( Files.isEmpty() )
    return QString();

  QString FileName = Files[0];
  LastFileName = FileName; 
  return FileName;
}

// -----------------------------------------

QString mainWidget::getOpenFileName( const QString &Filter )
{
  QFileDialog Dialog( this, "Open file...", LastFileName );
  Dialog.setFilter( Filter );
  Dialog.setAcceptMode( QFileDialog::AcceptOpen );

  bool OK = Dialog.exec();
  if ( ! OK )
    return QString();

  const QStringList &Files = Dialog.selectedFiles();

  if ( Files.isEmpty() )
    return QString();

  QString FileName = Files[0];
  LastFileName = FileName; 
  return FileName;
}

// -----------------------------------------
      
void mainWidget::showErrorMessage( const QString &Message )
{
     QMessageBox::critical( this,
                           "Program error",
                           Message,
                           QMessageBox::Ok,
                           QMessageBox::NoButton,
                           QMessageBox::NoButton );
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
  connect( ControlWidget, SIGNAL(save(QString)), SLOT(saveMeasurement(QString)) );
}

// -----------------------------------------

void measureMainWidget::setupMeasurementThread()
{
  const measureControlWidget *ControlWidget = mainControlWidget<measureControlWidget>();
  if ( ControlWidget == NULL )
    return;
  MeasurementThread = createMeasurementThread( *ControlWidget );
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

  MeasurementTimer->start(30);
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

// =========================================


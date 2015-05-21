
// =========================================

#include "sciAppFramework/mainWidget.h"
#include "sciAppFramework/outputWidget.h"
#include "sciAppFramework/outputSettingsWidget.h"
#include "sciAppFramework/controlWidget.h"

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
  QApplication::setApplicationName( appTitle() );
  QApplication::setWindowIcon( appIcon() );
  setWindowTitle( QApplication::applicationName() );
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

  QString FileName = Files.first();
  LastFileName = FileName; 
  return FileName;
}

// -----------------------------------------
      
QString mainWidget::getSaveDirectory()
{
  QFileDialog Dialog( this, "Saving in directory...", LastFileName );
  Dialog.setFileMode( QFileDialog::DirectoryOnly );
  Dialog.setAcceptMode( QFileDialog::AcceptSave );
  Dialog.setOption( QFileDialog::ShowDirsOnly );

  bool OK = Dialog.exec();
  if ( ! OK )
    return QString();

  const QStringList &Files = Dialog.selectedFiles();

  if ( Files.isEmpty() )
    return QString();

  QString FileName = Files.first();
  LastFileName = FileName; 
  return FileName;
}

// -----------------------------------------

QString mainWidget::getOpenFileName( const QString &Filter )
{
  QFileDialog Dialog( this, "Open file...", LastFileName );
  Dialog.setFilter( Filter );
  Dialog.setAcceptMode( QFileDialog::AcceptOpen );
  Dialog.setFileMode( QFileDialog::ExistingFiles );

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

void mainWidget::setControlWidgetInCorners( bool OnCorners )
{
  if ( OnCorners )
  {
    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);
  } else {
    setCorner(Qt::BottomLeftCorner, Qt::TopDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::BottomDockWidgetArea);
  }
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


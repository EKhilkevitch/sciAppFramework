
// =========================================

#include "sciAppFramework/controlWidget.h"
#include "sciAppFramework/multiInputWidget.h"

#include <QStackedLayout>
#include <QBoxLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QGroupBox>

using namespace sciAppFramework;

// =========================================

controlWidget::controlWidget( QWidget *Parent ) : 
  QWidget(Parent), 
  ParametersWidget(NULL)
{
  setMinimumWidth( 300 );
}

// -----------------------------------------

controlWidget::~controlWidget()
{
}

// -----------------------------------------

void controlWidget::initWidget()
{
  Q_ASSERT( ParametersWidget == NULL );

  QVBoxLayout *Layout = new QVBoxLayout();
  Layout->addWidget( createParametersWidget() );
  Layout->addStretch();
  Layout->addWidget( createBtnWidget() );
  setLayout( Layout );
}

// -----------------------------------------

QWidget* controlWidget::createParametersWidget()
{
  ParametersWidget = createMultiInputWidget();
  connect( ParametersWidget, SIGNAL(changed()), SIGNAL(changed()) );

  QGroupBox *Box = new QGroupBox("Parameters",this);
  QStackedLayout *Layout = new QStackedLayout();
  Layout->addWidget( ParametersWidget );
  Box->setLayout( Layout );

  return Box;
}

// -----------------------------------------

multiInputWidget* controlWidget::createMultiInputWidget()
{
  return new multiInputWidget(this);
}

// -----------------------------------------

QWidget* controlWidget::createBtnWidget()
{
  return new QWidget(this);
}

// -----------------------------------------

void controlWidget::loadSettings( QSettings *Settings )
{
  Q_ASSERT( ParametersWidget != NULL );
  ParametersWidget->loadSettings(Settings);
}

// -----------------------------------------

void controlWidget::saveSettings( QSettings *Settings )
{
  Q_ASSERT( ParametersWidget != NULL );
  ParametersWidget->saveSettings(Settings);
}

// =========================================

measureControlWidget::measureControlWidget( QWidget *Parent ) :
  controlWidget(Parent),
  StartStopLayout(NULL),
  PauseContLayout(NULL)
{
}

// -----------------------------------------

measureControlWidget::~measureControlWidget()
{
  delete StartStopLayout;
  delete PauseContLayout;
  StartStopLayout = PauseContLayout = NULL;
}

// -----------------------------------------

QWidget* measureControlWidget::createBtnWidget()
{
  QBoxLayout *Layout = new QVBoxLayout();
  Layout->addWidget( createControlButtonsWidget() );
  Layout->addWidget( createSaveButtonsWidget() );

  QWidget *Widget = new QWidget(this);
  Widget->setLayout(Layout);
  return Widget;
}

// -----------------------------------------

QWidget* measureControlWidget::createControlButtonsWidget()
{
  makeStartStopButtonsLayout();
  makePauseContinueButtonsLayout();

  QBoxLayout *Layout = new QHBoxLayout();
  if ( enableStartStopButtons() )
    Layout->addLayout( StartStopLayout );
  if ( enablePauseContinueButtons() )
    Layout->addLayout( PauseContLayout );

  QWidget *Widget = new QWidget(this);
  Widget->setMinimumHeight(60);
  Widget->setMaximumHeight(60);
  Widget->setLayout(Layout);
  return Widget;
}

// -----------------------------------------

QWidget* measureControlWidget::createSaveButtonsWidget()
{
  const QStringList &ButtonsNameAndText = saveButtonsTextAndNames();

  if ( ButtonsNameAndText.isEmpty() )
    return new QWidget(this);

  QBoxLayout *Layout = new QHBoxLayout();

  foreach( QString NameAndText, ButtonsNameAndText )
  {
    QPushButton *Button = new QPushButton( buttonsTextFromString(NameAndText) ,this);
    Button->setObjectName( buttonsNameFromString(NameAndText) );
    connect( Button, SIGNAL(clicked()), SLOT(doSave()) );
    Layout->addWidget(Button);
  }

  QWidget *Widget = new QWidget(this);
  Widget->setMinimumHeight(60);
  Widget->setMaximumHeight(60);
  Widget->setLayout(Layout);
  return Widget;

}

// -----------------------------------------

QStringList measureControlWidget::saveButtonsTextAndNames() const
{
  QStringList List;
  List.append( "Data;Save data" );
  List.append( "Screen;Save screen" );
  return List;
}

// -----------------------------------------

QString measureControlWidget::buttonsNameFromString( const QString &NameAndText )
{
  int Index = NameAndText.indexOf(";");
  return NameAndText.left(Index);
}

// -----------------------------------------

QString measureControlWidget::buttonsTextFromString( const QString &NameAndText )
{
  int Index = NameAndText.indexOf(";");
  return NameAndText.mid(Index+1);
}

// -----------------------------------------

void measureControlWidget::makeStartStopButtonsLayout()
{
  Q_ASSERT( StartStopLayout == NULL );
  
  if ( enableStartStopButtons() )
  {
    QPushButton *StartBtn = new QPushButton(startButtonText(),this);
    connect( StartBtn, SIGNAL(clicked()), SLOT(doStart()) );
    QPushButton *StopBtn = new QPushButton(stopButtonText(),this);
    connect( StopBtn, SIGNAL(clicked()), SLOT(doStop()) );
    StartStopLayout = createStackedLayout(StartBtn,StopBtn);
  } else {
    StartStopLayout = new QStackedLayout();
  }
}

// -----------------------------------------

void measureControlWidget::makePauseContinueButtonsLayout()
{
  Q_ASSERT( PauseContLayout == NULL );

  if ( enablePauseContinueButtons() )
  {
    QPushButton *PauseBtn = new QPushButton(pauseButtonText(),this);
    connect( PauseBtn, SIGNAL(clicked()), SLOT(doPause()) );
    QPushButton *ContBtn = new QPushButton(continueButtonText(),this);
    connect( ContBtn, SIGNAL(clicked()), SLOT(doContinue()) );
    PauseContLayout = createStackedLayout( PauseBtn, ContBtn );
  } else {
    PauseContLayout = new QStackedLayout();
  }

  setEnableLayout( PauseContLayout, false );
}

// -----------------------------------------

QStackedLayout* measureControlWidget::createStackedLayout( QWidget *FirstWidget, QWidget *SecondWidget )
{
  QStackedLayout *Layout = new QStackedLayout();
  if ( FirstWidget != NULL )
    Layout->addWidget(FirstWidget);
  if ( SecondWidget != NULL )
    Layout->addWidget(SecondWidget);
  return Layout;
}

// -----------------------------------------

void measureControlWidget::setEnableLayout( QLayout *Layout, bool Enable )
{
  if ( Layout == NULL )
    return;

  for ( int i = 0; i < Layout->count(); i++ )
  {
    if ( Layout->itemAt(i)->widget() != NULL )
      Layout->itemAt(i)->widget()->setEnabled(Enable);
    if ( Layout->itemAt(i)->layout() != NULL )
      setEnableLayout( Layout->itemAt(i)->layout(), Enable );
  }
}

// -----------------------------------------

void measureControlWidget::prepareToStart()
{
  Q_ASSERT( PauseContLayout != NULL );
  Q_ASSERT( StartStopLayout != NULL );

  getParameters().setEnabled(false);
  PauseContLayout->setCurrentIndex(0);
  StartStopLayout->setCurrentIndex(1);
  setEnableLayout( PauseContLayout, true );
}

// -----------------------------------------

void measureControlWidget::doStart()
{
  prepareToStart();
  emit start();
}

// -----------------------------------------

void measureControlWidget::prepareToStop()
{
  Q_ASSERT( PauseContLayout != NULL );
  Q_ASSERT( StartStopLayout != NULL );
  
  getParameters().setEnabled(true);
  StartStopLayout->setCurrentIndex(0);
  setEnableLayout( PauseContLayout, false );
}

// -----------------------------------------

void measureControlWidget::doStop()
{
  prepareToStop();
  emit stop();
}

// -----------------------------------------

void measureControlWidget::prepareToPause()
{
  Q_ASSERT( PauseContLayout != NULL );
  PauseContLayout->setCurrentIndex(1);
}

// -----------------------------------------

void measureControlWidget::doPause()
{
  prepareToPause();
  emit pause();
}

// -----------------------------------------

void measureControlWidget::prepareToContinue()
{
  Q_ASSERT( PauseContLayout != NULL );
  PauseContLayout->setCurrentIndex(0);
}

// -----------------------------------------

void measureControlWidget::doContinue()
{
  prepareToContinue();
  emit cont();
}

// -----------------------------------------

void measureControlWidget::doSave()
{
  QObject *Sender = sender();
  if ( Sender == NULL )
    return;
  QString SavedObjectName = Sender->objectName();
  emit save(SavedObjectName);
}

// -----------------------------------------
      
QVariant measureControlWidget::getVariantValue( const QString &Name ) const
{
  return getParameters().getVariantValue( Name );
}

// =========================================




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
  ParametersWidget->loadSettings(Settings);
}

// -----------------------------------------

void controlWidget::saveSettings( QSettings *Settings )
{
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
}

// -----------------------------------------

QWidget* measureControlWidget::createBtnWidget()
{
  QPushButton *StartBtn = new QPushButton("Start",this);
  connect( StartBtn, SIGNAL(clicked()), SLOT(doStart()) );

  QPushButton *StopBtn = new QPushButton("Stop",this);
  connect( StopBtn, SIGNAL(clicked()), SLOT(doStop()) );

  StartStopLayout = new QStackedLayout();
  StartStopLayout->addWidget( StartBtn );
  StartStopLayout->addWidget( StopBtn );

  QPushButton *PauseBtn = new QPushButton("Pause",this);
  connect( PauseBtn, SIGNAL(clicked()), SLOT(doPause()) );

  QPushButton *ContBtn = new QPushButton("Continue",this);
  connect( ContBtn, SIGNAL(clicked()), SLOT(doContinue()) );

  PauseContLayout = new QStackedLayout();
  PauseContLayout->addWidget( PauseBtn );
  PauseContLayout->addWidget( ContBtn );
  setEnableLayout( PauseContLayout, false );

  QBoxLayout *Layout = new QHBoxLayout();
  Layout->addLayout( StartStopLayout );
  Layout->addLayout( PauseContLayout );

  QWidget *Widget = new QWidget();
  Widget->setMinimumHeight(60);
  Widget->setMaximumHeight(60);
  Widget->setLayout(Layout);
  return Widget;
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
  PauseContLayout->setCurrentIndex(0);
}

// -----------------------------------------

void measureControlWidget::doContinue()
{
  prepareToContinue();
  emit cont();
}

// -----------------------------------------
      
QVariant measureControlWidget::getVariantValue( const QString &Name ) const
{
  return getParameters().getVariantValue( Name );
}

// =========================================



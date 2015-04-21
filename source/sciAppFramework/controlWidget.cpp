
// =========================================

#include "sciAppFramework/controlWidget.h"
#include "sciAppFramework/multiInputWidget.h"

#include <QStackedLayout>
#include <QBoxLayout>
#include <QVBoxLayout>
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
      
multiInputWidget& controlWidget::getParameters() 
{ 
  return *ParametersWidget; 
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



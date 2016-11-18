
// ======================================================

#include "sciAppFramework/waitWidget.h"

#include <QLabel>
#include <QWidget>
#include <QStackedLayout>
#include <QLabel>
#include <QTimer>
#include <QThread>

using namespace sciAppFramework;

// ======================================================

waitWidget::waitWidget( QWidget *Parent ) : 
  QDialog(Parent)
{
}

// ------------------------------------------------------

waitWidget::~waitWidget()
{
}

// ------------------------------------------------------
      
QString waitWidget::title() const 
{ 
  return "Please wait..."; 
}

// ------------------------------------------------------

void waitWidget::initWidget()
{
  setWidgetPropertis();
  
  QLabel *Label = new QLabel("Please wait",this);
  Label->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  
  QStackedLayout *Layout = new QStackedLayout();
  Layout->addWidget( Label );
  setLayout( Layout );
}

// ------------------------------------------------------

void waitWidget::setWidgetPropertis()
{
  setMinimumWidth( 350 );
  setMinimumHeight( 150 );
  setWindowModality( Qt::ApplicationModal );
  setWindowFlags( windowFlags() & (~Qt::WindowContextHelpButtonHint) );
  
  setWindowTitle( title() );
}

// ------------------------------------------------------

void waitWidget::acceptOnThreadDone()
{
  if ( Thread == NULL )
    return;
  if ( Thread->isFinished() )
    accept();
}

// ------------------------------------------------------

int waitWidget::exec()
{
  return QDialog::exec();
}

// ------------------------------------------------------

int waitWidget::execUntilThreadRun( QThread *Thread )
{
  this->Thread = Thread;

  QTimer Timer;
  connect( &Timer, SIGNAL(timeout()), SLOT(acceptOnThreadDone()) );
  Timer.start(25);

  return exec();
}

// ======================================================



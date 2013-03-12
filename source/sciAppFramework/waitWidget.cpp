
// ======================================================

#include "sciAppFramework/waitWidget.h"

#include <QLabel>
#include <QWidget>
#include <QStackedLayout>
#include <QLabel>

using namespace sciAppFramework;

// ======================================================

waitWidget::waitWidget( QWidget *Parent ) : QDialog(Parent)
{
}

// ------------------------------------------------------

waitWidget::~waitWidget()
{
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

// ======================================================



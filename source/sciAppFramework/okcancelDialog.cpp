
// ======================================================

#include "sciAppFramework/okcancelDialog.h"

#include <QStackedLayout>
#include <QBoxLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

using namespace sciAppFramework;

// ======================================================

QWidget* okcancelDialog::createButtonsWidget()
{
  QPushButton *OkButton = new QPushButton( acceptButtonName() ,this );
  connect( OkButton, SIGNAL(clicked()), SLOT(accept()) );

  QPushButton *CancelButton = new QPushButton( rejectButtonName(), this );
  connect( CancelButton, SIGNAL(clicked()), SLOT(reject()) );

  QBoxLayout *Layout = new QHBoxLayout();
  Layout->addWidget( OkButton );
  Layout->addWidget( CancelButton );

  QWidget *Widget = new QWidget(this);
  Widget->setLayout(Layout);
  return Widget;
}

// ------------------------------------------------------

void okcancelDialog::initWidget()
{
  QWidget *ButtonsWidget = createButtonsWidget();
  QWidget *ContentWidget = createContentWidget();

  Q_ASSERT( ButtonsWidget != NULL );
  Q_ASSERT( ContentWidget != NULL );
  
  QBoxLayout *Layout = new QVBoxLayout();
  Layout->addWidget( ContentWidget );
  Layout->setSpacing( 5 );
  Layout->addWidget( ButtonsWidget );
  setLayout( Layout );
}

// ======================================================


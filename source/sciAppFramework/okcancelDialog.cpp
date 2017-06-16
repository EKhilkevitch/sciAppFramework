
// ======================================================

#include "sciAppFramework/okcancelDialog.h"

#include <QStackedLayout>
#include <QBoxLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

using namespace sciAppFramework;

// ======================================================
      
okcancelDialog::okcancelDialog( QWidget *Parent ) : 
  QDialog(Parent) 
{
}

// ------------------------------------------------------

okcancelDialog::~okcancelDialog()
{
}

// ------------------------------------------------------

QWidget* okcancelDialog::createButtonsWidget()
{
  const QString AcceptButtonName = acceptButtonName();
  QPushButton *OkButton = NULL;
  if ( ! AcceptButtonName.isEmpty() )
  {
    OkButton = new QPushButton( AcceptButtonName, this );
    OkButton->setDefault( true );
    connect( OkButton, SIGNAL(clicked()), SLOT(accept()) );
  }

  const QString RejectButtonName = rejectButtonName();
  QPushButton *CancelButton = NULL;
  if ( ! RejectButtonName.isEmpty() )
  {
    CancelButton = new QPushButton( RejectButtonName, this );
    connect( CancelButton, SIGNAL(clicked()), SLOT(reject()) );
  }

  QBoxLayout *Layout = new QHBoxLayout();
  if ( OkButton != NULL )
    Layout->addWidget( OkButton );
  if ( CancelButton != NULL )
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

// ------------------------------------------------------
      
void okcancelDialog::accept()
{
  if ( acceptEnabled() )
    QDialog::accept();
}

// ------------------------------------------------------

void okcancelDialog::reject()
{
  if ( rejectEnabled() )
    QDialog::reject();
}

// ------------------------------------------------------

QString okcancelDialog::acceptButtonName() const 
{ 
  return "Ok"; 
}

// ------------------------------------------------------

QString okcancelDialog::rejectButtonName() const 
{ 
  return "Cancel"; 
}

// ------------------------------------------------------
      
bool okcancelDialog::acceptEnabled() 
{ 
  return true; 
}

// ------------------------------------------------------

bool okcancelDialog::rejectEnabled() 
{ 
  return true; 
}

// ======================================================


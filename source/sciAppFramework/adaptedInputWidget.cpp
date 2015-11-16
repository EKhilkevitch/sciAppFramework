
// ======================================================

#include "sciAppFramework/adaptedInputWidget.h"

#include <QLabel>
#include <QTextEdit>
#include <QRadioButton>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedLayout>
#include <QGroupBox>
#include <QDebug>

using namespace sciAppFramework;

// ======================================================

radioButtonWidget::radioButtonWidget( const QString& LabelText, QWidget *Parent ) : 
  inputWidget( Parent, LabelText ) 
{ 
  initWidget( LabelText ); 
}

// ------------------------------------------------------
      
void radioButtonWidget::initWidget( const QString &LabelText )
{
  ButtonsBox = new QGroupBox(LabelText,this);
  ButtonsBox->setLayout( new QVBoxLayout() );

  QBoxLayout *Layout = createLayoutWithoutMargins<QVBoxLayout>();
  Layout->addWidget( ButtonsBox );
  setLayout( Layout );
}

// ------------------------------------------------------

radioButtonWidget::radioButtonPair::radioButtonPair() : 
  Button(NULL) 
{
}

// ------------------------------------------------------

radioButtonWidget::radioButtonPair::radioButtonPair( QRadioButton *B, const QVariant &D ) : 
  Button(B), 
  Data(D) 
{
}

// ------------------------------------------------------

QVariant radioButtonWidget::valueToSettings() const 
{ 
  return currentIndex(); 
}

// ------------------------------------------------------

void radioButtonWidget::setValueFromSettings( const QVariant &Value ) 
{ 
  setCurrentIndex( Value.toInt() ); 
}
      
// ------------------------------------------------------

QVariant radioButtonWidget::getVariantValue() const 
{ 
  return currentData(); 
}

// ------------------------------------------------------

void radioButtonWidget::setVariantValue( const QVariant &Value ) 
{ 
  setCurrentData( Value ); 
}

// ------------------------------------------------------
      
radioButtonWidget& radioButtonWidget::setOrientation( Qt::Orientation Orientation )
{
  QBoxLayout *Layout = NULL;
  switch ( Orientation )
  {
    case Qt::Vertical:
      Layout = createLayoutWithoutMargins<QVBoxLayout>();
      break;

    case Qt::Horizontal:
      Layout = createLayoutWithoutMargins<QHBoxLayout>();
      Layout->setContentsMargins( 5, 5, 5, 5 );
      break;

    default:
      return *this;
  }

  Q_ASSERT( Layout != NULL );

  while ( true )
  {
    QLayoutItem *Child = ButtonsBox->layout()->takeAt(0);
    if ( Child == NULL )
      break;
    Layout->addItem( Child );
  }

  delete ButtonsBox->layout();
  ButtonsBox->setLayout( Layout );

  return *this;
}

// ------------------------------------------------------
      
radioButtonWidget& radioButtonWidget::setLayoutSpacing( int Spacing )
{
  QBoxLayout *Layout = dynamic_cast<QBoxLayout*>( layout() );
  if ( Layout != NULL )
    Layout->setSpacing( Spacing );
  return *this;
}

// ------------------------------------------------------

const QString radioButtonWidget::label() const
{
  return ButtonsBox->title();
}

// ------------------------------------------------------

int radioButtonWidget::count() const 
{ 
  return RadioButtons.size(); 
}

// ------------------------------------------------------

int radioButtonWidget::currentIndex() const
{
  for ( int i = 0; i < RadioButtons.size(); i++ )
    if ( RadioButtons[i].Button->isChecked() )
      return i;
  return -1;
}

// ------------------------------------------------------

QVariant radioButtonWidget::currentData() const 
{ 
  return RadioButtons.value( currentIndex() ).Data; 
}

// ------------------------------------------------------

radioButtonWidget& radioButtonWidget::addItem( const QString &Text, const QVariant &UserData )
{
  QRadioButton *Button = new QRadioButton( Text, this );
  ButtonsBox->layout()->addWidget( Button );
  RadioButtons.append( radioButtonPair(Button,UserData) );
  connect( Button, SIGNAL(clicked()), SLOT(oneOfButtonsChecked()) );
  connect( Button, SIGNAL(clicked()), SIGNAL(changed()) );
  if ( RadioButtons.size() == 1 )
    Button->setChecked(true);
  return *this;
}

// ------------------------------------------------------
      
void radioButtonWidget::clear()
{
  foreach ( radioButtonPair P, RadioButtons )
    delete P.Button;
  RadioButtons.clear();
}

// ------------------------------------------------------

radioButtonWidget& radioButtonWidget::setCurrentIndex( int Index )
{
  if ( Index < RadioButtons.size() && Index >= 0 )
    RadioButtons[Index].Button->setChecked(true);
  return *this;
}

// ------------------------------------------------------

radioButtonWidget& radioButtonWidget::setCurrentData( const QVariant& Value )
{
  for ( int i = 0; i < RadioButtons.size(); i++ )
  {
    if ( RadioButtons[i].Data == Value )
    {
      setCurrentIndex(i);
      break;
    }
  }
  return *this;
}

// ------------------------------------------------------

void radioButtonWidget::oneOfButtonsChecked()
{
  emit currentIndexChanged( currentIndex() );
}

// ======================================================

checkBoxWidget::checkBoxWidget( const QString& LabelText, QWidget *Parent ) : 
  inputWidget( Parent, LabelText ) 
{ 
  initWidget(LabelText); 
}

// ------------------------------------------------------
      
checkBoxWidget::checkBoxWidget( const QString& LabelText, bool Value, QWidget *Parent ) : 
  inputWidget( Parent, LabelText ) 
{ 
  initWidget(LabelText); 
  setChecked(Value); 
}

// ------------------------------------------------------

void checkBoxWidget::initWidget( const QString &LabelText )
{
  CheckBox = new QCheckBox( LabelText, this );

  QLayout *Layout = createLayoutWithoutMargins<QStackedLayout>();
  Layout->addWidget( CheckBox );
  setLayout( Layout );

  connect( CheckBox, SIGNAL(stateChanged(int)), SIGNAL(stateChanged(int)) );
  connect( CheckBox, SIGNAL(clicked()), SIGNAL(clicked()) );
  connect( CheckBox, SIGNAL(clicked()), SLOT(emitCheckedSignal()) );
  connect( CheckBox, SIGNAL(toggled(bool)), SLOT(emitCheckedSignal()) );
}

// ------------------------------------------------------

QVariant checkBoxWidget::valueToSettings() const 
{ 
  return isChecked(); 
}

// ------------------------------------------------------

void checkBoxWidget::setValueFromSettings( const QVariant &Value ) 
{ 
  setChecked( Value.toBool() ); 
}

// ------------------------------------------------------

QVariant checkBoxWidget::getVariantValue() const 
{ 
  return isChecked(); 
}

// ------------------------------------------------------

void checkBoxWidget::setVariantValue( const QVariant &Value ) 
{ 
  setChecked( Value.toBool() ); 
}

// ------------------------------------------------------

const QString checkBoxWidget::label() const 
{ 
  return CheckBox->text(); 
}

// ------------------------------------------------------

Qt::CheckState checkBoxWidget::checkState() const 
{ 
  return CheckBox->checkState(); 
}

// ------------------------------------------------------

void checkBoxWidget::setTristate( bool Set ) 
{ 
  CheckBox->setTristate(Set); 
}

// ------------------------------------------------------

bool checkBoxWidget::isTristate() const 
{ 
  return CheckBox->isTristate(); 
}

// ------------------------------------------------------

bool checkBoxWidget::isChecked() const 
{ 
  return CheckBox->isChecked(); 
}

// ------------------------------------------------------

void checkBoxWidget::setChecked( bool Checked ) 
{ 
  CheckBox->setChecked(Checked); 
  emitCheckedSignal(); 
} 

// ------------------------------------------------------

void checkBoxWidget::setCheckState( Qt::CheckState State ) 
{ 
  CheckBox->setCheckState(State); 
}

// ------------------------------------------------------
      
void checkBoxWidget::emitCheckedSignal()
{
  emit toggled( isChecked() );
  emit changed();
}

// ======================================================

multilineEditWidget::multilineEditWidget( const QString &LabelText, QWidget *Parent ) :
  inputWidget( Parent, LabelText ) 
{ 
  initWidget( LabelText ); 
}

// ------------------------------------------------------

multilineEditWidget::multilineEditWidget( const QString &LabelText, const QString &Text, QWidget *Parent ) : 
  inputWidget( Parent, LabelText ) 
{
  initWidget( LabelText ); 
  setText( Text );
}

// ------------------------------------------------------

void multilineEditWidget::initWidget( const QString &LabelText )
{
  Label = new QLabel( LabelText, this );
  TextEdit = new QTextEdit( this );

  QBoxLayout *Layout = createLayoutWithoutMargins<QVBoxLayout>();
  Layout->addWidget( Label );
  Layout->addWidget( TextEdit );
  setLayout( Layout );
}

// ------------------------------------------------------

QVariant multilineEditWidget::valueToSettings() const
{
  return text();
}

// ------------------------------------------------------

void multilineEditWidget::setValueFromSettings( const QVariant &Value )
{
  setText( Value.toString() );
}

// ------------------------------------------------------

QVariant multilineEditWidget::getVariantValue() const
{
  return text();
}

// ------------------------------------------------------

void multilineEditWidget::setVariantValue( const QVariant &Value )
{
  setText( Value.toString() );
}

// ------------------------------------------------------

const QString multilineEditWidget::label() const
{
  return Label->text();
}

// ------------------------------------------------------

const QString multilineEditWidget::text() const
{
  return TextEdit->toPlainText();
}

// ------------------------------------------------------

multilineEditWidget& multilineEditWidget::setText( const QString &String )
{
  TextEdit->setPlainText( String );
  return *this;
}

// ------------------------------------------------------

multilineEditWidget& multilineEditWidget::setReadOnly( bool ReadOnly )
{
  TextEdit->setReadOnly( ReadOnly );
  return *this;
}
      
// ------------------------------------------------------

multilineEditWidget& multilineEditWidget::setLayoutSpacing( int Spacing )
{
  QBoxLayout *Layout = dynamic_cast<QBoxLayout*>( layout() );
  if ( Layout != NULL )
    Layout->setSpacing( Spacing );
  return *this;
}

// ======================================================



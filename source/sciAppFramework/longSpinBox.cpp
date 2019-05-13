
// ======================================================

#include "sciAppFramework/longSpinBox.h"

#include <QLineEdit>
#include <QValidator>

#include <limits>

using namespace sciAppFramework;

// ======================================================

class longSpinBox::longValidator : public QValidator
{
  private:
    qlonglong Top, Bottom;

  public:
    explicit longValidator( QObject *Parent = NULL );

    qlonglong bottom() const { return Bottom; }
    void setBottom( qlonglong Value ) { setRange( top(), Value ); }
    
    qlonglong top() const { return Top; }
    void setTop( qlonglong Value ) { setRange( bottom(), Value ); }

    void setRange( qlonglong Bottom, qlonglong Top );

    QValidator::State validate( QString &Input, int &Position ) const;
};

// ------------------------------------------------------

longSpinBox::longValidator::longValidator( QObject *Parent ) :
  QValidator( Parent ),
  Top( std::numeric_limits<qlonglong>::max() ),
  Bottom( std::numeric_limits<qlonglong>::min() )
{
}

// ------------------------------------------------------

void longSpinBox::longValidator::setRange( qlonglong Bottom, qlonglong Top )
{
  this->Bottom = qMin( Bottom, Top );
  this->Top = qMax( Bottom, Top );
}

// ------------------------------------------------------

QValidator::State longSpinBox::longValidator::validate( QString &Input, int &Position ) const
{
  if ( Input.contains( QLatin1Char(' ')) )
    return Invalid;

  if ( Input.isEmpty() || ( Bottom < 0 && Input == QLatin1String("-")) )
    return Intermediate;

  bool Ok;
  qlonglong Entered = Input.toLongLong( &Ok );

  if ( !Ok || ( Entered < 0 && Bottom >= 0 ) ) 
  {
    return Invalid;
  } else if ( Entered >= Bottom && Entered <= Top ) 
  {
    return Acceptable;
  } else {
    if ( Entered >= 0 )
      return Entered > Top ? Invalid : Intermediate;
    else
      return Entered < Bottom ? Invalid : Intermediate;
  }
}

// ======================================================

longSpinBox::longSpinBox( QWidget *Parent ) :
  QAbstractSpinBox( Parent ),
  Validator( new longValidator(this) )
{
  connect( this, SIGNAL( editingFinished() ), SLOT( emitValueChanged() ) );
}
  
// ------------------------------------------------------
      
qlonglong longSpinBox::maximum() const
{
  longValidator *LongValidator = dynamic_cast<longValidator*>( Validator );
  if ( LongValidator != NULL )
    return LongValidator->top();
  return 0;
}

// ------------------------------------------------------

qlonglong longSpinBox::minimum() const
{
  longValidator *LongValidator = dynamic_cast<longValidator*>( Validator );
  if ( LongValidator != NULL )
    return LongValidator->bottom();
  return 0;
}

// ------------------------------------------------------

void longSpinBox::setMaximum( qlonglong Max )
{
  longValidator *LongValidator = dynamic_cast<longValidator*>( Validator );
  if ( LongValidator != NULL )
    LongValidator->setTop( Max );
}

// ------------------------------------------------------

void longSpinBox::setMinimum( qlonglong Min )
{
  longValidator *LongValidator = dynamic_cast<longValidator*>( Validator );
  if ( LongValidator != NULL )
    LongValidator->setBottom( Min );
}

// ------------------------------------------------------

void longSpinBox::setRange( qlonglong Min, qlonglong Max )
{
  dynamic_cast<longValidator*>( Validator )->setRange( Min, Max ); 
}

// ------------------------------------------------------

qlonglong longSpinBox::value() const
{
  return lineEdit()->text().toLongLong();
}

// ------------------------------------------------------

void longSpinBox::setValue( qlonglong Value )
{
  lineEdit()->setText( QString::number(Value) );
}

// ------------------------------------------------------
      
QValidator::State longSpinBox::validate( QString &Input, int &Pos ) const
{
  return Validator->validate( Input, Pos );
}

// ------------------------------------------------------

void longSpinBox::fixup( QString &Input ) const
{
  Validator->fixup( Input );
}

// ------------------------------------------------------

void longSpinBox::stepBy( int Steps )
{
  longValidator *LongValidator = dynamic_cast<longValidator*>( Validator );

  qlonglong Value = lineEdit()->text().toLongLong();

  if ( Steps == 0 )
    return;

  if ( Steps > 0 )
  {
    Value = qMin( Value, LongValidator->top() - Steps ) + Steps;
  }
  
  if ( Steps < 0 )
  {
    Value = qMax( Value, LongValidator->bottom() - Steps ) + Steps;
  }

  lineEdit()->setText( QString::number(Value) );
}

// ------------------------------------------------------
      
longSpinBox::StepEnabled longSpinBox::stepEnabled() const
{
  longValidator *LongValidator = dynamic_cast<longValidator*>( Validator );
  qlonglong Value = lineEdit()->text().toLongLong();
  if ( Value >= LongValidator->top() )
    return StepDownEnabled;
  if ( Value <= LongValidator->bottom() )
    return StepUpEnabled;
  return StepUpEnabled | StepDownEnabled;
}

// ------------------------------------------------------
      
void longSpinBox::emitValueChanged()
{
  emit valueChanged( value() );
}

// ======================================================



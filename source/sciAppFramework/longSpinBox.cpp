
// ======================================================

#include "sciAppFramework/longSpinBox.h"

#include <QLineEdit>
#include <QValidator>

#include <limits>

using namespace sciAppFramework;

// ======================================================

namespace 
{

  // ------------------------------------------------------
  
  class longValidator : public QValidator
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

  longValidator::longValidator( QObject *Parent ) :
    QValidator( Parent ),
    Top( std::numeric_limits<qlonglong>::max() ),
    Bottom( std::numeric_limits<qlonglong>::min() )
  {
  }

  // ------------------------------------------------------
  
  void longValidator::setRange( qlonglong Bottom, qlonglong Top )
  {
    this->Bottom = qMin( Bottom, Top );
    this->Top = qMax( Bottom, Top );
  }
  
  // ------------------------------------------------------
  
  QValidator::State longValidator::validate( QString &Input, int &Position ) const
  {
    if ( Input.contains( QLatin1Char(' ')) )
      return Invalid;

    if ( Input.isEmpty() || ( Bottom < 0 && Input == QLatin1String("-")) )
      return Intermediate;

    bool Ok;
    int Entered = Input.toLongLong( &Ok );

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

  // ------------------------------------------------------

};

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
  return dynamic_cast<longValidator*>( Validator )->top();
}

// ------------------------------------------------------

qlonglong longSpinBox::minimum() const
{
  return dynamic_cast<longValidator*>( Validator )->bottom();
}

// ------------------------------------------------------

void longSpinBox::setMaximum( qlonglong Max )
{
  dynamic_cast<longValidator*>( Validator )->setTop( Max ); 
}

// ------------------------------------------------------

void longSpinBox::setMinimum( qlonglong Min )
{
  dynamic_cast<longValidator*>( Validator )->setBottom( Min ); 
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



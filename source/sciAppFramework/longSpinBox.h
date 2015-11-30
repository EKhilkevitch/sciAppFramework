
#pragma once

// ======================================================

#include <QAbstractSpinBox>

// ======================================================

namespace sciAppFramework
{

  // ======================================================
 
  class longSpinBox : public QAbstractSpinBox
  {
    Q_OBJECT

    private:
      QValidator *Validator;

    protected:
      StepEnabled stepEnabled() const;

    private slots:
      void emitValueChanged();

    public:
      explicit longSpinBox( QWidget *Parent = NULL );

      qlonglong maximum() const;
      qlonglong minimum() const;

      void setMaximum( qlonglong Max );
      void setMinimum( qlonglong Min );
      void setRange( qlonglong Min, qlonglong Max );
      
      void setSingleStep( qlonglong Step );
      qlonglong singleStep() const;
      
      qlonglong value() const;
      
      QValidator::State validate( QString &Input, int &Pos ) const;
      void fixup( QString &Input ) const;
      void stepBy( int Steps );

    public slots:
      void setValue( qlonglong value );

    signals:
      void valueChanged( qlonglong Value );
      void valueChanged( const QString &Text );
  };

  // ======================================================

}



#pragma once

// ======================================================

#include <QWidget>
#include <QList>
#include <QVariant>

#include "sciAppFramework/inputWidget.h"

class QLabel;
class QTextEdit;
class QCheckBox;
class QRadioButton;
class QGroupBox;

// ======================================================

namespace sciAppFramework
{

  // ======================================================

  class radioButtonWidget : public inputWidget
  {
    Q_OBJECT

    private:
      struct radioButtonPair
      {
        QRadioButton *Button;
        QVariant Data;

        radioButtonPair();
        radioButtonPair( QRadioButton *Button, const QVariant &Data );
      };

    private:
      QList< radioButtonPair > RadioButtons;
      QGroupBox *ButtonsBox;

    protected:
      QVariant valueToSettings() const;
      void setValueFromSettings( const QVariant &Value );

      void initWidget( const QString &LabelText );

    private slots:
      void oneOfButtonsChecked();

    public:
      explicit radioButtonWidget( const QString &LabelText, QWidget *Parent = NULL );
      
      QVariant getVariantValue() const;
      void setVariantValue( const QVariant &Value );

      radioButtonWidget& setOrientation( Qt::Orientation Orientation );
      radioButtonWidget& setLayoutSpacing( int Spacing );

      const QString label() const;

      int currentIndex() const;
      int count() const;
      QVariant currentData() const;
      radioButtonWidget& addItem( const QString &Text, const QVariant &UserData = QVariant() );
      void clear();
      radioButtonWidget& setCurrentIndex( int Index );
      radioButtonWidget& setCurrentData( const QVariant &Value );

    signals:
      void currentIndexChanged( int Index );
  };
  
  // ======================================================
 
  class checkBoxWidget : public inputWidget
  {
    Q_OBJECT

    private:
      QCheckBox *CheckBox;
    
    protected:
      QVariant valueToSettings() const;
      void setValueFromSettings( const QVariant &Value );

    private:
      void initWidget( const QString &LabelText );

    private slots:
      void emitCheckedSignal();

    public:
      explicit checkBoxWidget( const QString& LabelText, QWidget *Parent = NULL );
      checkBoxWidget( const QString& LabelText, bool Value, QWidget *Parent = NULL );
      
      QVariant getVariantValue() const;
      void setVariantValue( const QVariant &Value );

      const QString label() const;

      Qt::CheckState checkState() const;
      void setTristate( bool Set = true );
      bool isTristate() const;
      bool isChecked() const;
      void setChecked( bool Checked );

    public slots:
      void setCheckedOn() { setCheckState(Qt::Checked); }
      void setCheckedOff() { setCheckState(Qt::Unchecked); }
      void setCheckedPartial() { setCheckState(Qt::PartiallyChecked); }
      void setCheckState( int State ) { setCheckState( static_cast<Qt::CheckState>(State) ); }
      void setCheckState( Qt::CheckState State );
    
    signals:
      void clicked();
      void stateChanged( int );
      void toggled( bool );
  };
  
  // ======================================================
  
  class multilineEditWidget : public inputWidget
  {
    Q_OBJECT

    private:
      QLabel *Label;
      QTextEdit *TextEdit;

    protected:
      QVariant valueToSettings() const;
      void setValueFromSettings( const QVariant &Value );
     
    private:
      void initWidget( const QString &LabelText );

    public:
      explicit multilineEditWidget( const QString &LabelText, QWidget *Parent = NULL );
      multilineEditWidget( const QString &LabelText, const QString &Text, QWidget *Parent = NULL );

      QVariant getVariantValue() const;
      void setVariantValue( const QVariant &Value );
      
      const QString label() const;

      const QString text() const;
      multilineEditWidget& setText( const QString &String );
      
      multilineEditWidget& setReadOnly( bool ReadOnly );
      multilineEditWidget& setLayoutSpacing( int Spacing );
  };

  // ======================================================

}


#pragma once

// ======================================================

#include <QWidget>
#include <QFileDialog>
#include <QList>
#include <QVariant>

#include "sciAppFramework/settingsObject.h"

class QLabel;
class QLineEdit;
class QTextEdit;
class QSpinBox;
class QDoubleSpinBox;
class QComboBox;
class QCheckBox;
class QRadioButton;
class QSettings;
class QGroupBox;
class QValidator;
class QDoubleValidator;
class QVariant;

// ======================================================

namespace sciAppFramework
{

  // ======================================================

  class inputWidget : public QWidget, public singleSettingsObject
  {
    Q_OBJECT

    protected:
      QVariant valueToSettings() const;
      void setValueFromSettings( const QVariant& Value );

    public:
      explicit inputWidget( QWidget *Parent, const QString &SettingsName = QString() );
      virtual ~inputWidget() {}
   
      virtual const QString label() const = 0;
      
      virtual QVariant getVariantValue() const = 0;
      virtual void setVariantValue( const QVariant &Value ) = 0;

      template <class input> static input* create( QWidget *Parent, const QString &Name, const QString &Label, const QVariant &Value = QVariant() );
      template <class input> static input* create( const QString &Name, const QString &Label, const QVariant &Value = QVariant() );
      template <class layoutType> static layoutType* createLayoutWithoutMargins();

    signals:
      void changed();

    public slots:
      void setEnabled( int Enabled );
  };

  // ======================================================

  class labelInputWidget : public inputWidget
  {
    Q_OBJECT

    private:
      QLabel  *LabelWidget;
      QWidget *InputWidget;

    protected:
      template <class input> input* getInput() { return dynamic_cast<input*>( InputWidget ); }
      template <class input> const input* getInput() const { return dynamic_cast<const input*>( InputWidget ); }
      
      virtual QLabel* createLabelWidget( const QString &LabelText );
      virtual QWidget* createInputWidget() = 0;

      void initWidget( const QString &LabelText );
      int spacingWidgth() const;

    public:
      explicit labelInputWidget( QWidget *Parent = NULL, const QString &SettingsName = QString() );

      void setInputStyleSheet( const QString &Style );
      void setLabelStyleSheet( const QString &Style );

      const QString label() const;
      void setStretchFactors( int LabelStretch, int InputStretch );
  };

  // ======================================================

  class labelEditWidget : public labelInputWidget
  {
    Q_OBJECT

    protected:
      virtual QLineEdit* getLineEdit();
      const QLineEdit* getLineEdit() const;

      QWidget* createInputWidget(); 

    protected:
      labelEditWidget( int, QWidget *Parent = NULL, const QString &SettingsName = QString() );

    public:
      explicit labelEditWidget( const QString& LabelText, QWidget *Parent = NULL );
      labelEditWidget( const QString& LabelText, QValidator *Validator, QWidget *Parent = NULL );
      labelEditWidget( const QString& LabelText, const QString& Text, QWidget *Parent );
      labelEditWidget( const QString& LabelText, const QString& Text, QValidator *Validator = NULL, QWidget *Parent = NULL );

      QVariant getVariantValue() const;
      void setVariantValue( const QVariant &Value );

      QString text() const;
      void setText( const QString& String );
      void setValidator( QValidator *Validator );
      
      void setReadOnly( bool ReadOnly );

    signals:
      void editingFinished();
      void returnPressed();
      void cursorPositionChanged(int,int);
      void selectionChanged();
      void textChanged(const QString&);
      void textEdited(const QString&);
  };

  // ======================================================
  
  class labelDoubleEditWidget : public labelEditWidget
  {
    Q_OBJECT

    private:
      QString PrintfFormat;

    private:
      // Disable usage of this functions
      void setText( const QString& S ) { labelEditWidget::setText(S); } 
      void setValidator( QValidator *Validator ) { labelEditWidget::setValidator(Validator); }
      QString text() const { return labelEditWidget::text(); }

      QDoubleValidator* doubleValidator();
      const QDoubleValidator* doubleValidator() const;

    protected:
      QWidget* createInputWidget(); 

    public:
      explicit  labelDoubleEditWidget( const QString& LabelText, QWidget *Parent = NULL );
      labelDoubleEditWidget( const QString& LabelText, double Value, QWidget *Parent = NULL );
      
      QVariant getVariantValue() const;
      void setVariantValue( const QVariant &Value );

      double minimum() const;
      double maximum() const;

      void setRange( double Min, double Max );
      void setPrintfFormat( const QString &Format ) { PrintfFormat = Format; }

      void setValue( double Value );
      double value() const;
  };
  
  // ======================================================

  class labelPathEditWidget : public labelEditWidget
  {
    Q_OBJECT

    private:
      QFileDialog::FileMode FileMode;
      QFileDialog::AcceptMode AcceptMode;
      QString Directory, Filter;

    protected:
      virtual QLineEdit* getLineEdit();
      QWidget* createInputWidget(); 
      void setDefaultModes();
      void initWidget( const QString &LabelText );

    public:
      explicit labelPathEditWidget( const QString& LabelText, QWidget *Parent = NULL );
      labelPathEditWidget( const QString& LabelText, const QString& Text, QWidget *Parent = NULL );

      void setFileMode( QFileDialog::FileMode Mode ) { FileMode = Mode; }
      void setAcceptMode( QFileDialog::AcceptMode Mode ) { AcceptMode = Mode; }
      void setNameFilter( const QString &F ) { Filter = F; }
      void setNameFilters( const QStringList &F );
      void setDirectory( const QString &D ) { Directory = D; }

    public slots:
      void setEditFromFileDialog();
  };
  
  // ======================================================

  class labelSpinWidget : public labelInputWidget
  {
    Q_OBJECT
    
    protected:
      const QSpinBox* getSpinBox() const;
      QSpinBox* getSpinBox();
      QWidget* createInputWidget(); 

    public:
      explicit labelSpinWidget( const QString &LabelText, QWidget *Parent = NULL );
      labelSpinWidget( const QString &LabelText, int Value, QWidget *Parent = NULL );
      labelSpinWidget( const QString &LabelText, int Min, int Max, QWidget *Parent = NULL );

      QVariant getVariantValue() const;
      void setVariantValue( const QVariant &Value );

      int value() const;
      void setValue( int Value );
      void setReadOnly( bool ReadOnly );

      labelSpinWidget& setRange( int Min, int Max );
      labelSpinWidget& setMinimum( int Min );
      labelSpinWidget& setMaximum( int Max );

      int maximum() const;
      int minimum() const;

    signals:
      void valueChanged( int i );
  };

  // ======================================================

  class labelDoubleSpinWidget : public labelInputWidget
  {
    Q_OBJECT
    
    protected:
      const QDoubleSpinBox* getDoubleSpinBox() const;
      QDoubleSpinBox* getDoubleSpinBox();
      QWidget* createInputWidget(); 

    public:
      explicit labelDoubleSpinWidget( const QString& LabelText, QWidget *Parent = NULL );
      labelDoubleSpinWidget( const QString& LabelText, double Value, QWidget *Parent = NULL );
      labelDoubleSpinWidget( const QString& LabelText, double Min, double Max, QWidget *Parent = NULL );
      labelDoubleSpinWidget( const QString& LabelText, double Value, double Min, double Max, QWidget *Parent = NULL );

      QVariant getVariantValue() const;
      void setVariantValue( const QVariant &Value );

      double value() const;
      void setValue( double Value );
      void setReadOnly( bool ReadOnly );

      labelDoubleSpinWidget& setRange( double Min, double Max );
      labelDoubleSpinWidget& setMinimum( double Min );
      labelDoubleSpinWidget& setMaximum( double Max );
      labelDoubleSpinWidget& setDecimals( int Decimals );

      double minimum() const;
      double maximum() const;
      int decimals() const;

      void setSingleStep( double Step );
      double singleStep() const;
      
    signals:
      void valueChanged( double d );
  };

  // ======================================================
  
  class labelComboWidget : public labelInputWidget
  {
    Q_OBJECT

    protected:
      const QComboBox* getComboBox() const;
      QComboBox* getComboBox();
      QWidget* createInputWidget(); 
      
      QVariant valueToSettings() const;
      void setValueFromSettings( const QVariant &Value );
      
    public:
      explicit labelComboWidget( const QString& LabelText, QWidget *Parent = NULL );
      labelComboWidget( const QString& LabelText, const QStringList& Items, QWidget *Parent = NULL );

      QVariant getVariantValue() const { return currentData(); }
      void setVariantValue( const QVariant &Value ) { setCurrentData( Value ); }    

      int  currentIndex() const;
      int  count() const;
      QString currentText() const;
      QVariant currentData() const;
      labelComboWidget& addItem( const QString &Text, const QVariant &UserData = QVariant() );
      labelComboWidget& addItems( const QStringList& List );
      void clear();
      labelComboWidget& setCurrentIndex( int Index );
      labelComboWidget& setCurrentData( const QVariant& V );
      
    signals:
      void currentIndexChanged( int Index );
      void currentIndexChanged( const QString& );
  };

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

      void setOrientation( Qt::Orientation Orientation );

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
      void setText( const QString &String );
      
      void setReadOnly( bool ReadOnly );
  };

  // ======================================================
  
  template <class layoutType> layoutType* inputWidget::createLayoutWithoutMargins()
  {
    layoutType *Layout = new layoutType();
    Layout->setContentsMargins( 0, 0, 0, 0 );
    return Layout;
  }
      
  template <class input> input* inputWidget::create( QWidget *Parent, const QString &Name, const QString &Label, const QVariant &Value )
  {
    input *Input = new input( Label, Parent );
    Input->setVariantValue( Value );
    Input->setSettingsName( Name );
    Input->setSettingsParent( dynamic_cast<multiSettingsObject*>(Parent) );
    return Input;
  }

  template <class input> input* inputWidget::create( const QString &Name, const QString &Label, const QVariant &Value )
  {
    return create<input>( NULL, Name, Label, Value );
  }
  
  // ======================================================

}

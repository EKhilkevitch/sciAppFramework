
#pragma once

// ======================================================

#include <QWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QLabel>
#include <QFileDialog>
#include <QRadioButton>
#include <QCheckBox>
#include <QVariant>
#include <QList>

#include "sciAppFramework/settingsObject.h"

class QSettings;
class QGroupBox;
class QValidator;

// ======================================================

namespace sciAppFramework
{

// ======================================================

  class inputWidget : public QWidget, public singleSettingsObject
  {
    Q_OBJECT

    protected:
      QVariant valueToSettings() const { return getVariantValue(); }
      void setValueFromSettings( const QVariant& Value ) { setVariantValue( Value ); }

    public:
      inputWidget( QWidget *Parent, const QString &SettingsName = QString() );
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
      void setEnabled( int E ) { QWidget::setEnabled( E != 0 ); }
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
      int spacingWidgth() const { return 5; }

    public:
      labelInputWidget( QWidget *Parent = NULL, const QString &SettingsName = QString() ) : 
        inputWidget(Parent,SettingsName), 
        LabelWidget(NULL), 
        InputWidget(NULL) {}

      void setInputStyleSheet( const QString &Style ) { InputWidget->setStyleSheet(Style); }
      void setLabelStyleSheet( const QString &Style ) { LabelWidget->setStyleSheet(Style); }

      const QString label() const { return LabelWidget->text(); }
      void setStretchFactors( int LabelStretch, int InputStretch );
  };

  // ======================================================

  class labelEditWidget : public labelInputWidget
  {
    Q_OBJECT

    protected:
      virtual QLineEdit* getLineEdit() { return getInput<QLineEdit>(); }
      const QLineEdit* getLineEdit() const { return ( const_cast< labelEditWidget* >( this ) )->getLineEdit(); }

      QWidget* createInputWidget(); 

    protected:
      labelEditWidget( int, QWidget *Parent = NULL, const QString &SettingsName = QString() ) : 
        labelInputWidget(Parent,SettingsName) {} // Without initWidget()

    public:
      labelEditWidget( const QString& LabelText, QWidget *Parent = NULL ) 
        : labelInputWidget(Parent,LabelText) { initWidget(LabelText); }

      labelEditWidget( const QString& LabelText, QValidator *Validator, QWidget *Parent = NULL )  
        : labelInputWidget(Parent,LabelText) { initWidget(LabelText); setValidator(Validator); }
      
      labelEditWidget( const QString& LabelText, const QString& Text, QWidget *Parent )
        : labelInputWidget(Parent,LabelText) { initWidget(LabelText); setText(Text); }

      labelEditWidget( const QString& LabelText, const QString& Text, QValidator *Validator = NULL, 
        QWidget *Parent = NULL )
        : labelInputWidget(Parent,LabelText) { initWidget(LabelText); setValidator(Validator); setText(Text); }

      QVariant getVariantValue() const { return text(); }
      void setVariantValue( const QVariant &Value ) { setText( Value.toString() ); }

      QString text() const { return getLineEdit()->text(); }
      void setText( const QString& String ) { getLineEdit()->setText(String); }
      void setValidator( QValidator *Validator );
      
      void setReadOnly( bool E ) { getLineEdit()->setReadOnly(E); }

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

    protected:
      QWidget* createInputWidget(); 

    public:
      labelDoubleEditWidget( const QString& LabelText, QWidget *Parent = NULL ) : 
        labelEditWidget(0,Parent,LabelText) { initWidget(LabelText); }

      labelDoubleEditWidget( const QString& LabelText, double Value, QWidget *Parent = NULL ) :
        labelEditWidget(0,Parent,LabelText) { initWidget(LabelText); setValue(Value); }
      
      QVariant getVariantValue() const { return value(); }
      void setVariantValue( const QVariant &Value ) { setValue( Value.toDouble() ); }

      void setRange( double Min, double Max );
      void setPrintfFormat( const QString &Format ) { PrintfFormat = Format; }

      void setValue( double Value );
      double value() const { return text().toDouble(); }
  };
  
  // ======================================================

  class labelPathEditWidget : public labelEditWidget
  {
    Q_OBJECT

    private:
      enum QFileDialog::FileMode FileMode;
      enum QFileDialog::AcceptMode AcceptMode;

    protected:
      virtual QLineEdit* getLineEdit();
      QWidget* createInputWidget(); 
      void setDefaultModes() { FileMode = QFileDialog::AnyFile; AcceptMode = QFileDialog::AcceptOpen; }
      void initWidget( const QString &LabelText );

    public:
      labelPathEditWidget( const QString& LabelText, QWidget *Parent = NULL )
        : labelEditWidget(0,Parent,LabelText) { initWidget(LabelText); }
      labelPathEditWidget( const QString& LabelText, const QString& Text, QWidget *Parent = NULL )
        : labelEditWidget(0,Parent,LabelText) { initWidget(LabelText); setText(Text); }

      void setFileMode( QFileDialog::FileMode Mode ) { FileMode = Mode; }
      void setAcceptMode( QFileDialog::AcceptMode Mode ) { AcceptMode = Mode; }

    public slots:
      void setEditFromFileDialog();
  };

  // ======================================================

  class labelSpinWidget : public labelInputWidget
  {
    Q_OBJECT
    
    protected:
      const QSpinBox* getSpinBox() const { return getInput<QSpinBox>(); }
      QSpinBox* getSpinBox() { return getInput<QSpinBox>(); }
      QWidget* createInputWidget(); 

    public:
      labelSpinWidget( const QString& LabelText, QWidget *Parent = NULL ) 
        : labelInputWidget(Parent,LabelText) { initWidget(LabelText); }
      
      labelSpinWidget( const QString& LabelText, int Value, QWidget *Parent = NULL ) 
        : labelInputWidget(Parent,LabelText) { initWidget(LabelText); setValue(Value); }

      labelSpinWidget( const QString& LabelText, int Min, int Max, QWidget *Parent = NULL )  
        : labelInputWidget(Parent,LabelText) { initWidget(LabelText); setRange(Min,Max); }

      QVariant getVariantValue() const { return value(); }
      void setVariantValue( const QVariant &Value ) { setValue( Value.toInt() ); }

      int value() const { return getSpinBox()->value(); }
      void setValue( int V ) { getSpinBox()->setValue(V); }
      void setReadOnly( bool E ) { getSpinBox()->setReadOnly(E); };

      void setRange( int Min, int Max ) { getSpinBox()->setRange(Min,Max); }
      int maximum() const { return getSpinBox()->maximum(); }
      int minimum() const { return getSpinBox()->minimum(); }

    signals:
      void valueChanged( int i );
  };

  // ======================================================

  class labelDoubleSpinWidget : public labelInputWidget
  {
    Q_OBJECT
    
    protected:
      const QDoubleSpinBox* getDoubleSpinBox() const { return getInput<QDoubleSpinBox>(); }
      QDoubleSpinBox* getDoubleSpinBox() { return getInput<QDoubleSpinBox>(); }
      QWidget* createInputWidget(); 

    public:
      labelDoubleSpinWidget( const QString& LabelText, QWidget *Parent = NULL ) 
        : labelInputWidget(Parent,LabelText) { initWidget(LabelText); }
      
      labelDoubleSpinWidget( const QString& LabelText, double Value, QWidget *Parent = NULL )  
        : labelInputWidget(Parent,LabelText) { initWidget(LabelText); setValue(Value); }

      labelDoubleSpinWidget( const QString& LabelText, double Min, double Max, QWidget *Parent = NULL )  
        : labelInputWidget(Parent,LabelText) { initWidget(LabelText); setRange(Min,Max); }
      
      labelDoubleSpinWidget( const QString& LabelText, double Value, double Min, double Max, QWidget *Parent = NULL )  
        : labelInputWidget(Parent,LabelText) { initWidget(LabelText); setValue(Value); setRange(Min,Max); }

      QVariant getVariantValue() const { return value(); }
      void setVariantValue( const QVariant &Value ) { setValue( Value.toDouble() ); }    

      double value() const { return getDoubleSpinBox()->value(); }
      void setValue( double V ) { getDoubleSpinBox()->setValue(V); }
      void setReadOnly( bool E ) { getDoubleSpinBox()->setReadOnly(E); };

      void setRange( double Min, double Max ) { getDoubleSpinBox()->setRange(Min,Max); }
      double minimum() const { return getDoubleSpinBox()->minimum(); }
      double maximum() const { return getDoubleSpinBox()->maximum(); }

      void setDecimals( int D ) { getDoubleSpinBox()->setDecimals(D); }
      int decimals() const { return getDoubleSpinBox()->decimals(); }

      void setSingleStep( double S ) { getDoubleSpinBox()->setSingleStep(S); }
      double singleStep() const { return getDoubleSpinBox()->singleStep(); }
      
    signals:
      void valueChanged( double d );
  };

  // ======================================================

  class labelComboWidget : public labelInputWidget
  {
    Q_OBJECT

    protected:
      const QComboBox* getComboBox() const { return getInput<QComboBox>(); }
      QComboBox* getComboBox() { return getInput<QComboBox>(); }
      QWidget* createInputWidget(); 
      
      QVariant valueToSettings() const { return currentIndex(); }
      void setValueFromSettings( const QVariant &Value ) { setCurrentIndex( qMax( 0, Value.toInt() ) ); }
      
    public:
      labelComboWidget( const QString& LabelText, QWidget *Parent = NULL ) 
        : labelInputWidget(Parent,LabelText) { initWidget(LabelText); }

      labelComboWidget( const QString& LabelText, const QStringList& Items, QWidget *Parent = NULL )  
        : labelInputWidget(Parent,LabelText) { initWidget(LabelText); addItems(Items); }

      QVariant getVariantValue() const { return currentData(); }
      void setVariantValue( const QVariant &Value ) { setCurrentData( Value ); }    

      int  currentIndex() const { return getComboBox()->currentIndex(); }
      int  count() const { return getComboBox()->count(); }
      QString currentText() const { return getComboBox()->currentText(); }
      QVariant currentData() const { return getComboBox()->itemData( currentIndex() ); }
      void addItem( const QString &Text, const QVariant &UserData = QVariant() ) { getComboBox()->addItem(Text,UserData); }
      void addItems( const QStringList& List );
      void clear() { getComboBox()->clear(); }
      void setCurrentIndex( int Index ) { getComboBox()->setCurrentIndex(Index); }
      void setCurrentData( const QVariant& V );
      
    signals:
      void currentIndexChanged( int i );
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
        radioButtonPair() : 
          Button(NULL) {}
        radioButtonPair( QRadioButton *B, const QVariant &D ) : 
          Button(B), Data(D) {}
      };

    private:
      QList< radioButtonPair > RadioButtons;
      QGroupBox *ButtonsBox;

    protected:
      QVariant valueToSettings() const { return currentIndex(); }
      void setValueFromSettings( const QVariant &Value ) { setCurrentIndex( Value.toInt() ); }

      void initWidget( const QString &LabelText );

    private slots:
      void oneOfButtonsChecked();

    public:
      radioButtonWidget( const QString& LabelText, QWidget *Parent = NULL ) 
        : inputWidget( Parent, LabelText ) { initWidget( LabelText ); }
      
      QVariant getVariantValue() const { return currentData(); }
      void setVariantValue( const QVariant &Value ) { setCurrentData( Value ); }

      void setOrientation( Qt::Orientation O );

      const QString label() const;

      int currentIndex() const;
      int count() const { return RadioButtons.size(); }
      QVariant currentData() const { return RadioButtons.value( currentIndex() ).Data; }
      void addItem( const QString &Text, const QVariant &UserData = QVariant() );
      void clear();
      void setCurrentIndex( int Index );
      void setCurrentData( const QVariant& V );

    signals:
      void currentIndexChanged( int i );
  };
  
  // ======================================================
 
  class checkBoxWidget : public inputWidget
  {
    Q_OBJECT

    private:
      QCheckBox *CheckBox;
    
    protected:
      QVariant valueToSettings() const { return isChecked(); }
      void setValueFromSettings( const QVariant &Value ) { setChecked( Value.toBool() ); }

      void initWidget( const QString &LabelText );

    private slots:
      void emitCheckedSignal();

    public:
      checkBoxWidget( const QString& LabelText, QWidget *Parent = NULL ) 
        : inputWidget( Parent, LabelText ) { initWidget(LabelText); }
      
      checkBoxWidget( const QString& LabelText, bool Value, QWidget *Parent = NULL ) 
        : inputWidget( Parent, LabelText ) { initWidget(LabelText); setChecked(Value); }
      
      QVariant getVariantValue() const { return isChecked(); }
      void setVariantValue( const QVariant &Value ) { setChecked( Value.toBool() ); }

      const QString label() const { return CheckBox->text(); }

      Qt::CheckState checkState() const { return CheckBox->checkState(); }
      void setTristate( bool Set = true ) { CheckBox->setTristate(Set); }
      bool isTristate() const { return CheckBox->isTristate(); }
      bool isChecked() const { return CheckBox->isChecked(); }
      void setChecked( bool C );

    public slots:
      void setCheckedOn() { setCheckState(Qt::Checked); }
      void setCheckedOff() { setCheckState(Qt::Unchecked); }
      void setCheckedPartial() { setCheckState(Qt::PartiallyChecked); }
      void setCheckState( Qt::CheckState State ) { CheckBox->setCheckState(State); }
      void setCheckState( int State ) { setCheckState( static_cast<Qt::CheckState>(State) ); }
    
    signals:
      void clicked();
      void stateChanged( int );
      void toggled( bool );
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

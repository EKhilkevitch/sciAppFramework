
#pragma once

// ======================================================

#include "sciAppFramework/inputWidget.h"

#include <QFileDialog>

class QLabel;
class QLineEdit;
class QSpinBox;
class QDoubleSpinBox;
class QComboBox;
class QSettings;
class QValidator;
class QDoubleValidator;

// ======================================================

namespace sciAppFramework
{

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
      labelEditWidget& setText( const QString& String );
      labelEditWidget& setValidator( QValidator *Validator );
      
      labelEditWidget& setReadOnly( bool ReadOnly );

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

      labelDoubleEditWidget& setRange( double Min, double Max );
      labelDoubleEditWidget& setMinimum( double Min );
      labelDoubleEditWidget& setMaximum( double Max );
      labelDoubleEditWidget& setPrintfFormat( const QString &Format );

      labelDoubleEditWidget& setValue( double Value );
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

      labelPathEditWidget& setFileMode( QFileDialog::FileMode Mode );
      labelPathEditWidget& setAcceptMode( QFileDialog::AcceptMode Mode );
      labelPathEditWidget& setNameFilter( const QString &F );
      labelPathEditWidget& setNameFilters( const QStringList &F );
      labelPathEditWidget& setDirectory( const QString &D );

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

      labelSpinWidget& setValue( int Value );
      labelSpinWidget& setReadOnly( bool ReadOnly );

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
      labelDoubleSpinWidget& setValue( double Value );
      labelDoubleSpinWidget& setReadOnly( bool ReadOnly );

      labelDoubleSpinWidget& setRange( double Min, double Max );
      labelDoubleSpinWidget& setMinimum( double Min );
      labelDoubleSpinWidget& setMaximum( double Max );
      labelDoubleSpinWidget& setDecimals( int Decimals );

      double minimum() const;
      double maximum() const;
      int decimals() const;

      labelDoubleSpinWidget& setSingleStep( double Step );
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
      labelComboWidget& addItem( const QString &Text );
      labelComboWidget& addItem( const QString &Text, const QVariant &UserData );
      labelComboWidget& addItems( const QStringList& List );
      void clear();
      labelComboWidget& setCurrentIndex( int Index );
      labelComboWidget& setCurrentData( const QVariant& V );
      
    signals:
      void currentIndexChanged( int Index );
      void currentIndexChanged( const QString& );
  };

  // ======================================================
  
}


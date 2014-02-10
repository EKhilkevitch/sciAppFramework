
// ======================================================

#include "sciAppFramework/inputWidget.h"
#include "sciAppFramework/utility.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedLayout>
#include <QFileDialog>
#include <QFileInfo>
#include <QGroupBox>
#include <QDoubleValidator>
#include <QValidator>
#include <QDebug>

#include <limits>

using namespace sciAppFramework;

// ======================================================

inputWidget::inputWidget( QWidget *Parent, const QString &SettingsName ) : 
  QWidget(Parent),
  singleSettingsObject( dynamic_cast<settingsObject*>(Parent), settingsObject::normolizeToSettingsName(SettingsName) ) 
{
}

// ======================================================

void labelInputWidget::initWidget( const QString& LabelText )
{
  Q_ASSERT( LabelWidget == NULL );
  Q_ASSERT( InputWidget == NULL );
  
  LabelWidget = createLabelWidget( LabelText );
  InputWidget = createInputWidget();
  
  //qDebug() << "labelInput::initWidget: label = " << LabelText << ", parent = " << parent() << " this = " << this;
  //qDebug() << "labelInput::initWidget: members = " << LabelWidget << InputWidget;

  QBoxLayout *Layout = createLayoutWithoutMargins<QHBoxLayout>();
  Layout->addSpacing( spacingWidgth() );
  Layout->addWidget( LabelWidget );
  Layout->addSpacing( spacingWidgth() );
  Layout->addWidget( InputWidget );
  Layout->addSpacing( spacingWidgth() );
  setLayout( Layout );

  setStretchFactors( 7, 2 );
}

// ------------------------------------------------------

QLabel* labelInputWidget::createLabelWidget( const QString &LabelText )
{
  QLabel *Label = new QLabel( LabelText, this );
  return Label;
}

// ------------------------------------------------------

void labelInputWidget::setStretchFactors( int LabelStretch, int InputStretch )
{
  QBoxLayout *Layout = dynamic_cast<QBoxLayout*>( layout() );
  if ( Layout != NULL )
  {
    Layout->setStretchFactor( LabelWidget, LabelStretch );
    Layout->setStretchFactor( InputWidget, InputStretch );
  }
}

// ======================================================
    
QWidget* labelEditWidget::createInputWidget()
{
  QLineEdit *Edit = new QLineEdit(this);
  
  connect( Edit, SIGNAL(returnPressed()), SIGNAL(returnPressed()));
  connect( Edit, SIGNAL(editingFinished()), SIGNAL(editingFinished()));
  
  connect( Edit, SIGNAL(cursorPositionChanged(int,int)), SIGNAL(cursorPositionChanged(int,int)));
  connect( Edit, SIGNAL(selectionChanged()), SIGNAL(selectionChanged()));
  connect( Edit, SIGNAL(textChanged(const QString&)), SIGNAL(textChanged(const QString&)));
  connect( Edit, SIGNAL(textEdited(const QString&)), SIGNAL(textEdited(const QString&)));

  connect( Edit, SIGNAL(returnPressed()), SIGNAL(changed()) );  
  connect( Edit, SIGNAL(editingFinished()), SIGNAL(changed()) );  
  
  return Edit;
}

// ------------------------------------------------------
      
void labelEditWidget::setValidator( QValidator *Validator ) 
{ 
  if ( Validator != NULL ) 
    getLineEdit()->setValidator(Validator); 
}

// ======================================================

QWidget* labelDoubleEditWidget::createInputWidget()
{
  QWidget *Input = labelEditWidget::createInputWidget();
  QLineEdit *Edit = dynamic_cast<QLineEdit*>( Input );

  Q_ASSERT( Edit != NULL );

  QValidator *Validator = new QDoubleValidator(this);
  Edit->setValidator( Validator );

  return Edit;
}

// ------------------------------------------------------
      
void labelDoubleEditWidget::setRange( double Min, double Max )
{
  const QValidator *Validator = getLineEdit()->validator();
  if ( Validator == NULL )
  {
    qDebug() << "labelDoubleEditWidget::setRange: Validator == NULL!";
    return;
  }

  QDoubleValidator *DoubleValidator = const_cast<QDoubleValidator*>( dynamic_cast<const QDoubleValidator*>(Validator) );
  if ( DoubleValidator == NULL )
  {
    qDebug() << "labelDoubleEditWidget::setRange: DoubleValidator == NULL!";
    return;
  }

  DoubleValidator->setRange( Min, Max );
}
      
// ------------------------------------------------------

void labelDoubleEditWidget::setValue( double Value ) 
{ 
  if ( PrintfFormat.isEmpty() )
    setText( QString::number(Value) );
  else
    setText( QString().sprintf( PrintfFormat.toLocal8Bit().data(), Value ) );
}

// ======================================================

QLineEdit* labelPathEditWidget::getLineEdit()
{
  QWidget *Widget = getInput<QWidget>();
  QList< QLineEdit* > ListOfLineEdits = Widget->findChildren<QLineEdit*>();
  return ListOfLineEdits.value( 0, NULL );
}

// ------------------------------------------------------

QWidget* labelPathEditWidget::createInputWidget()
{
  QPushButton *Button = new QPushButton("...",this);
  Button->setMaximumHeight( 20 );
  Button->setMaximumWidth( 25 );
  connect( Button, SIGNAL(clicked()), SLOT(setEditFromFileDialog()) );

  QBoxLayout *Layout = createLayoutWithoutMargins<QHBoxLayout>(); 
  Layout->addWidget( labelEditWidget::createInputWidget() );
  Layout->addSpacing( spacingWidgth() );
  Layout->addWidget( Button );

  QWidget *Widget = new QWidget(this);
  Widget->setLayout( Layout );
  return Widget;
}

// ------------------------------------------------------
    
void labelPathEditWidget::initWidget( const QString &LabelText )
{
  setDefaultModes(); 
  labelEditWidget::initWidget(LabelText);
  setStretchFactors( 6, 8 );
}

// ------------------------------------------------------

void labelPathEditWidget::setEditFromFileDialog()
{
  //qDebug() << "File: " << text() << " path = " << QFileInfo(text()).path();

  QString CurrDir = QFileInfo( text().length() > 0 ? text() : "." ).path();

  QFileDialog Dialog( this );
  Dialog.selectFile( text() );
  Dialog.setFileMode( FileMode );
  Dialog.setAcceptMode( AcceptMode );
  Dialog.selectFile( text() );
  
  if ( Dialog.exec() == QDialog::Accepted )
  {
    setText( Dialog.selectedFiles().value(0,"") );
    emit changed();
  }
}

// ======================================================

QWidget* labelSpinWidget::createInputWidget()
{
  QSpinBox *Spin = new QSpinBox(this);
  Spin->setRange( std::numeric_limits<int>::min(), std::numeric_limits<int>::max() );
  connect( Spin, SIGNAL(valueChanged(int)), SIGNAL(valueChanged(int)));
  connect( Spin, SIGNAL(valueChanged(int)), SIGNAL(changed()) );
  return Spin;
}

// ======================================================

QWidget* labelDoubleSpinWidget::createInputWidget()
{
  QDoubleSpinBox *Spin = new QDoubleSpinBox(this);
  Spin->setRange( -std::numeric_limits<double>::max(), +std::numeric_limits<double>::max() );
  Spin->setDecimals( 3 );
  connect( Spin,  SIGNAL(valueChanged(double)), SIGNAL(valueChanged(double))); 
  connect( Spin,  SIGNAL(valueChanged(double)), SIGNAL(changed()) ); 
  return Spin;
}

// ======================================================

QWidget* labelComboWidget::createInputWidget()
{
  QComboBox *Combo = new QComboBox(this);
  Combo->setSizeAdjustPolicy( QComboBox::AdjustToContents );
  connect( Combo, SIGNAL(currentIndexChanged(int)), SIGNAL(currentIndexChanged(int)));
  connect( Combo, SIGNAL(currentIndexChanged(const QString&)), SIGNAL(currentIndexChanged(const QString&)));
  connect( Combo, SIGNAL(currentIndexChanged(int)), SIGNAL(changed()));
  return Combo;
}

// ------------------------------------------------------

void labelComboWidget::addItems( const QStringList& List ) 
{ 
  foreach( QString String, List ) 
    addItem( String ); 
}

// ------------------------------------------------------
      
void labelComboWidget::setCurrentData( const QVariant& V ) 
{ 
  for ( int i = 0; i < count(); i++ ) 
  { 
    if (getComboBox()->itemData(i) == V ) 
      setCurrentIndex(i); 
  } 
}

// ======================================================

void radioButtonWidget::initWidget( const QString &LabelText )
{
  ButtonsBox = new QGroupBox(LabelText,this);
  ButtonsBox->setLayout( new QVBoxLayout() );

  QBoxLayout *Layout = createLayoutWithoutMargins<QVBoxLayout>();
  Layout->addWidget( ButtonsBox );
  setLayout( Layout );
}

// ------------------------------------------------------
      
void radioButtonWidget::setOrientation( Qt::Orientation O )
{
  QBoxLayout *Layout = NULL;
  switch ( O )
  {
    case Qt::Vertical:
      Layout = createLayoutWithoutMargins<QVBoxLayout>();
      break;

    case Qt::Horizontal:
      Layout = createLayoutWithoutMargins<QHBoxLayout>();
      break;
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
}

// ------------------------------------------------------

const QString radioButtonWidget::label() const
{
  return ButtonsBox->title();
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

void radioButtonWidget::addItem( const QString &Text, const QVariant &UserData )
{
  QRadioButton *Button = new QRadioButton( Text, this );
  ButtonsBox->layout()->addWidget( Button );
  RadioButtons.append( radioButtonPair(Button,UserData) );
  connect( Button, SIGNAL(clicked()), SLOT(oneOfButtonsChecked()) );
  connect( Button, SIGNAL(clicked()), SIGNAL(changed()) );
  if ( RadioButtons.size() == 1 )
    Button->setChecked(true);
}

// ------------------------------------------------------
      
void radioButtonWidget::clear()
{
  foreach ( radioButtonPair P, RadioButtons )
    delete P.Button;
  RadioButtons.clear();
}

// ------------------------------------------------------

void radioButtonWidget::setCurrentIndex( int Index )
{
  if ( Index < RadioButtons.size() && Index >= 0 )
    RadioButtons[Index].Button->setChecked(true);
}

// ------------------------------------------------------

void radioButtonWidget::setCurrentData( const QVariant& V )
{
  for ( int i = 0; i < RadioButtons.size(); i++ )
  {
    if ( RadioButtons[i].Data == V )
    {
      setCurrentIndex(i);
      break;
    }
  }
}

// ------------------------------------------------------

void radioButtonWidget::oneOfButtonsChecked()
{
  emit currentIndexChanged( currentIndex() );
}

// ======================================================

void checkBoxWidget::initWidget( const QString &LabelText )
{
  CheckBox = new QCheckBox( LabelText, this );

  QBoxLayout *Layout = createLayoutWithoutMargins<QVBoxLayout>();
  Layout->addWidget( CheckBox );
  setLayout( Layout );

  connect( CheckBox, SIGNAL(stateChanged(int)), SIGNAL(stateChanged(int)) );
  connect( CheckBox, SIGNAL(clicked()), SIGNAL(clicked()) );
  connect( CheckBox, SIGNAL(clicked()), SLOT(emitCheckedSignal()) );
  connect( CheckBox, SIGNAL(toggled(bool)), SLOT(emitCheckedSignal()) );
}

// ------------------------------------------------------

void checkBoxWidget::setChecked( bool C ) 
{ 
  CheckBox->setChecked(C); 
  emitCheckedSignal(); 
} 

// ------------------------------------------------------
      
void checkBoxWidget::emitCheckedSignal()
{
  emit toggled( isChecked() );
  emit changed();
}

// ======================================================


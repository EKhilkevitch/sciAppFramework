
// ======================================================

#include "sciAppFramework/inputWidget.h"
#include "sciAppFramework/utility.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QStackedLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QFileInfo>
#include <QSettings>
#include <QGroupBox>
#include <QDebug>

#include <limits>

using namespace sciAppFramework;

// ======================================================

const QString inputWidget::name() const 
{ 
  QString Name = objectName();
  if ( ! Name.isEmpty() )
    return Name;
  
  Name = label();
  Name.replace( QRegExp("[\\s:+*\\-]"), "_" );
  return Name;
}

// ------------------------------------------------------
    
void inputWidget::saveSettings( QSettings *Settings )
{
  if ( Settings == NULL )
    return;
  Settings->setValue( name(), getValueToSettings() );
}

// ------------------------------------------------------

void inputWidget::loadSettings( QSettings *Settings )
{
  if ( Settings == NULL )
    return;

  QVariant Value = Settings->value( name() );
  //qDebug() << "labelInput::loadSettings" << name() << Value;
  if ( Value.isValid() )
    setValueFromSettings( Value );
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

QLineEdit* labelEditPathWidget::getLineEdit()
{
  QWidget *Widget = getInput<QWidget>();
  QList< QLineEdit* > ListOfLineEdits = Widget->findChildren<QLineEdit*>();
  return ListOfLineEdits.value( 0, NULL );
}

// ------------------------------------------------------

QWidget* labelEditPathWidget::createInputWidget()
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
    
void labelEditPathWidget::initWidget( const QString &LabelText )
{
  setDefaultModes(); 
  labelEditWidget::initWidget(LabelText);
  setStretchFactors( 6, 8 );
}

// ------------------------------------------------------

void labelEditPathWidget::setEditFromFileDialog()
{
  qDebug() << "File: " << text() << " path = " << QFileInfo(text()).path();

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

// ======================================================

void radioButtonWidget::initWidget( const QString &LabelText )
{
  ButtonsLayout = new QVBoxLayout();
  
  QGroupBox *Box = new QGroupBox(LabelText,this);
  Box->setLayout( ButtonsLayout );

  QBoxLayout *Layout = createLayoutWithoutMargins<QVBoxLayout>();
  Layout->addWidget( Box );
  setLayout( Layout );
}

// ------------------------------------------------------

const QString radioButtonWidget::label() const
{
  QList<QGroupBox*> GroupBoxes = findChildren<QGroupBox*>();
  if ( GroupBoxes.isEmpty() )
    return QString();
  return GroupBoxes.first()->title();
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
  ButtonsLayout->addWidget( Button );
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
  if ( Index < RadioButtons.size() )
    RadioButtons[Index].Button->setChecked(true);
}

// ------------------------------------------------------

void radioButtonWidget::setCurrentData( const QVariant& V )
{
  for ( int i = 0; i < count(); i++ )
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

  connect( CheckBox, SIGNAL(clicked()), SIGNAL(clicked()) );
  connect( CheckBox, SIGNAL(stateChanged(int)), SIGNAL(stateChanged(int)) );
  connect( CheckBox, SIGNAL(clicked()), SIGNAL(changed()) );
}

// ======================================================


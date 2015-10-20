
// ======================================================

#include "sciAppFramework/inputWidget.h"

#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QRadioButton>
#include <QCheckBox>
#include <QPushButton>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedLayout>
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

// ------------------------------------------------------
      
void inputWidget::setEnabled( int Enabled ) 
{ 
  QWidget::setEnabled( Enabled != 0 ); 
}

// ------------------------------------------------------
      
QVariant inputWidget::valueToSettings() const 
{ 
  return getVariantValue(); 
}

// ------------------------------------------------------

void inputWidget::setValueFromSettings( const QVariant& Value ) 
{ 
  setVariantValue( Value ); 
}

// ======================================================

labelInputWidget::labelInputWidget( QWidget *Parent, const QString &SettingsName ) : 
  inputWidget(Parent,SettingsName), 
  LabelWidget(NULL), 
  InputWidget(NULL) 
{
}

// ------------------------------------------------------

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

int labelInputWidget::spacingWidgth() const 
{ 
  return 5; 
}

// ------------------------------------------------------

QLabel* labelInputWidget::createLabelWidget( const QString &LabelText )
{
  QLabel *Label = new QLabel( LabelText, this );
  return Label;
}

// ------------------------------------------------------
      
const QString labelInputWidget::label() const 
{ 
  return LabelWidget->text(); 
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

// ------------------------------------------------------
      
void labelInputWidget::setInputStyleSheet( const QString &Style ) 
{ 
  InputWidget->setStyleSheet(Style); 
}

// ------------------------------------------------------
 
void labelInputWidget::setLabelStyleSheet( const QString &Style ) 
{ 
  LabelWidget->setStyleSheet(Style); 
}

// ======================================================
    
labelEditWidget::labelEditWidget( int, QWidget *Parent, const QString &SettingsName ) : 
  labelInputWidget(Parent,SettingsName) 
{
  // Without initWidget()
} 

// ------------------------------------------------------

labelEditWidget::labelEditWidget( const QString& LabelText, QWidget *Parent ) :
  labelInputWidget(Parent,LabelText) 
{ 
  initWidget(LabelText); 
}

// ------------------------------------------------------

labelEditWidget::labelEditWidget( const QString& LabelText, QValidator *Validator, QWidget *Parent ) :
  labelInputWidget(Parent,LabelText) 
{ 
  initWidget(LabelText); 
  setValidator(Validator); 
}
      
// ------------------------------------------------------
      
labelEditWidget::labelEditWidget( const QString& LabelText, const QString& Text, QWidget *Parent ) :
  labelInputWidget(Parent,LabelText) 
{ 
  initWidget(LabelText); 
  setText(Text); 
}

// ------------------------------------------------------
      
labelEditWidget::labelEditWidget( const QString& LabelText, const QString& Text, QValidator *Validator, QWidget *Parent  ) :
  labelInputWidget(Parent,LabelText) 
{ 
  initWidget(LabelText); 
  setValidator(Validator); 
  setText(Text); 
}

// ------------------------------------------------------

QVariant labelEditWidget::getVariantValue() const 
{ 
  return text(); 
}

// ------------------------------------------------------

void labelEditWidget::setVariantValue( const QVariant &Value ) 
{ 
  setText( Value.toString() ); 
}

// ------------------------------------------------------

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

QLineEdit* labelEditWidget::getLineEdit() 
{ 
  QLineEdit* Edit = getInput<QLineEdit>(); 
  Q_ASSERT( Edit != NULL );
  return Edit;
}

// ------------------------------------------------------

const QLineEdit* labelEditWidget::getLineEdit() const 
{ 
  return ( const_cast< labelEditWidget* >( this ) )->getLineEdit(); 
}

// ------------------------------------------------------
      
QString labelEditWidget::text() const 
{ 
  return getLineEdit()->text(); 
}

// ------------------------------------------------------

labelEditWidget& labelEditWidget::setText( const QString& String ) 
{ 
  getLineEdit()->setText(String); 
  return *this;
}

// ------------------------------------------------------

labelEditWidget& labelEditWidget::setReadOnly( bool ReadOnly ) 
{ 
  getLineEdit()->setReadOnly(ReadOnly); 
  return *this;
}

// ------------------------------------------------------
      
labelEditWidget& labelEditWidget::setValidator( QValidator *Validator ) 
{ 
  if ( Validator != NULL ) 
    getLineEdit()->setValidator(Validator); 
  return *this;
}

// ======================================================
      
labelDoubleEditWidget::labelDoubleEditWidget( const QString& LabelText, QWidget *Parent ) : 
  labelEditWidget(0,Parent,LabelText) 
{ 
  initWidget(LabelText); 
}

// ------------------------------------------------------

labelDoubleEditWidget::labelDoubleEditWidget( const QString& LabelText, double Value, QWidget *Parent ) :
  labelEditWidget(0,Parent,LabelText) 
{ 
  initWidget(LabelText); 
  setValue(Value); 
}

// ------------------------------------------------------

QDoubleValidator* labelDoubleEditWidget::doubleValidator()
{
  return const_cast<QDoubleValidator*>( const_cast<const labelDoubleEditWidget*>(this)->doubleValidator() );
}

// ------------------------------------------------------

const QDoubleValidator* labelDoubleEditWidget::doubleValidator() const
{
  const QValidator *Validator = getLineEdit()->validator();
  if ( Validator == NULL )
  {
    qDebug() << "labelDoubleEditWidget::setRange: Validator == NULL!";
    return NULL;
  }

  const QDoubleValidator *DoubleValidator = dynamic_cast<const QDoubleValidator*>(Validator);
  if ( DoubleValidator == NULL )
  {
    qDebug() << "labelDoubleEditWidget::setRange: DoubleValidator == NULL!";
    return NULL;
  }

  return DoubleValidator;
}

// ------------------------------------------------------

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

double labelDoubleEditWidget::minimum() const
{
  const QDoubleValidator *Validator = doubleValidator();
  if ( Validator == NULL )
    return std::numeric_limits<double>::min();
  return Validator->bottom();
}

// ------------------------------------------------------

double labelDoubleEditWidget::maximum() const
{
  const QDoubleValidator *Validator = doubleValidator();
  if ( Validator == NULL )
    return std::numeric_limits<double>::max();
  return Validator->top();
}

// ------------------------------------------------------
      
labelDoubleEditWidget& labelDoubleEditWidget::setRange( double Min, double Max )
{
  QDoubleValidator *Validator = doubleValidator();
  if ( Validator != NULL )
    Validator->setRange( Min, Max );
  return *this;
}
      
// ------------------------------------------------------

labelDoubleEditWidget& labelDoubleEditWidget::setMinimum( double Min )
{
  QDoubleValidator *Validator = doubleValidator();
  if ( Validator != NULL )
    Validator->setBottom( Min );
  return *this;
}

// ------------------------------------------------------

labelDoubleEditWidget& labelDoubleEditWidget::setMaximum( double Max )
{
  QDoubleValidator *Validator = doubleValidator();
  if ( Validator != NULL )
    Validator->setTop( Max );
  return *this;
}

// ------------------------------------------------------
      
labelDoubleEditWidget& labelDoubleEditWidget::setPrintfFormat( const QString &Format ) 
{ 
  PrintfFormat = Format;
  return *this;
}

// ------------------------------------------------------

labelDoubleEditWidget& labelDoubleEditWidget::setValue( double Value ) 
{ 
  if ( PrintfFormat.isEmpty() )
    setText( QString::number(Value) );
  else
    setText( QString().sprintf( PrintfFormat.toLocal8Bit().data(), Value ) );

  return *this;
}

// ------------------------------------------------------
      
double labelDoubleEditWidget::value() const 
{
  return text().toDouble(); 
}
      
// ------------------------------------------------------

QVariant labelDoubleEditWidget::getVariantValue() const 
{ 
  return value(); 
}

// ------------------------------------------------------

void labelDoubleEditWidget::setVariantValue( const QVariant &Value ) 
{ 
  setValue( Value.toDouble() ); 
}

// ======================================================
      
labelPathEditWidget::labelPathEditWidget( const QString& LabelText, QWidget *Parent )
  : labelEditWidget(0,Parent,LabelText) 
{ 
  initWidget(LabelText); 
}

// ------------------------------------------------------

labelPathEditWidget::labelPathEditWidget( const QString& LabelText, const QString& Text, QWidget *Parent )
  : labelEditWidget(0,Parent,LabelText) 
{ 
  initWidget(LabelText); 
  setText(Text); 
}

// ------------------------------------------------------

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

void labelPathEditWidget::setDefaultModes() 
{ 
  FileMode = QFileDialog::AnyFile; 
  AcceptMode = QFileDialog::AcceptOpen; 
}

// ------------------------------------------------------

void labelPathEditWidget::setNameFilters( const QStringList &F ) 
{ 
  Filter = F.join(";;"); 
}

// ------------------------------------------------------

void labelPathEditWidget::setEditFromFileDialog()
{
  //qDebug() << "File: " << text() << " path = " << QFileInfo(text()).path();

/* 
 * This code not working on Release build with icc on Win32 
 */
#if __unix__ || _MSC_VER >= 1700
  QString CurrDir = QFileInfo( text().length() > 0 ? text() : "." ).path();

  QFileDialog Dialog( this );
  Dialog.selectFile( text() );
  Dialog.setFileMode( FileMode );
  Dialog.setAcceptMode( AcceptMode );
  Dialog.setDirectory( Directory );
  Dialog.setNameFilter( Filter );
  Dialog.selectFile( text() );
  Dialog.setOption( QFileDialog::DontUseNativeDialog, false );
  
  if ( Dialog.exec() == QDialog::Accepted )
  {
    setText( Dialog.selectedFiles().value(0,"") );
    emit changed();
  }
#else
  QWidget *Parent = this;
  const QString Selected = text();
  const QString Caption = QString();
  const QString Directory = this->Directory;
  const QString Filter = this->Filter;
  QFileDialog::Options Options = 0;
  
  QString FileName;
  if ( FileMode == QFileDialog::DirectoryOnly )
  {
    Options |= QFileDialog::ShowDirsOnly;
    if ( AcceptMode == QFileDialog::AcceptOpen )
      FileName = QFileDialog::getExistingDirectory( Parent, Caption, Selected, Options );
    else 
      FileName = QFileDialog::getExistingDirectory( Parent, Caption, Selected, Options );
      //FileName = QFileDialog::getSaveFileName( Parent, Caption, Selected, Filter, NULL, Options );
  } else {
    if ( AcceptMode == QFileDialog::AcceptOpen )
      FileName = QFileDialog::getOpenFileName( Parent, Caption, Selected, Filter );
    else 
      FileName = QFileDialog::getSaveFileName( Parent, Caption, Selected, Filter );
  }

  if ( FileName.isEmpty() )
    return;
    
  setText( FileName );
  emit changed();
#endif

}

// ======================================================
      
labelSpinWidget::labelSpinWidget( const QString &LabelText, QWidget *Parent ) : 
  labelInputWidget(Parent,LabelText) 
{ 
  initWidget( LabelText ); 
}

// ------------------------------------------------------
      
labelSpinWidget::labelSpinWidget( const QString& LabelText, int Value, QWidget *Parent ) : 
  labelInputWidget(Parent,LabelText) 
{ 
  initWidget(LabelText); 
  setValue(Value); 
}

// ------------------------------------------------------

labelSpinWidget::labelSpinWidget( const QString& LabelText, int Min, int Max, QWidget *Parent ) : 
  labelInputWidget(Parent,LabelText) 
{ 
  initWidget(LabelText); 
  setRange(Min,Max); 
}

// ------------------------------------------------------
      
const QSpinBox* labelSpinWidget::getSpinBox() const 
{ 
  const QSpinBox *Spin = getInput<QSpinBox>();
  Q_ASSERT( Spin != NULL );
  return Spin;
}

// ------------------------------------------------------

QSpinBox* labelSpinWidget::getSpinBox() 
{ 
  QSpinBox *Spin = getInput<QSpinBox>();
  Q_ASSERT( Spin != NULL );
  return Spin;
}

// ------------------------------------------------------

QWidget* labelSpinWidget::createInputWidget()
{
  QSpinBox *Spin = new QSpinBox(this);
  Spin->setRange( std::numeric_limits<int>::min(), std::numeric_limits<int>::max() );
  connect( Spin, SIGNAL(valueChanged(int)), SIGNAL(valueChanged(int)));
  connect( Spin, SIGNAL(valueChanged(int)), SIGNAL(changed()) );
  return Spin;
}

// ------------------------------------------------------
      
int labelSpinWidget::value() const 
{ 
  return getSpinBox()->value(); 
}

// ------------------------------------------------------

labelSpinWidget& labelSpinWidget::setValue( int Value ) 
{ 
  getSpinBox()->setValue(Value); 
  return *this;
}

// ------------------------------------------------------

labelSpinWidget& labelSpinWidget::setReadOnly( bool ReadOnly ) 
{ 
  getSpinBox()->setReadOnly(ReadOnly); 
  return *this;
}

// ------------------------------------------------------

labelSpinWidget& labelSpinWidget::setRange( int Min, int Max ) 
{ 
  getSpinBox()->setRange(Min,Max); 
  return *this;
}

// ------------------------------------------------------

labelSpinWidget& labelSpinWidget::setMinimum( int Min ) 
{ 
  getSpinBox()->setMinimum(Min); 
  return *this;
}

// ------------------------------------------------------

labelSpinWidget& labelSpinWidget::setMaximum( int Max ) 
{ 
  getSpinBox()->setMaximum(Max); 
  return *this;
}

// ------------------------------------------------------

int labelSpinWidget::maximum() const 
{ 
  return getSpinBox()->maximum(); 
}

// ------------------------------------------------------

int labelSpinWidget::minimum() const 
{ 
  return getSpinBox()->minimum(); 
}
      
// ------------------------------------------------------

QVariant labelSpinWidget::getVariantValue() const 
{ 
  return value(); 
}

// ------------------------------------------------------

void labelSpinWidget::setVariantValue( const QVariant &Value ) 
{ 
  setValue( Value.toInt() ); 
}

// ======================================================
      
labelDoubleSpinWidget::labelDoubleSpinWidget( const QString& LabelText, QWidget *Parent ) : 
  labelInputWidget(Parent,LabelText) 
{ 
  initWidget(LabelText); 
}
      
// ------------------------------------------------------
labelDoubleSpinWidget::labelDoubleSpinWidget( const QString& LabelText, double Value, QWidget *Parent ) :
  labelInputWidget(Parent,LabelText) 
{ 
  initWidget(LabelText); 
  setValue(Value); 
}

// ------------------------------------------------------

labelDoubleSpinWidget::labelDoubleSpinWidget( const QString& LabelText, double Min, double Max, QWidget *Parent ) :
  labelInputWidget(Parent,LabelText) 
{ 
  initWidget(LabelText); 
  setRange(Min,Max); 
}

// ------------------------------------------------------
      
labelDoubleSpinWidget::labelDoubleSpinWidget( const QString& LabelText, double Value, double Min, double Max, QWidget *Parent ) :
  labelInputWidget(Parent,LabelText) 
{ 
  initWidget(LabelText); 
  setValue(Value); 
  setRange(Min,Max); 
}

// ------------------------------------------------------

const QDoubleSpinBox* labelDoubleSpinWidget::getDoubleSpinBox() const 
{
  return getInput<QDoubleSpinBox>(); 
}

// ------------------------------------------------------

QDoubleSpinBox* labelDoubleSpinWidget::getDoubleSpinBox() 
{ 
  return getInput<QDoubleSpinBox>(); 
}

// ------------------------------------------------------

QWidget* labelDoubleSpinWidget::createInputWidget()
{
  QDoubleSpinBox *Spin = new QDoubleSpinBox(this);
  Spin->setRange( -std::numeric_limits<double>::max(), +std::numeric_limits<double>::max() );
  Spin->setDecimals( 3 );
  connect( Spin,  SIGNAL(valueChanged(double)), SIGNAL(valueChanged(double))); 
  connect( Spin,  SIGNAL(valueChanged(double)), SIGNAL(changed()) ); 
  return Spin;
}

// ------------------------------------------------------
      
double labelDoubleSpinWidget::value() const 
{ 
  return getDoubleSpinBox()->value(); 
}

// ------------------------------------------------------
      
labelDoubleSpinWidget& labelDoubleSpinWidget::setValue( double Value ) 
{ 
  getDoubleSpinBox()->setValue(Value); 
  return *this;
}

// ------------------------------------------------------

labelDoubleSpinWidget& labelDoubleSpinWidget::setReadOnly( bool ReadOnly ) 
{ 
  getDoubleSpinBox()->setReadOnly(ReadOnly); 
  return *this;
}

// ------------------------------------------------------

labelDoubleSpinWidget& labelDoubleSpinWidget::setRange( double Min, double Max ) 
{ 
  getDoubleSpinBox()->setRange(Min,Max); 
  return *this;
}

// ------------------------------------------------------

labelDoubleSpinWidget& labelDoubleSpinWidget::setMinimum( double Min )
{
  getDoubleSpinBox()->setMinimum( Min );
  return *this;
}

// ------------------------------------------------------

labelDoubleSpinWidget& labelDoubleSpinWidget::setMaximum( double Max )
{
  getDoubleSpinBox()->setMaximum( Max );
  return *this;
}

// ------------------------------------------------------

double labelDoubleSpinWidget::minimum() const 
{ 
  return getDoubleSpinBox()->minimum(); 
}

// ------------------------------------------------------

double labelDoubleSpinWidget::maximum() const 
{ 
  return getDoubleSpinBox()->maximum(); 
}

// ------------------------------------------------------

labelDoubleSpinWidget& labelDoubleSpinWidget::setDecimals( int Decimals ) 
{ 
  getDoubleSpinBox()->setDecimals( Decimals ); 
  return *this;
}

// ------------------------------------------------------

int labelDoubleSpinWidget::decimals() const 
{ 
  return getDoubleSpinBox()->decimals(); 
}

// ------------------------------------------------------

labelDoubleSpinWidget& labelDoubleSpinWidget::setSingleStep( double S ) 
{ 
  getDoubleSpinBox()->setSingleStep(S);
  return *this;
}

// ------------------------------------------------------

double labelDoubleSpinWidget::singleStep() const 
{ 
  return getDoubleSpinBox()->singleStep(); 
}
      
// ------------------------------------------------------

QVariant labelDoubleSpinWidget::getVariantValue() const 
{ 
  return value(); 
}

// ------------------------------------------------------

void labelDoubleSpinWidget::setVariantValue( const QVariant &Value ) 
{ 
  setValue( Value.toDouble() ); 
}

// ======================================================

labelComboWidget::labelComboWidget( const QString& LabelText, QWidget *Parent ) : 
  labelInputWidget(Parent,LabelText) 
{ 
  initWidget(LabelText); 
}

// ------------------------------------------------------

labelComboWidget::labelComboWidget( const QString& LabelText, const QStringList& Items, QWidget *Parent ) :
  labelInputWidget(Parent,LabelText) 
{ 
  initWidget(LabelText); 
  addItems(Items); 
}

// ------------------------------------------------------

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

const QComboBox* labelComboWidget::getComboBox() const 
{ 
  return getInput<QComboBox>(); 
}

// ------------------------------------------------------
      
QComboBox* labelComboWidget::getComboBox() 
{ 
  QComboBox *ComboBox = getInput<QComboBox>();
  Q_ASSERT( ComboBox != NULL );
  return ComboBox;
}

// ------------------------------------------------------

QVariant labelComboWidget::valueToSettings() const 
{ 
  return currentIndex(); 
}

// ------------------------------------------------------

void labelComboWidget::setValueFromSettings( const QVariant &Value ) 
{ 
  int Index = qMax( 0, Value.toInt() );
  setCurrentIndex( Index ); 
}

// ------------------------------------------------------

int  labelComboWidget::currentIndex() const 
{ 
  return getComboBox()->currentIndex(); 
}

// ------------------------------------------------------

int  labelComboWidget::count() const 
{ 
  return getComboBox()->count(); 
}

// ------------------------------------------------------

QString labelComboWidget::currentText() const 
{ 
  return getComboBox()->currentText(); 
}

// ------------------------------------------------------

QVariant labelComboWidget::currentData() const 
{ 
  return getComboBox()->itemData( currentIndex() ); 
}

// ------------------------------------------------------

labelComboWidget& labelComboWidget::addItem( const QString &Text ) 
{
  return addItem( Text, QVariant(Text) );
}

// ------------------------------------------------------

labelComboWidget& labelComboWidget::addItem( const QString &Text, const QVariant &UserData ) 
{ 
  getComboBox()->addItem(Text,UserData); 
  return *this;
}

// ------------------------------------------------------

labelComboWidget& labelComboWidget::addItems( const QStringList& List ) 
{ 
  foreach( QString String, List ) 
    addItem( String ); 
  return *this;
}

// ------------------------------------------------------
      
void labelComboWidget::clear() 
{ 
  getComboBox()->clear(); 
}

// ------------------------------------------------------

labelComboWidget& labelComboWidget::setCurrentIndex( int Index ) 
{ 
  getComboBox()->setCurrentIndex(Index); 
  return *this;
}

// ------------------------------------------------------
      
labelComboWidget& labelComboWidget::setCurrentData( const QVariant& V ) 
{ 
  for ( int i = 0; i < count(); i++ ) 
  { 
    if (getComboBox()->itemData(i) == V ) 
      setCurrentIndex(i); 
  } 
  return *this;
}

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



// ======================================================

#include "sciAppFramework/multiInputWidget.h"
#include "sciAppFramework/multiInputXmlFactory.h"

#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QDomNamedNodeMap>
#include <QRegExp>
#include <QDebug>

using namespace sciAppFramework;

// ======================================================

namespace 
{
  
  // ------------------------------------------------------

  class labelModifier : public multiInputWidgetXmlFactory::modifierOfMultiInputWidget
  {
    public:
      QString tag() const { return "label"; }
      void addToMultiInputWidget( multiInputWidget *Widget, const QDomElement &Element ) const;
  };

  // ------------------------------------------------------
  
  class spacingModifier : public multiInputWidgetXmlFactory::modifierOfMultiInputWidget
  {
    public:
      QString tag() const { return "spacing"; }
      void addToMultiInputWidget( multiInputWidget *Widget, const QDomElement &Element ) const;
  };
  
  // ------------------------------------------------------
  
  class tabModifier : public multiInputWidgetXmlFactory::modifierOfMultiInputWidget
  {
    public:
      QString tag() const { return "tabs"; }
      void addToMultiInputWidget( multiInputWidget *Widget, const QDomElement &Element ) const;
  };
  
  // ------------------------------------------------------
  
  template <class input> class inputModifier : public multiInputWidgetXmlFactory::modifierOfMultiInputWidget
  {
    private:
      static int InputsCount;

    private:
      input* createInputWidget( multiInputWidget *Widget, const QDomElement &Element ) const;
      void setStretchFactors( labelInputWidget* Input, const QDomElement &Element ) const;

    protected:
      virtual void setUpInput( input *Input, const QDomElement &Element ) const {}

    public:
      void addToMultiInputWidget( multiInputWidget *Widget, const QDomElement &Element ) const;
  };

  template <class input> int inputModifier<input>::InputsCount = 0;
  
  // ------------------------------------------------------
  
  class editModifier : public inputModifier<labelEditWidget>
  {
    public:
      QString tag() const { return "edit"; }
  };
  
  // ------------------------------------------------------
  
  class doubleEditModifier : public inputModifier<labelDoubleEditWidget>
  {
    protected:
      void setUpInput( labelDoubleEditWidget *Input, const QDomElement &Element ) const;
    public:
      QString tag() const { return "double"; }
  };
    
  // ------------------------------------------------------

  class pathEditModifier : public inputModifier<labelPathEditWidget>
  {
    protected:
      void setUpInput( labelPathEditWidget *Input, const QDomElement &Element ) const;
    public:
      QString tag() const { return "path"; }
  };

  // ------------------------------------------------------
  
  class spinModifier : public inputModifier<labelSpinWidget>
  {
    protected:
      void setUpInput( labelSpinWidget *Input, const QDomElement &Element ) const;
    public:
      QString tag() const { return "spin"; }
  };

  // ------------------------------------------------------
  
  class doubleSpinModifier : public inputModifier<labelDoubleSpinWidget>
  {
    protected:
      void setUpInput( labelDoubleSpinWidget *Input, const QDomElement &Element ) const;
    public:
      QString tag() const { return "doubleSpin"; }
  };
  
  // ------------------------------------------------------
  
  template <class multiItemsInput> class multiItemsModifier : public inputModifier<multiItemsInput>
  {
    protected:
      void setUpInput( multiItemsInput *Input, const QDomElement &Element ) const;
  };
  
  // ------------------------------------------------------
  
  class comboModifier : public multiItemsModifier<labelComboWidget>
  {
    public:
      QString tag() const { return "combo"; }
  };
  
  // ------------------------------------------------------

  class radioModifier : public multiItemsModifier<radioButtonWidget>
  {
    public:
      QString tag() const { return "radio"; }
  };
  
  // ------------------------------------------------------
  
  class checkModifier : public inputModifier<checkBoxWidget>
  {
    public:
      QString tag() const { return "check"; }
  };

  // ======================================================
  
  void labelModifier::addToMultiInputWidget( multiInputWidget *Widget, const QDomElement &Element ) const
  {
    if ( Widget == NULL || Element.tagName() != tag() ) 
      return;

    foreach( QString Text, text( Element, "text" ) )
      Widget->addLabel( Text );
  }
  
  // ------------------------------------------------------
  
  void spacingModifier::addToMultiInputWidget( multiInputWidget *Widget, const QDomElement &Element ) const
  {
    if ( Widget == NULL || Element.tagName() != tag() ) 
      return;
    
    int SpacingValue = text( Element, "value" ).value( 0 ).toInt();
    if ( SpacingValue > 0 )
      Widget->addSpacing( SpacingValue );
  }
  
  // ------------------------------------------------------
  
  void tabModifier::addToMultiInputWidget( multiInputWidget *Widget, const QDomElement &Element ) const
  {
    if ( Widget == NULL ) 
      return;

#if 0
    multiInputWidgetXmlFactory::modifierOfMultiInputWidgetMap *Modifiers = createModifiersMap();
    
    multiInputWidget *SubWidget = new multiInputWidget(Widget);
    for ( QDomNode Node = Root.firstChild(); ! Node.isNull(); Node = Node.nextSibling() )  
      addNextItemToMultiInputWidget( Widget, Node.toElement() );
    Widget->addTabMultiInputWidget(  );
#endif
    //for ( QDomNode Node = Root.firstChild(); ! Node.isNull(); Node = Node.nextSibling() )  
    //  addNextItemToMultiInputWidget( Widget, Node.toElement() );
  }
  
  // ------------------------------------------------------
    
  template <class input> void inputModifier<input>::addToMultiInputWidget( multiInputWidget *Widget, const QDomElement &Element ) const
  {
    if ( Element.tagName() != tag() ) 
      return;

    input *Input = createInputWidget( Widget, Element );
    setStretchFactors( dynamic_cast<labelInputWidget*>( Input ), Element );
    setUpInput( Input, Element );
  }
  
  // ------------------------------------------------------ 
      
  template <class input> input* inputModifier<input>::createInputWidget( multiInputWidget *Widget, const QDomElement &Element ) const
  {
    const QString &Name  = attribute( Element, "name", tag() + QString::number(++InputsCount) );
    const QString &Label = text( Element, "label" ).join(" ");
    const QString &Value = text( Element, "value" ).join(" ");

    if ( Widget == NULL )
      return inputWidget::create<input>( NULL, Name, Label, QVariant(Value) );
    return Widget->addInputWidget<input>( Name, Label, QVariant(Value) );
  }
  
  // ------------------------------------------------------ 
  
  template <class input> void inputModifier<input>::setStretchFactors( labelInputWidget* Input, const QDomElement &Element ) const
  {
    if ( Input == NULL )
      return;
    
    int LabelStretch = 0, InputStretch = 0;
    const QDomNodeList &StretchNodes = Element.elementsByTagName( "stretch" );
    if ( ! StretchNodes.isEmpty() )
    {
      LabelStretch = text( StretchNodes.item(0).toElement(), QString("label") ).value( 0, "" ).toInt();
      InputStretch = text( StretchNodes.item(0).toElement(), QString("input") ).value( 0, "" ).toInt();
    }
    
    if ( LabelStretch > 0 && InputStretch > 0 )
      Input->setStretchFactors( LabelStretch, InputStretch );
  }
  
  // ------------------------------------------------------ 
  
  void pathEditModifier::setUpInput( labelPathEditWidget *Input, const QDomElement &Element ) const
  {
    Q_ASSERT( Input != NULL );

    QString AcceptMode = text( Element, "mode" ).value(0);

    QRegExp DirMode( "dir$" );
    if ( AcceptMode.contains( DirMode ) )
    {
      Input->setFileMode( QFileDialog::DirectoryOnly );
      AcceptMode.replace( DirMode, "" );
    }

    if ( AcceptMode.isEmpty() )
      /* do nothing */;
    else if ( AcceptMode == "open" )
      Input->setAcceptMode( QFileDialog::AcceptOpen );
    else if ( AcceptMode == "save" )
      Input->setAcceptMode( QFileDialog::AcceptSave );
    else
      qWarning() << "pathEditModifier::setUpInput: invalid accept mode";
  }
  
  // ------------------------------------------------------ 
      
  void doubleEditModifier::setUpInput( labelDoubleEditWidget *Input, const QDomElement &Element ) const
  {
    Q_ASSERT( Input != NULL );
    
    bool OkMin = false, OkMax = false;
    double MinValue = text( Element, "min" ).value( 0 ).toDouble( &OkMin );
    double MaxValue = text( Element, "max" ).value( 0 ).toDouble( &OkMax );

    if ( ! OkMin )      MinValue = Input->minimum();
    if ( ! OkMax )      MaxValue = Input->maximum();
    Input->setRange( MinValue, MaxValue );
  }
  
  // ------------------------------------------------------ 
      
  void spinModifier::setUpInput( labelSpinWidget *Input, const QDomElement &Element ) const
  {
    Q_ASSERT( Input != NULL );

    bool OkMin = false, OkMax = false;
    int MinValue = text( Element, "min" ).value( 0 ).toInt( &OkMin );
    int MaxValue = text( Element, "max" ).value( 0 ).toInt( &OkMax );

    if ( ! OkMin )      MinValue = Input->minimum();
    if ( ! OkMax )      MaxValue = Input->maximum();
    Input->setRange( MinValue, MaxValue );
  }

  // ------------------------------------------------------ 
      
  void doubleSpinModifier::setUpInput( labelDoubleSpinWidget *Input, const QDomElement &Element ) const
  {
    Q_ASSERT( Input != NULL );

    bool OkMin = false, OkMax = false;
    double MinValue = text( Element, "min" ).value( 0 ).toDouble( &OkMin );
    double MaxValue = text( Element, "max" ).value( 0 ).toDouble( &OkMax );
    if ( ! OkMin )  MinValue = Input->minimum();
    if ( ! OkMax )  MaxValue = Input->maximum();
    Input->setRange( MinValue, MaxValue );

    bool OkDecimals = false;
    double Decimals = text( Element, "decimals" ).value( 0 ).toDouble( &OkDecimals );
    if ( ! OkDecimals ) Decimals = Input->decimals();
    Input->setDecimals( Decimals );

    bool OkStep = false;
    double Step = text( Element, "step" ).value( 0 ).toDouble( &OkStep );
    if ( ! OkStep ) Step = Input->singleStep();
    Input->setSingleStep( Step );
  }

  // ------------------------------------------------------ 

  template <class multiItemsInput> void multiItemsModifier<multiItemsInput>::setUpInput( multiItemsInput *Input, const QDomElement &Element ) const
  {
    Q_ASSERT( Input != NULL );

    for ( QDomNode Node = Element.firstChild(); ! Node.isNull(); Node = Node.nextSibling() )
    {
      if ( Node.nodeName() == "item" )
      {
        const QString &Value = multiInputWidgetXmlFactory::modifierOfMultiInputWidget::attribute( Node.toElement(), QString("data") );
        const QString &Text = multiInputWidgetXmlFactory::modifierOfMultiInputWidget::text( Node ).join("");
        Input->addItem( Text, QVariant(Value) );
      }
    }
    
  };

  // ======================================================

}

// ======================================================

QString multiInputWidgetXmlFactory::modifierOfMultiInputWidget::attribute( const QDomElement &Element, const QString &AttrName, const QString &Default )
{
  const QDomNamedNodeMap &Attributes = Element.attributes();
  for ( int i = 0; i < Attributes.count(); i++ )
  {
    const QDomAttr &Attr = Attributes.item(i).toAttr();
    if ( Attr.name() == AttrName )
      return Attr.value();
  }

  return Default;
}

// ------------------------------------------------------

QStringList multiInputWidgetXmlFactory::modifierOfMultiInputWidget::text( const QDomElement &Element )
{
  QStringList Result;

  for ( QDomNode TextNode = Element.firstChild(); ! TextNode.isNull(); TextNode = TextNode.nextSibling() )
  {
    if ( TextNode.nodeType() == QDomNode::TextNode )
      Result.append( TextNode.toText().data() );
  }

  return Result;
}

// ------------------------------------------------------

QStringList multiInputWidgetXmlFactory::modifierOfMultiInputWidget::text( const QDomElement &Element, const QString &NodeName )
{
  QStringList Result;
  for ( QDomNode Node = Element.firstChild(); ! Node.isNull(); Node = Node.nextSibling() )
    if ( Node.nodeName() == NodeName )
      Result.append( text( Node ) );

  return Result;
}

// ------------------------------------------------------

QStringList multiInputWidgetXmlFactory::modifierOfMultiInputWidget::text( const QDomNode &Node )
{
  return text( Node.toElement() );
}

// ------------------------------------------------------

QStringList multiInputWidgetXmlFactory::modifierOfMultiInputWidget::text( const QDomNodeList &Nodes )
{
  QStringList Result;

  for ( int i = 0; i < Nodes.count(); i++ )
    Result.append( text(Nodes.item(i) ) );

  return Result;
}

// ======================================================

multiInputWidget* multiInputWidgetXmlFactory::create( const QString &Xml, QWidget *Parent )
{
  return multiInputWidgetXmlFactory( Xml ).create( Parent );
}

// ------------------------------------------------------

multiInputWidgetXmlFactory::multiInputWidgetXmlFactory( const QString &Xml ) :
  ErrorString(),
  Doculemt( createDomDocument(Xml,&ErrorString) ),
  Modifiers( createModifiersMap() )
{
}

// ------------------------------------------------------

multiInputWidgetXmlFactory::~multiInputWidgetXmlFactory()
{
  deleteModifiersMap( Modifiers );
  delete Doculemt;
}

// ------------------------------------------------------

multiInputWidgetXmlFactory::modifierOfMultiInputWidgetMap* multiInputWidgetXmlFactory::createModifiersMap()
{

  QList< modifierOfMultiInputWidget* > Mods;
  Mods << 
    new labelModifier() <<
    new spacingModifier() <<
    new editModifier() <<
    new doubleEditModifier() <<
    new pathEditModifier() <<
    new spinModifier() << 
    new doubleSpinModifier() <<
    new comboModifier() <<
    new radioModifier() << 
    new checkModifier();

  modifierOfMultiInputWidgetMap *ModifiersMap = new modifierOfMultiInputWidgetMap();

  for ( int i = 0; i < Mods.size(); i++ )
    addModifierOfMultiInputWidget( Mods[i], ModifiersMap );

  return ModifiersMap;
}

// ------------------------------------------------------
      
void multiInputWidgetXmlFactory::deleteModifiersMap( modifierOfMultiInputWidgetMap *ModifiersMap )
{
  if ( ModifiersMap == NULL )
    return;

  foreach ( modifierOfMultiInputWidget *Mod, *ModifiersMap )
    delete Mod;
  ModifiersMap->clear();
  delete ModifiersMap;
}

// ------------------------------------------------------

QDomDocument* multiInputWidgetXmlFactory::createDomDocument( const QString &Xml, QString *ErrorString )
{
  QDomDocument *Result = new QDomDocument();

  QString XmlErrorMessage;
  int ErrorLine = -1;
  bool OK = Result->setContent( Xml, &XmlErrorMessage, &ErrorLine );

  if ( ! OK )
  {
    if ( ErrorString != NULL )
      *ErrorString = XmlErrorMessage + "(Line: " + QString::number(ErrorLine) + ")";
    qWarning() << "multiInputWidgetXmlFactory: invalid xml string, " << ErrorString;
    delete Result;
    Result = NULL;
  }

  return Result;
}

// ------------------------------------------------------

void multiInputWidgetXmlFactory::addModifierOfMultiInputWidget( modifierOfMultiInputWidget *Mod, modifierOfMultiInputWidgetMap *ModifiersMap )
{
  if ( Mod == NULL || ModifiersMap == NULL )
    return;

  QString Tag = Mod->tag();

  delete ModifiersMap->value( Tag, NULL );
  ModifiersMap->insert( Tag, Mod );
}

// ------------------------------------------------------
      
multiInputWidget* multiInputWidgetXmlFactory::create( QWidget *Parent ) const
{
  multiInputWidget *Widget = new multiInputWidget( Parent );
  addItemsToMultiInputWidget( Widget ); 
  return Widget;
}

// ------------------------------------------------------

void multiInputWidgetXmlFactory::addItemsToMultiInputWidget( multiInputWidget* Widget ) const
{
  if ( Widget == NULL )
    return;
 
  const QDomElement &Root = Doculemt->documentElement();
  setSettingsName( Widget, Root );

  for ( QDomNode Node = Root.firstChild(); ! Node.isNull(); Node = Node.nextSibling() )  
    addNextItemToMultiInputWidget( Widget, Node.toElement() );

}

// ------------------------------------------------------

void multiInputWidgetXmlFactory::setSettingsName( settingsObject *SettingsObject, const QDomElement &Element )
{
  Q_ASSERT( SettingsObject != NULL );

  QString TagName = Element.tagName();
  SettingsObject->setSettingsName( TagName );
}

// ------------------------------------------------------

void multiInputWidgetXmlFactory::addNextItemToMultiInputWidget( multiInputWidget *Widget, const QDomElement &Element ) const
{
  Q_ASSERT( Widget != NULL );

  QString TagName = Element.tagName();

  modifierOfMultiInputWidget *Mod = Modifiers->value( TagName, NULL ) ;
  if ( Mod == NULL )
  {
    qWarning() << "addNextItemToMultiInputWidget: invalid tag " << TagName;
    return;
  }

  Mod->addToMultiInputWidget( Widget, Element );
}
      
// ------------------------------------------------------

QString multiInputWidgetXmlFactory::xmlRootName( const QString &Xml )
{
  QDomDocument Doculemt;
  
  QString XmlErrorMessage;
  int ErrorLine = -1;
  bool OK = Doculemt.setContent( Xml, &XmlErrorMessage, &ErrorLine );

  if ( ! OK )
  {
    qWarning() << "multiInputWidgetXmlFactory::xmlRootName: invalid xml string, " << XmlErrorMessage << " line " << ErrorLine;
    return QString();
  }

  const QDomElement &Root = Doculemt.documentElement();
  QString TagName = Root.tagName();

  return TagName;
}

// ======================================================


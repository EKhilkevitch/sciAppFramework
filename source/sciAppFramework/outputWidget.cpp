
// =========================================

#include "sciAppFramework/outputWidget.h"
#include "scigraphics/qt4/manager.h"

#include <QStackedLayout>
#include <QTabWidget>
#include <QSettings>

using namespace sciAppFramework;

// =========================================

plotManagerOutputWidgetItem::plotManagerOutputWidgetItem( scigraphics::qt4plotManager *PM, const QString &Title ) : 
  outputWidgetItem(Title), 
  PlotManager(PM)
{
  Q_ASSERT( PlotManager != NULL );
}

// -----------------------------------------

QWidget* plotManagerOutputWidgetItem::outputWidget()
{
  return PlotManager->plotWidget();
}

// -----------------------------------------

QWidget* plotManagerOutputWidgetItem::outputSettingsWidget()
{
  return PlotManager->settingsWidget();
}
      
// -----------------------------------------

void plotManagerOutputWidgetItem::saveSettings( QSettings *Settings )
{
  PlotManager->saveSettings( Settings );
}

// -----------------------------------------

void plotManagerOutputWidgetItem::loadSettings( QSettings *Settings )
{
  PlotManager->loadSettings( Settings );
}

// =========================================

outputWidget::outputWidget( QWidget *Parent, const QList<outputWidgetItem*> &OutputWidgets ) : QWidget(Parent)
{
  setLayout( new QStackedLayout() );

  foreach ( outputWidgetItem *Item, OutputWidgets )
    appendOutputWidgetItem( Item );
}

// -----------------------------------------

outputWidget::~outputWidget()
{
  foreach ( outputWidgetItem *Item, OutputWidgetItems )
    delete Item;
  OutputWidgetItems.clear();
}

// -----------------------------------------

void outputWidget::appendOutputWidgetItem( outputWidgetItem *Item )
{
  if ( Item == NULL )
    return;
  
  OutputWidgetItems.append( Item );
    
  QStackedLayout *Layout = dynamic_cast< QStackedLayout* >( layout() );

  switch ( OutputWidgetItems.size() )
  {
    case 0:
      qFatal( "OutputWidgetItems.size() == 0" );
      break;
    case 1:
      Layout->addWidget( Item->outputWidget() );
      break;
    case 2:
      Layout->takeAt(0);
      Layout->addWidget( createTabWidget() );
      addToTabWidget( OutputWidgetItems.first()->outputWidget(), OutputWidgetItems.first()->title() );
      /* no break */
    default:
      addToTabWidget( Item->outputWidget(), Item->title() );
      break;
  }

}

// -----------------------------------------
      
QTabWidget* outputWidget::createTabWidget()
{
  QTabWidget *TabWidget = new QTabWidget(this);
  connect( TabWidget,  SIGNAL(currentChanged(int)), SIGNAL(currentOutputChanged(int)) );
  TabWidget->setTabPosition( QTabWidget::South );
  return TabWidget;
}

// -----------------------------------------
      
void outputWidget::addToTabWidget( QWidget *Widget, const QString &Title )
{
  if ( Widget == NULL )
    return;
  if ( layout() == NULL )
    return;
  if ( layout()->count() <= 0 )
    return;

  QTabWidget *TabWidget = dynamic_cast<QTabWidget*>(layout()->itemAt(0)->widget());
  if ( TabWidget == NULL )
    return;

  TabWidget->addTab( Widget, Title );
}

// -----------------------------------------

void outputWidget::saveSettings( QSettings *Settings )
{
  if ( Settings == NULL )
    return;

  foreach ( outputWidgetItem *Item, OutputWidgetItems )
  {
    Settings->beginGroup( Item->title() );
    Item->saveSettings( Settings );
    Settings->endGroup();
  }
}

// -----------------------------------------

void outputWidget::loadSettings( QSettings *Settings )
{
  if ( Settings == NULL )
    return;
  
  foreach ( outputWidgetItem *Item, OutputWidgetItems )
  {
    Settings->beginGroup( Item->title() );
    Item->loadSettings( Settings );
    Settings->endGroup();
  }
}

// -----------------------------------------

const QList<QWidget*> outputWidget::listOfSettingsWidgets()
{
  QList<QWidget*> SettingsWidgets;

  foreach ( outputWidgetItem *Item, OutputWidgetItems )
    SettingsWidgets.append( Item->outputSettingsWidget() );

  return SettingsWidgets;
}

// =========================================


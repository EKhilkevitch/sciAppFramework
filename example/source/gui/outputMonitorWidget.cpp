
// ======================================================

#include "scigraphics/qt4/qt4plotmanager.h"
#include "outputMonitorWidget.h"

using namespace sciAppFramework;

// ======================================================

outputMonitorWidget::outputMonitorWidget( QWidget *Parent ) : outputWidget(Parent)
{
  LoadingPlotManager = new qt4plotManager(2,2,this,qt4plotManager::SharedSettings);
  appendOutputWidgetItem( new plotManagerOutputWidgetItem( LoadingPlotManager, "Loading graphics" ) );
}

// ======================================================



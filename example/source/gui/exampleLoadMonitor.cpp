
// ======================================================

#include "exampleLoadMonitor.h"
#include "controlMonitorWidget.h"
#include "outputMonitorWidget.h"
#include "monitorThread.h"

using namespace sciAppFramework;

// ======================================================

controlWidget* exampleLoadMonitor::createControlWidget()
{
  return new controlMonitorWidget(this);
}

// ------------------------------------------------------

outputWidget* exampleLoadMonitor::createOutputWidget()
{
  return new outputMonitorWidget(this);
}

// ------------------------------------------------------

measurementThread* exampleLoadMonitor::createMeasurementThread( const measurementParameters& Parameters )
{
  return new monitorThread(Parameters);
}

// ======================================================



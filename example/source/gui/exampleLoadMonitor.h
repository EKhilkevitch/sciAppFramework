
#pragma once

// ======================================================

#include "sciAppFramework/mainWidget.h"

// ======================================================

class exampleLoadMonitor : public sciAppFramework::mainWidget
{
  private:
    sciAppFramework::controlWidget* createControlWidget();
    sciAppFramework::outputWidget* createOutputWidget();
    sciAppFramework::measurementThread* createMeasurementThread( const sciAppFramework::measurementParameters& Parameters );

  public:
    exampleLoadMonitor() { initWidget(); }
};

// ======================================================


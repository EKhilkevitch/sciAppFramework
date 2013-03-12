
#pragma once

// ======================================================

#include "sciAppFramework/outputWidget.h"

// ======================================================

class outputMonitorWidget : public sciAppFramework::outputWidget
{
  private:
    qt4plotManager *LoadingPlotManager;

  public:
    outputMonitorWidget( QWidget *Parent );
};

// ======================================================


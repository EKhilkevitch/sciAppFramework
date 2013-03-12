
#pragma once

// ======================================================

#include "sciAppFramework/controlWidget.h"

// ======================================================

class controlMonitorWidget : public sciAppFramework::controlWidget
{
  private:
    sciAppFramework::multiInputWidget* createMultiInputWidget();

  public:
    controlMonitorWidget( QWidget *Parent ) : sciAppFramework::controlWidget(Parent) { initWidget(); }
};

// ======================================================


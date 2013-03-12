
// ======================================================

#include "sciAppFramework/multiInputWidget.h"
#include "sciAppFramework/inputWidget.h"

#include "controlMonitorWidget.h"

using namespace sciAppFramework;

// ======================================================

multiInputWidget* controlMonitorWidget::createMultiInputWidget()
{
  multiInputWidget *Widget = new multiInputWidget(this);
  return Widget;
}

// ======================================================



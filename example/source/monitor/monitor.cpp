
// ======================================================

#include "monitor.h"

// ======================================================

loadingState loadingInInterval( loadingState Current, loadingState Previous )
{
  return loadingState( Current.TimeOfWorking - Previous.TimeOfWorking, Current.TimeOfIdle - Previous.TimeOfIdle );
}

// ------------------------------------------------------

// ======================================================



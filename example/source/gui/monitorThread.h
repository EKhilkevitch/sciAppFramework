
#pragma once

// ======================================================

#include "sciAppFramework/measurementThread.h"

// ======================================================

class monitorThread : public sciAppFramework::measurementThread
{
  public:
    monitorThread( const sciAppFramework::measurementParameters &Parameters );
};

// ======================================================


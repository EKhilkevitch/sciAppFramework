

#pragma once

// ======================================================

#include <string>
#include <list>

// ======================================================

struct loadingState 
{
  double TimeOfWorking;
  double TimeOfIdle;
  loadingState() : TimeOfWorking(0), TimeOfIdle(0) {}
  loadingState( double W, double I ) : TimeOfWorking(W), TimeOfIdle(I) {}
};

loadingState loadingInInterval( loadingState Current, loadingState Previous );

// ======================================================

class monitorData
{
  friend class monitor;

  private:
    double TimeInterval;
    double IdleInterval;
    unsigned NumberOfProcessors;

  public:
    monitorData() {}

    double loadingPerSeconds() const { return 1 - idlePerSeconds(); }
    double idlePerSeconds() const { return TimeInterval == 0 || NumberOfProcessors == 0 ? 0 : ( IdleInterval/NumberOfProcessors) /TimeInterval; }
    unsigned numberOfProcessors() const { return NumberOfProcessors; }
};

// ======================================================

class monitor
{
  private:
    std::string UptimeFile;
    std::string ProcessorsFile;

    loadingState PrevState;

  public:
    monitor()  {}
    virtual ~monitor() {}
};

// ======================================================


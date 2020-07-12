#include "processor.h"

#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  long currentActive = LinuxParser::ActiveJiffies();
  long deltaActive = currentActive - prevActive;
  prevActive = currentActive;
  long currentTotal = LinuxParser::Jiffies();
  long deltaTotal = currentTotal - prevTotal;
  prevTotal = currentTotal;
  return float(deltaActive) / float(deltaTotal);
}
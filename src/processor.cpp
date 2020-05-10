#include "processor.h"
#include <unistd.h>

#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
 jiffieStart = LinuxParser::Jiffies();
 activeJiffieStart = LinuxParser::ActiveJiffies();

  usleep(200000);  // microseconds --> 100 milliseconds

  jiffieEnd = LinuxParser::Jiffies();
  activeJiffieEnd = LinuxParser::ActiveJiffies();

  long total = jiffieEnd  - jiffieStart;
  long active = activeJiffieEnd - activeJiffieStart;

  auto utilization = active / total;
  return utilization;
}
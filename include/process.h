#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "linux_parser.h"
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                               //  See src/process.cpp
  std::string User();                      //  See src/process.cpp
  std::string Command();                   //  See src/process.cpp
  float CpuUtilization();                  //  See src/process.cpp
  std::string Ram();                       //  See src/process.cpp
  long int UpTime();                       //  See src/process.cpp
  bool operator<(Process const& a) const;  //  See src/process.cpp
    Process(int pid);
  //  Declare any necessary private members
 private:
 int pid_;
 float seconds, total_time, cpu_usage;
};

#endif
#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"


using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid){
    Process::pid_ =pid;
    cpu_usage = Process::CpuUtilization();
}
// TODO: Return this process's ID
int Process::Pid() { return pid_;}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    total_time = LinuxParser::ActiveJiffies(pid_)/sysconf(_SC_CLK_TCK);
    seconds = LinuxParser::UpTime(pid_);
    return total_time/seconds;
 }

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
// bool Process::operator<(Process const& a) const {
//       return stol(LinuxParser::Ram(pid_)) > stol(LinuxParser::Ram(a.pid_));
//       }

      bool Process::operator<(Process const& a) const { 
  return cpu_usage > a.cpu_usage; 
}
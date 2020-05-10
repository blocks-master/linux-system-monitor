#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::stol;

//  how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}


vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line, key, value;
  float MemTotal, MemFree;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if(stream.is_open()){
    while(std::getline(stream,line)){
      std::istringstream lineStream(line);
        lineStream >> key >> value;
        if(key == "MemTotal:"){
          MemTotal = stof(value);
        }
        if(key == "MemFree:") {
          MemFree = stof(value);         
        }
    }
    return (MemTotal - MemFree)/MemTotal;
  }
  return 0.0;
  
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string uptime;
  string line;

  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if(stream.is_open()){
    std::getline(stream,line);
    std::istringstream lineStream(line);
    lineStream >> uptime;
    return stol(uptime);
    
  }
  return 0.0;
 }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  long jiffies_amount = 0;
  auto cpu_values = LinuxParser::CpuUtilization();
  for(auto i: cpu_values){
    jiffies_amount += stol(i);
  }
  return jiffies_amount;
 }

// TODO: Read and return the number of active jiffies for a PID

long LinuxParser::ActiveJiffies(int pid) { 
  string value,line;
  long active_jiffies;
  vector<string> stat_list;
  std::ifstream stream(kProcDirectory + to_string(pid)+kStatFilename);
  if(stream.is_open()){
    std::getline(stream,line);
    std::istringstream lineStream(line);
    while (lineStream >> value){
      stat_list.push_back(value);
    }
  }
  
  long utime {stol(stat_list[13])};
  long stime {stol(stat_list[14])};
  long cutime {stol(stat_list[15])};
  long cstime {stol(stat_list[16])};

   
active_jiffies = utime + stime + cutime + cstime;
return active_jiffies;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
return LinuxParser::Jiffies() - LinuxParser::IdleJiffies();
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  long idle_amount = 0;
  auto cpu_values = LinuxParser::CpuUtilization();
    
  idle_amount = stol(cpu_values[3]) + stol(cpu_values[4]);

  
  return idle_amount;
 }


// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line, value;
  vector<string> cpu_values;

  std::ifstream stream(kProcDirectory+kStatFilename);
  if(stream.is_open()) {
    std::getline(stream,line);
    std::istringstream lineStream(line);
    while (lineStream >> value){
      if(value != "cpu"){
        cpu_values.push_back(value);
      }
    }
  }
  return cpu_values;

}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  int total;
  string key, value, line;
  std::ifstream stream(kProcDirectory+ kStatFilename);
  if (stream.is_open()){
    while(std::getline(stream,line)) {
      std::istringstream lineStream(line);
        lineStream >> key >> value;
        if(key == "processes"){
          total = std::stoi(value);
          return total;
        }
    }
    
  }
  return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  int running;
  string key, value, line;
  std::ifstream stream(kProcDirectory+ kStatFilename);
  if (stream.is_open()){
    while(std::getline(stream,line)) {
      std::istringstream lineStream(line);
        lineStream >> key >> value;
        if(key == "procs_running"){
          running = std::stoi(value);
          return running;
        }
    }
    
  }
  return 0;
 }

// TODO: Read and return the command associated with a process

string LinuxParser::Command(int pid) { 
  string command;

std::ifstream stream(kProcDirectory + to_string(pid)+kCmdlineFilename);
if (stream.is_open()){
  std::getline(stream,command);
  return command;
}
  return string();
}

// TODO: Read and return the memory used by a process

string LinuxParser::Ram(int pid) { 
  string key, value, line;
  int ram;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()){
    while (std::getline(stream,line)) {
      std::istringstream lineStream(line);
      lineStream >> key >> value;
      if(key == "VmSize:"){
        ram = std::stoi(value);
        ram = ram/1000;
        return to_string(ram);
      }
    }
  }
  return string();
  }

// TODO: Read and return the user ID associated with a process

string LinuxParser::Uid(int pid) {
  string key, value, line;

  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()){
    while (std::getline(stream,line)) {
      std::istringstream lineStream(line);
      lineStream >> key >> value;
      if(key == "Uid:"){
       return value;
      }
    }
  }
  return string();
 }

// TODO: Read and return the user associated with a process

string LinuxParser::User(int pid) { 
  string key, value, line, x;
  string uid = LinuxParser::Uid(pid);
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()){
    while (std::getline(stream,line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream lineStream(line);
      lineStream >> key >> x >> value;
      if(value == uid){
        return key;
      }
    }
  }
  return string();
}

// TODO: Read and return the uptime of a process

long LinuxParser::UpTime(int pid) { 
  string value,line;
  long starttime, uptime;
  vector<string> stat_list;
  std::ifstream stream(kProcDirectory + to_string(pid)+kStatFilename);
  if(stream.is_open()){
    std::getline(stream,line);
    std::istringstream lineStream(line);
    while (lineStream >> value){
      stat_list.push_back(value);
    }
  }

  starttime =  stol(stat_list[21])/sysconf(_SC_CLK_TCK);
  

  uptime =  LinuxParser::UpTime() - starttime;
  return uptime;
}
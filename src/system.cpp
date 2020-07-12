#include "system.h"

#include <unistd.h>

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  vector<int> currentpids = LinuxParser::Pids();
  vector<Process> procs = {};

  // check whether the process being examined is in the old vector of processes.
  for (auto process : processes_) {
    int x = process.Pid();
    for (auto pid : currentpids) {
      if (x == pid) {
        procs.push_back(process);
      }
    }
  }

  // check whether there is new procs, if yes, add it to the vector<Process>.
  for (int pid : currentpids) {
    bool alreadyexists = false;

    for (auto oldproc : procs) {
      if (pid == oldproc.Pid()) {
        alreadyexists = true;
        break;
      }
    }
    if (alreadyexists == false) {
      Process p{};
      p.SetPid(pid);
      procs.push_back(p);
    }
  }

  processes_ = procs;
  std::sort(processes_.begin(), processes_.end());
  return processes_;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }
#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
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

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, kernel2;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel >> kernel2;
  }
  return kernel2;
}

// BONUS: Update this to use std::filesystem
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
  string memorytotal, memoryfree;
  int mtotal, mfree;
  float musage = 0.0;
  string mline;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    std::getline(stream, mline);
    std::istringstream linestream(mline);
    linestream >> memorytotal >> mtotal;

    std::getline(stream, mline);
    std::istringstream linestream2(mline);
    linestream2 >> memoryfree >> mfree;

    musage = float(mtotal - mfree) / float(mtotal);
  }

  return musage;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  long uptime;
  string uline;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, uline);
    std::istringstream linestream(uline);
    linestream >> uline;
    uptime = stof(uline);
  }
  return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  auto jiffies = LinuxParser::CpuUtilization();

  return std::stol(jiffies[kUser_]) + std::stol(jiffies[kNice_]) +
         std::stol(jiffies[kSystem_]) + std::stol(jiffies[kIdle_]);
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  long nr14, nr15, nr16, nr17;
  string pid_ = to_string(pid);
  string activeline;
  std::ifstream stream(kProcDirectory + pid_ + kStatFilename);
  if (stream.is_open()) {
    getline(stream, activeline);
    std::istringstream linestream(activeline);
    for (int i = 0; i < 13; i++) {
      string word;
      linestream >> word;
    }
    linestream >> nr14 >> nr15 >> nr16 >> nr17;
    return nr14 + nr15 + nr16 + nr17;
  }
  return 0;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  auto util = LinuxParser::CpuUtilization();

  return std::stol(util[kUser_]) + std::stol(util[kNice_]) +
         std::stol(util[kSystem_]);
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  return std::stol(LinuxParser::CpuUtilization()[kIdle_]);
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> cpuutil;
  cpuutil.resize(10);
  string cpu;
  string utilline;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, utilline);
    std::istringstream linestream(utilline);
    linestream >> cpu;
    for (auto& util : cpuutil) {
      linestream >> utilline;
      util = utilline;
    }
  }
  return cpuutil;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string totalprocesses;
  int tprocesses = 0;
  string pline;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, pline)) {
      std::istringstream linestream(pline);
      linestream >> totalprocesses >> tprocesses;
      if (totalprocesses == "processes") {
        break;
      }
    }
  }
  return tprocesses;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string runningprocesses;
  int rprocesses = 0;
  string rline;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, rline)) {
      std::istringstream linestream(rline);
      linestream >> runningprocesses >> rprocesses;
      if (runningprocesses == "procs_running") {
        break;
      }
    }
  }
  return rprocesses;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string command;
  string cline;
  string pid_ = to_string(pid);
  std::ifstream stream(kProcDirectory + pid_ + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, cline);
    command = cline;
    std::replace(command.begin(), command.end(), '\0', ' ');
  }
  return command;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string ramusage;
  int ram = 0;
  string ramline;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, ramline)) {
      std::istringstream linestream(ramline);
      linestream >> ramusage >> ram;
      if (ramusage == "VmSize:") {
        break;
      }
    }
  }
  return to_string(ram / 1000);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string userid;
  int uid = 0;
  string uline;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, uline)) {
      std::istringstream linestream(uline);
      linestream >> userid >> uid;
      if (userid == "Uid:") {
        break;
      }
    }
  }
  return to_string(uid);
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string expected_uid = LinuxParser::Uid(pid);
  string user, x, uid;
  string userline;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, userline)) {
      std::replace(userline.begin(), userline.end(), ':', ' ');
      std::istringstream linestream(userline);
      linestream >> user >> x >> uid;
      if (uid == expected_uid) {
        break;
      }
    }
  }
  return user;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string upline;
  string word;
  long uptime = 0;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, upline);
    std::istringstream linestream(upline);
    for (int i = 0; i < 22; i++) {
      linestream >> word;
    }
    uptime = std::stol(word);
  }

  return uptime;
}
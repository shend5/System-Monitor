#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// Return the operating system name
string LinuxParser::OperatingSystem()
{
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

// Return the system's kernel identifier
string LinuxParser::Kernel()
{
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

// Return the ids of the processes
vector<int> LinuxParser::Pids()
{
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

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization()
{
  float memTotal = 0;
  float memFree = 0;

  string line, key, value, kb;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);

  if(filestream.is_open())
  {
    while(std::getline(filestream, line))
    {
      std::istringstream linestream(line);
      while(linestream >> key >> value >> kb)
      {
        if (key == "MemTotal:")
          memTotal = stof(value);
        else if (key == "MemFree:")
          memFree = stof(value);
        else break;
      }
    }
  }
  return ((memTotal - memFree) / memTotal);
}

// Read and return the system uptime
long int LinuxParser::UpTime()
{
  string line;
  string upTime;
  string secondValue;

  int result = 0;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if(filestream.is_open())
  {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> upTime >> secondValue;
    result = stoi(upTime);
  }
  return result;
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization()
{
  string line;
  string name = "cpu";
  std::ifstream filestream(kProcDirectory + kStatFilename);
  while(std::getline(filestream, line))
  {
    if (line.compare(0, name.size(), name) == 0)
    {
      std::istringstream buf(line);
      std::istream_iterator<string> beg(buf), end;
      vector<string> values(beg, end);
      return values;
    }
  }
  return (vector<string>());
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses()
{
  string line, key, value;
  int numberOfProcesses = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open())
  {
    while(std::getline(filestream, line))
    {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes")
      {
        numberOfProcesses = stoi(value);
        break;
      }
    }
  }
  return numberOfProcesses;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses()
{
  string line, key, value;
  int numberOfProcessesRunning = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open())
  {
    while(std::getline(filestream, line))
    {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "procs_running")
      {
        return numberOfProcessesRunning = stoi(value);
      }
    }
  }
  return numberOfProcessesRunning;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid)
{ 
  string line;
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + "/" + kCmdlineFilename);
  std::getline(filestream, line);
  return line;
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid)
{
  string line, key, value, other;

  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + "/" + kStatusFilename);
  if(filestream.is_open())
  {
      while(std::getline(filestream, line))
      {
        std::istringstream linestream(line);
        linestream >> key >> value >> other;
        if(key == "VmSize:")
        {
          float result = stof(value) / 1000;
          return std::to_string(result);
        }
      }
  }
  return string(); 
}

//Read and return cpu usage of a process with id pid
long LinuxParser::CpuUtilization(int pid)
{
  string line;
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + "/" + kStatFilename);
  std::getline(filestream, line);
  std::istringstream buf(line);
  std::istream_iterator<string> beg(buf), end;
  vector<string> values(beg,end);

  //getting relevant times for calculation of active occupation of CPU for process with pid
  int utime = LinuxParser::UpTime(pid);
  float stime = stof(values[14]);
  float cutime = stof(values[15]);
  float cstime = stof(values[16]);
  float starttime = stof(values[21]);
  long int uptime = LinuxParser::UpTime();

  float freq = sysconf(_SC_CLK_TCK);
  float total_time = utime + stime + cutime + cstime;
  long int seconds = uptime - (starttime / freq);
  float result = 100 * ((total_time / freq) / seconds);

  return result;
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid)
{
  string line;
  string name = "Uid:";
  std::ifstream filstream(kProcDirectory + "/" + std::to_string(pid) + "/" + kStatusFilename);
  
  while(std::getline(filstream, line))
  {
    if (line.compare(0, name.size(), name) == 0)
    {
      std::istringstream buf(line);
      std::istream_iterator<string> beg(buf), end;
      vector<string> values(beg, end);
      return values[1];
    }
  }
  return string();
}

// Read and return the user associated with a process
string LinuxParser::User(int pid)
{
  string line;
  string userId = LinuxParser::Uid(pid);
  string name = ("x:" + userId);
  string result;
  std::ifstream filestream(kPasswordPath);

  while(std::getline(filestream, line))
  {
    if(line.find(name) != std::string::npos)
    {
      result = line.substr(0, line.find(":"));
      return result;
    }
  }
  return string();
}

// Read and return the uptime of a process
long int LinuxParser::UpTime(int pid)
{
  string line;
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + "/" + kStatFilename);
  std::getline(filestream, line);
  std::istringstream buf(line);
  std::istream_iterator<string> beg(buf), end;
  vector<string> values(beg,end);
  string result =  values[13];
  return  float(stof(result) / sysconf(_SC_CLK_TCK));
}

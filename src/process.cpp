#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// Process constructor
Process::Process(int pid)
{
    this->pid_ = pid;
    this->user_ = Process::User();
    this->command_ = Process::Command();
    this->ram_ = Process::Ram();
    this->cpu_ = Process::CpuUtilization();
    this->upTime_ = Process::UpTime();
}

// Return this process's id
int Process::Pid()
{
    return pid_;
}

// Return this process's CPU utilization
long Process::CpuUtilization()
{
    return LinuxParser::CpuUtilization(pid_);
}

//Return the command that generated this process
string Process::Command()
{
    return LinuxParser::Command(pid_);
}

// Return this process's memory utilization
string Process::Ram()
{
    return LinuxParser::Ram(pid_);
}


// Return the user (name) that generated this process
string Process::User()
{
    return LinuxParser::User(pid_);
}

// Return the age of this process (in seconds)
long int Process::UpTime()
{
    return LinuxParser::UpTime(pid_);
}

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { return a.cpu_ < cpu_; }

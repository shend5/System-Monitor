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
    Process(int pid);
    int Pid();                               
    std::string User();                      
    std::string Command();                   
    long CpuUtilization();                  
    std::string Ram();                       
    long int UpTime();                       
    bool operator<(Process const& a) const;

 private:
    int pid_;
    std::string user_;
    std::string command_;
    std::string cpu_;
    std::string ram_;
    std::string upTime_;
};

#endif
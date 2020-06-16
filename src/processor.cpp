#include "processor.h"
#include <vector>
#include <string>
#include "linux_parser.h"

float Processor::Utilization()
 {
     std::vector<std::string> cpu = LinuxParser::CpuUtilization();
     std::vector<std::string> idleVector = {cpu[4], cpu[5]};
     std::vector<std::string> activeVector = {cpu[1], cpu[2], cpu[3], cpu[6], cpu[7], cpu[8], cpu[9], cpu[10]};

     float idleTime = Processor::SysIdleCpuTime(idleVector);
     float activeTime = Processor::SysActiveCpuTime(activeVector);

     float totalTime = activeTime + idleTime;
     float result = (activeTime / totalTime);

     return result;
 }

float Processor::SysActiveCpuTime(std::vector<std::string> values)
{
    float activeCpuTime = 0;
    for (int i = 0; i < values.size(); i++)
    {
        activeCpuTime += std::stof(values[i]);
    }
    return activeCpuTime;
}

float Processor::SysIdleCpuTime(std::vector<std::string> values)
{
    return std::stof(values[0]) + std::stof(values[1]);
}
#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <vector>
#include <string>


class Processor
{
 public:
    float Utilization();

private:
    float SysActiveCpuTime(std::vector<std::string> values);
    float SysIdleCpuTime(std::vector<std::string> values);
};

#endif
#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Return a container composed of the system's processes
// The class process are firstly initialised with the values of pid and utilization in order to keep the
// size of the clas small and the sort-calculation can work quicklier than with the class of all the atribute initialised.
// Then the rest attributes will be filled in with values(user_, command_, etc.)
vector<Process>& System::Processes() 
{
    pids_=LinuxParser::Pids();
    processes_={};
    for(const int& pid:pids_)
    {
        Process tmp(pid);
        processes_.emplace_back(tmp);
    }
    std::sort(processes_.begin(),processes_.end(),[](const Process & a, const Process & b){return b<a;});
    for(Process& process:processes_)
    {
        process.SetAll();
    }
    return processes_;
}

// Return the system's kernel identifier (string)
std::string System::Kernel() { return kernel_; }

// Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Return the operating system name
std::string System::OperatingSystem() { return os_; }

// Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }
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
  Process(const int& pid):pid_(pid)
  {
    cpu_uti_=LinuxParser::CpuUtilization(pid);
  }
  int Pid();                               
  std::string User();                      
  std::string Command();                   
  float CpuUtilization();                  
  std::string Ram();                       
  long int UpTime();                      
  bool operator<(Process const& a) const;  
  void SetAll();

  // Declare any necessary private members
 private:
   int pid_;
   float cpu_uti_;
   std::string user_;
   std::string command_;
   std::string ram_;
   long int uptime_;
   
};

#endif
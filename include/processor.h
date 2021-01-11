#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <fstream>
#include <sstream>
#include <iostream>
#include <chrono>
#include <thread>
#include "linux_parser.h"
class Processor {
 public:
 float Utilization();  

 private:
 int PrevTotal[2]{0,0};
 int Total[2]{0,0};
};

#endif
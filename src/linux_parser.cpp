#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <cmath>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() 
{
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) 
    {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "PRETTY_NAME") 
      {
        std::replace(value.begin(), value.end(), '_', ' ');
        return value;
      }
      
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
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

// BONUS: Update this to use std::filesystem
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

//Read and return the system memory utilization
float LinuxParser::MemoryUtilization() 
{ 
  string line;
  float percent{1};
  std::ifstream stream(kProcDirectory+kMeminfoFilename);
  if(stream.is_open())
  {
    string key;
    int value;
    while(std::getline(stream,line)){
      std::istringstream linestream(line);
      linestream>>key>>value;
      if(key=="MemTotal:")
      {
        percent/=value;
      }
      else if(key=="MemFree:")
      {
        percent*=value;
        return 1-percent;;
      }
    }
    

  }
  return 1-percent;
 }

// Read and return the system uptime
long LinuxParser::UpTime() 
{
  long int uptime;
  string line;
  std::ifstream stream(kProcDirectory+kUptimeFilename);
  if(stream.is_open())
  {
    std::getline(stream,line);
    std::istringstream linestream(line);
    linestream>>uptime;
  }
  return uptime;

}

// It is about to calculate the Idlejeffies and Totaljeffies of the cpu 
// The result will be saved in an input array with the format {Idle, Total}
void LinuxParser::Jiffies(int* total)
{
  std::string line;
  std::ifstream FIlestream(kProcDirectory+kStatFilename);
  std::getline(FIlestream,line);
  std::istringstream linestream(line);
  int idle{0};
  int nonidle{0};
  int tmp{0};
  int count{0};
  std::string cpu;
  linestream>>cpu;
  while(linestream>>tmp && count<8)
  {
      if(count==3 || count==4)
      {
          idle+=tmp;
      }
      else
      {
          nonidle+=tmp;
      }
      count++;
  }
  total[0]=nonidle;
  total[1]=idle+nonidle;  
}

// Read and return CPU utilization of every process according to its pid
float LinuxParser::CpuUtilization(const int& pid_)
{
  string line;
  float percent;
  std::ifstream FileStream(kProcDirectory+std::to_string(pid_)+kStatFilename);
  if(FileStream.is_open())
  {
    std::getline(FileStream,line);
    int key;
    int utime;
    int stime;
    int cutime;
    int cstime;
    string cpptool;
    string s;
    std::istringstream linestream(line);
    int count{0};
    linestream>>key>>cpptool>>s;
    while(linestream>>key)
    {
      if(count==10)
      {
        utime=key;
          
      }
      else if (count==11)
      {
        stime=key;
      }
      else if (count==12)
      {
        cutime=key;
      }
      else if (count==13)
      {
        cstime=key;
        percent=1.0f*(utime+stime+cutime+cstime)/sysconf(_SC_CLK_TCK)/(UpTime()-UpTime(pid_));
          
        return percent;
      }
      count++;
    }
  }
  return percent;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() 
{ 
  int total_Pro{0};
  string line;
  string key;
  std::ifstream stream(kProcDirectory+kStatFilename);
  if(stream.is_open())
  {
    while(std::getline(stream,line))
    {
      std::istringstream linestream(line);
      linestream>>key>>total_Pro;
      if(key=="processes")
      {
        return total_Pro;
      }


    }
  }
  return total_Pro;

}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() 
{
  int run_Pro{0};
  string line;
  string key;
  std::ifstream stream(kProcDirectory+kStatFilename);
  if(stream.is_open())
  {
    while(std::getline(stream,line))
    {
      std::istringstream linestream(line);
      linestream>>key>>run_Pro;
      if(key=="procs_running")
      {
        return run_Pro;
      }
    }
  }
  return run_Pro;
}

// Read and return the command associated with a process
string LinuxParser::Command(const int& pid) 
{
  string line;
  
  std::ifstream FileStream(kProcDirectory+std::to_string(pid)+kCmdlineFilename);
  if(FileStream.is_open())
  {
    std::getline(FileStream,line);  
  }
  return line;
}

// Read and return the memory used by a process
string LinuxParser::Ram(const int& pid) 
{
  string line;
  std::ifstream FileStream(kProcDirectory+std::to_string(pid)+kStatusFilename);
  if(FileStream.is_open())
  {
    string key;
    float ram;
    while(std::getline(FileStream,line))
    {
      std::istringstream linestream(line);
      linestream>>key>>ram;
      if(key=="VmSize:")
      {
        
        float ram_Mb=ram/1024;
        return std::to_string(ram_Mb).substr(0,7);
      }
    }
  }
  return string();

}

// Read and return the user ID associated with a process

string LinuxParser::Uid(const int& pid) 
{
  string line;
  string uid;
  std::ifstream FileStream(kProcDirectory+std::to_string(pid)+kStatusFilename);
  if(FileStream.is_open())
  {
    string key;
    while(std::getline(FileStream,line))
    {
      std::istringstream linestream(line);
      linestream>>key>>uid;
      if(key=="Uid:")
      {
        return uid;
      }
    }
  }
  return uid;
}

// Read and return the user associated with a process
string LinuxParser::User(const int& pid) 
{
  string line;
  string user;
  std::ifstream FileStream(kPasswordPath);
  if(FileStream.is_open())
  {
    string id;
    string uid=Uid(pid);
    string key;
    while(std::getline(FileStream,line))
    {
      
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream>>user>>key>>id;
      if(id==uid)
      {
        return user;
      }
    }
  }
  return user;

}

// Read and return the uptime of a process
// The calculation relys on the data from the file with path /proc/pid/stat
// THe 22th data is the starttime, which is used here
long LinuxParser::UpTime(const int& pid) 
{
  string line;
  long int time;
  std::ifstream FileStream(kProcDirectory+std::to_string(pid)+kStatFilename);
  if(FileStream.is_open())
  {
    string key;
    string s;
    std::getline(FileStream,line);
    std::istringstream linestream(line);
    int count{0};
    linestream>>time>>key>>s;
    while(linestream>>time)
    {
      
      if(count==18)
      {
        time/=sysconf(_SC_CLK_TCK);
        return time;
      }
      count++;
    }
  }
  return time;
}
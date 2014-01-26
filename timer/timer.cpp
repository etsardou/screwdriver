#include "timer.h"

std::map<std::string,double> Timer::times = std::map<std::string,double>();

std::map<std::string,unsigned long> Timer::count = 
  std::map<std::string,unsigned long>();
  
struct timeval Timer::msTime = timeval();

void Timer::printMsInternal(double t)
{
  std::cout << t << " ms\n";
}

void Timer::printSecInternal(double t)
{
  std::cout << t / 1000.0 << " sec\n";
}

void Timer::printMinInternal(double t)
{
  std::cout << t / 1000.0 / 60.0 << " minutes\n";
}

void Timer::printHoursInternal(double t)
{
  std::cout << t / 1000.0 / 60.0 / 60.0 << " hours\n";
}

void Timer::printLiteralInternal(double t)
{
  int sec = t / 1000.0;
  if(sec >= 1)
  {
    t -= sec * 1000;
    int min = sec / 60.0;
    if(min >= 1)
    {
      sec -= min * 60;
      int hours = min / 60.0;
      if(hours >= 1)
      {
        min -= hours * 60;
        std::cout << min << " hours " <<hours << " minutes " << sec 
          << " sec " << t << " ms\n";
      }
      else
      {	// Mins
        std::cout << min << " minutes " << sec << " sec " << t << " ms\n";
      }
    }
    else
    {	// Sec
      std::cout << sec << " sec " << t << " ms\n";
    }
  }
  else
  {	// Ms
    std::cout << t << " ms\n";
  }
}

void Timer::start(std::string timerId)
{  
  MsdIt it = times.find(timerId);
  if(it == times.end())
  {
    gettimeofday(&msTime, NULL);
    double ms = (double)msTime.tv_sec * 1000 + (double)msTime.tv_usec / 1000 ;
    times.insert(Psd(timerId,ms));
    count.insert(Psul(timerId,0));
  }
  else
  {
    gettimeofday(&msTime, NULL);
    double ms = (double)msTime.tv_sec * 1000 + (double)msTime.tv_usec / 1000 ;
    it->second = ms;
    count[timerId] = 0;
  }
}

double Timer::stop(std::string timerId)
{  
  MsdIt it = times.find(timerId);
  if(it == times.end())
  {
    std::cout << "Invalid timer id : " << timerId << "\n";
    return -1;
  }
  else
  {
    gettimeofday(&msTime , NULL);
    double ms = (double)msTime.tv_sec * 1000 + (double)msTime.tv_usec / 1000 ;
    return ms - it->second;
  }
}

double Timer::mean(std::string timerId)
{  
  MsdIt it = times.find(timerId);
  if(it == times.end())
  {
    std::cout << "Invalid timer id : " << timerId << "\n";
    return -1;
  }
  else
  {
    gettimeofday(&msTime , NULL);
    double ms = (double)msTime.tv_sec * 1000 + (double)msTime.tv_usec / 1000 ;
    return (ms - it->second) / count[timerId];
  }
}

void Timer::tick(std::string timerId)
{  
  MsdIt it = times.find(timerId);
  if(it == times.end())
  {
    std::cout << "Invalid timer id : " << timerId << "\n";
  }
  else
  {
    count[timerId]++;
  }
}

void Timer::printMs(std::string timerId)
{
  std::cout << "Timer " << timerId << " : ";
  printMsInternal(stop(timerId));
}

void Timer::printSec(std::string timerId)
{
  std::cout << "Timer " << timerId << " : ";
  printSecInternal(stop(timerId));
}

void Timer::printMin(std::string timerId)
{
  std::cout << "Timer " << timerId << " : ";
  printMinInternal(stop(timerId));
}

void Timer::printHours(std::string timerId)
{
  std::cout << "Timer " << timerId << " : ";
  printHoursInternal(stop(timerId));
}

void Timer::printLiteral(std::string timerId)
{
  std::cout << "Timer " << timerId << " : ";
  printLiteralInternal(stop(timerId));
}

void Timer::printLiteralMean(std::string timerId)
{
  MsdIt it = times.find(timerId);
  if(it == times.end())
  {
    std::cout << "Invalid timer id : " << timerId << "\n";
  }
  else
  {
    std::cout << "Timer " << timerId << " mean time : ";
    gettimeofday(&msTime , NULL);
    double ms = (double)msTime.tv_sec * 1000 + (double)msTime.tv_usec / 1000 ;
    printLiteralInternal((ms - it->second) / count[timerId]);
  }
}

void Timer::printAll(void)
{
  std::cout << "Timers available :\n";
  for(MsdCIt it = times.begin() ; it != times.end() ; it++)
  {
    std::cout << "\t";
    printLiteral(it->first);
  }
}

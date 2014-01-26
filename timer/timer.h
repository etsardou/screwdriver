#ifndef CLOCKPLUSPLUC_HPP
#define CLOCKPLUSPLUC_HPP

#include <iostream>
#include <cstdlib>
#include <string>
#include <map>
#include <sys/time.h>

class Timer{
  typedef std::map<std::string,double> Msd;
  typedef std::map<std::string,unsigned long> Msul;
  typedef std::map<std::string,double>::iterator MsdIt;
  typedef std::map<std::string,double>::const_iterator MsdCIt;
  typedef std::pair<std::string,double> Psd;
  typedef std::pair<std::string,unsigned long> Psul;
  
  static Msd times;
  static Msul count;
  static struct timeval msTime;

  Timer(void){}
  static void printMsInternal(double t);
  static void printSecInternal(double t);
  static void printMinInternal(double t);
  static void printHoursInternal(double t);
  static void printLiteralInternal(double t);
public:
  static void start(std::string timerId);
  static double stop(std::string timerId);
  static double mean(std::string timerId);
  static void tick(std::string timerId);
  static void printMs(std::string timerId);
  static void printSec(std::string timerId);
  static void printMin(std::string timerId);
  static void printHours(std::string timerId);
  static void printLiteral(std::string timerId);
  static void printLiteralMean(std::string timerId);
  static void printAll(void);
};



#endif

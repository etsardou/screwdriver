/***************************************************************************
 *   Copyright (C) 2011 by Tsardoulias Emmanouil                           *
 *   Author: Tsardoulias Emmanouil  <etsardou@gmail.com>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef CLOCKPLUSPLUC_HPP
#define CLOCKPLUSPLUC_HPP

#include <iostream>
#include <cstdlib>
#include <string>
#include <map>
#include <sys/time.h>

class Timer{
	static std::map<int,double> times;
	static std::map<int,double> countInt;
	static std::map<int,double>::iterator it;
	static std::map<std::string,double> timesStr;
	static std::map<std::string,double> countString;
	static std::map<std::string,double>::iterator itStr;
	//static std::map<int,double>
	static struct timeval msTime;
	static void printMsInternal(double t){
		std::cout<<t<<" ms\n";
	}
	static void printSecInternal(double t){
		std::cout<<t/1000.0<<" sec\n";
	}
	static void printMinInternal(double t){
		std::cout<<t/1000.0/60.0<<" minutes\n";
	}
	static void printHoursInternal(double t){
		std::cout<<t/1000.0/60.0/60.0<<" hours\n";
	}
	static void printLiteralInternal(double t){
		int sec=t/1000.0;
		if(sec>=1){
			t-=sec*1000;
			int min=sec/60.0;
			if(min>=1){
				sec-=min*60;
				int hours=min/60.0;
				if(hours>=1){
					min-=hours*60;
					std::cout<<min<<" hours "<<hours<<" minutes "<<sec<<" sec "<<t<<" ms\n";
				}
				else{	// Mins
					std::cout<<min<<" minutes "<<sec<<" sec "<<t<<" ms\n";
				}
			}
			else{	// Sec
				std::cout<<sec<<" sec "<<t<<" ms\n";
			}
		}
		else{	// Ms
			std::cout<<t<<" ms\n";
		}
	}
public:
	static void start(int timerId){
		
		it=times.find(timerId);
		if(it==times.end()){
			gettimeofday(&msTime , NULL);
			double ms=(double)msTime.tv_sec * 1000 + (double)msTime.tv_usec / 1000 ;
			times.insert(std::pair<int,double>(timerId,ms));
		}
		else
		{
			gettimeofday(&msTime , NULL);
			double ms=(double)msTime.tv_sec * 1000 + (double)msTime.tv_usec / 1000 ;
			it->second=ms;
		}
	}
	static void start(std::string timerId){
		
		itStr=timesStr.find(timerId);
		if(itStr==timesStr.end()){
			gettimeofday(&msTime , NULL);
			double ms=(double)msTime.tv_sec * 1000 + (double)msTime.tv_usec / 1000 ;
			timesStr.insert(std::pair<std::string,double>(timerId,ms));
		}
		else{
			gettimeofday(&msTime , NULL);
			double ms=(double)msTime.tv_sec * 1000 + (double)msTime.tv_usec / 1000 ;
			itStr->second=ms;
		}
	}
	static double stop(int timerId){
		
		it=times.find(timerId);
		if(it==times.end()){
			std::cout<<"Invalid timer id : "<<timerId<<"\n";
			return -1;
		}
		else{
			gettimeofday(&msTime , NULL);
			double ms=(double)msTime.tv_sec * 1000 + (double)msTime.tv_usec / 1000 ;
			return ms-it->second;
		}
	}
	static double stop(std::string timerId){
		
		itStr=timesStr.find(timerId);
		if(itStr==timesStr.end()){
			std::cout<<"Invalid timer id : "<<timerId<<"\n";
			return -1;
		}
		else{
			gettimeofday(&msTime , NULL);
			double ms=(double)msTime.tv_sec * 1000 + (double)msTime.tv_usec / 1000 ;
			return ms-itStr->second;
		}
	}
	static void printMs(int timerId){
		std::cout<<"Timer "<<timerId<<" : ";
		printMsInternal(stop(timerId));
	}
	static void printMs(std::string timerId){
		std::cout<<"Timer "<<timerId<<" : ";
		printMsInternal(stop(timerId));
	}
	
	static void printSec(int timerId){
		std::cout<<"Timer "<<timerId<<" : ";
		printSecInternal(stop(timerId));
	}
	static void printSec(std::string timerId){
		std::cout<<"Timer "<<timerId<<" : ";
		printSecInternal(stop(timerId));
	}
	
	static void printMin(int timerId){
		std::cout<<"Timer "<<timerId<<" : ";
		printMinInternal(stop(timerId));
	}
	static void printMin(std::string timerId){
		std::cout<<"Timer "<<timerId<<" : ";
		printMinInternal(stop(timerId));
	}
	
	static void printHours(int timerId){
		std::cout<<"Timer "<<timerId<<" : ";
		printHoursInternal(stop(timerId));
	}
	static void printHours(std::string timerId){
		std::cout<<"Timer "<<timerId<<" : ";
		printHoursInternal(stop(timerId));
	}
	
	static void printLiteral(int timerId){
		std::cout<<"Timer "<<timerId<<" : ";
		printLiteralInternal(stop(timerId));
	}
	static void printLiteral(std::string timerId){
		std::cout<<"Timer "<<timerId<<" : ";
		printLiteralInternal(stop(timerId));
	}
	static void printAll(void){
		std::cout<<"Timers available :\n";
		for(std::map<int,double>::const_iterator it=times.begin();it!=times.end();it++){
			std::cout<<"\t";
			printLiteral(it->first);
		}
		for(std::map<std::string,double>::const_iterator it=timesStr.begin();it!=timesStr.end();it++){
			std::cout<<"\t";
			printLiteral(it->first);
		}
	}
};

std::map<int,double> Timer::times=std::map<int,double>();
std::map<int,double>::iterator Timer::it=std::map<int,double>::iterator();
std::map<std::string,double> Timer::timesStr=std::map<std::string,double>();
std::map<std::string,double>::iterator Timer::itStr=std::map<std::string,double>::iterator();
struct timeval Timer::msTime=timeval();

#endif

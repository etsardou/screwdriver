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

#ifndef SCREWDRIVER_FILE_HPP
#define SCREWDRIVER_FILE_HPP

#include <iostream>
#include <cstdlib>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>

struct Data{
	std::vector<std::vector<std::string> > d;
	
	friend std::ostream& operator<<(std::ostream& output,const Data & p);
	
	template<typename type>
	std::vector<std::vector<type> > cast(void){
		type temp;
		std::vector<std::vector<type> > ret;
		for(unsigned int i=0;i<d.size();i++){
			ret.push_back(std::vector<type>());
			for(unsigned int j=0;j<d[i].size();j++){
				std::istringstream snum(d[i][j]);
				snum>>temp;
				ret[ret.size()-1].push_back(temp);
			}
		}
		return ret;
	}
	
	std::vector<std::string> row(unsigned int i){
		std::vector<std::string> ret;
		return ret;
	}
	
	std::vector<std::string> column(unsigned int i){
		std::vector<std::string> ret;
		return ret;
	}
	
	std::vector<std::string> fixVector1D(){
		std::vector<std::string> ret;
		return ret;
	}
	
	std::vector<std::vector<std::vector<std::string> > > fixVector3D(){
		std::vector<std::vector<std::vector<std::string> > > ret;
		return ret;
	}
	
	std::vector<std::string> & operator[] (unsigned int i){
		return d[i];
	}
	
	unsigned int size(void){
		return d.size();
	}
};

std::ostream & operator<<(std::ostream& output,const Data& p){
	for(unsigned int i=0;i<p.d.size();i++){
		output<<"\nLine "<<i<<" :\n";
		for(unsigned int j=0;j<p.d[i].size();j++)
			output<<"\t"<<p.d[i][j];
	}
	output<<"\n";
	return output;
}

class File{
	public:
		static std::ifstream *file;
		static char charLine[5000];
		static Data data;
		
		static Data readData(std::string filename,char delimiter=','){
			data.d.clear();
			file=new std::ifstream (filename.c_str(),std::ifstream::in);
			while(!file->eof()){
				file->getline(charLine,5000);
				std::string tempLine=std::string(charLine),da;
				if(tempLine=="")
					continue;
				data.d.push_back(std::vector<std::string>());
				unsigned int counter=0;
				while (counter!=tempLine.size()){
					da.clear();
					unsigned char c=delimiter+1;
					do{
						c=tempLine[counter];
						da+=c;
						counter++;
					}while(c!=delimiter && counter<tempLine.size());
					if(da[da.size()-1]==delimiter)
						da.erase(da.size()-1,1);
					data.d[data.d.size()-1].push_back(da);
				}
			}
			file->close();
			delete file;
			return data;
		}
};

std::ifstream *File::file;
Data File::data;
char File::charLine[5000];
#endif

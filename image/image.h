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

#ifndef SCREWDRIVER_IMAGE_HPP
#define SCREWDRIVER_IMAGE_HPP

#include <iostream>
#include <cstdlib>
#include <string>
#include <map>
#include <set>
#include <cv.h>
#include <highgui.h>

using namespace std;

struct RGB{
	static int R,G,B;
	static CvScalar color(int r,int g,int b){
		CvScalar s;
		s.val[0]=b;
		s.val[1]=g;
		s.val[2]=r;
		return s;
	}	
};

int RGB::R;
int RGB::G;
int RGB::B;

class Image{
	private:
		static map<string,IplImage *> images;
	public:
		static void loadImage(string ID,string fileName,bool grayscale=false){
			int depth=(!grayscale)*3;
			images.insert(pair<string,IplImage *>(ID,cvLoadImage(fileName.c_str(),depth)));
		}
		static void createImage(string ID,int width,int height,RGB fill){
			
		}
		static void show(string ID,int usec){
			cvNamedWindow(ID.c_str());
			cvShowImage( ID.c_str(), images.find(ID)->second );
			cvWaitKey(usec);
			cvDestroyWindow(ID.c_str());
		}
		static void destroy(string ID){
			cvReleaseImage(&images.find(ID)->second);
			images.erase(ID);
		}
		static pair<int,int> size(string ID){
			return pair<int,int>(images.find(ID)->second->width,images.find(ID)->second->height);
		}
		static void info(string ID){
			cout<<"Image \'"<<ID<<"\' :\n";
			cout<<"\tWidth = "<<images.find(ID)->second->width<<" px\n";
			cout<<"\tHeight = "<<images.find(ID)->second->height<<" px\n";
			cout<<"\tChannels = "<<images.find(ID)->second->nChannels<<" \n";
			cout<<"\tPixel depth = "<<images.find(ID)->second->depth<<" \n";
		}
		static void filter(string ID,std::vector< std::vector<float> > mask){
			unsigned int N=mask.size();
			
			if(N%2==0){
				cout<<"Filter size not odd\n";
				return;
			}
			
			for(unsigned int i=0;i<mask.size();i++)
				if(mask[i].size()!=N){
					cout<<"Mask not NxN\n";
					return;
				}
			IplImage* temp=images.find(ID)->second;	
			float ** im;
			im=new float *[temp->width];
			for(int i=0;i<temp->width;i++)
				im[i]=new float[temp->height];
				
			int step       = temp->widthStep/sizeof(uchar);
			uchar* data    = (uchar *)temp->imageData;	
				
			for(unsigned int i=N/2+1;i<temp->height-N/2-1;i++){
				for(unsigned int j=N/2+1;j<temp->width-N/2-1;j++){
					im[i][j]=0;
					for(unsigned int m=0;m<N;m++)
						for(unsigned int n=0;n<N;n++)
							im[i][j]+=mask[m][n]*data[(i+m-1)*step+(j+n-1)];
				}	
			}
			
			CvScalar pixel;
			for(int i=0;i<temp->height;i++)
				for(int j=0;j<temp->width;j++){
					pixel.val[0]=im[i][j];
					cvSet2D(temp,i,j, pixel);
				}
				
			for(int i=0;i<temp->width;i++)
				delete [] im[i];
			delete [] im;
		}
		static void meanFilter(string ID){
			vector<vector<float> > mask(3);
			mask[0]=vector<float>(3);
				mask[0][0]=1.0/9.0;
				mask[0][1]=1.0/9.0;
				mask[0][2]=1.0/9.0;
			mask[1]=vector<float>(3);
				mask[1][0]=1.0/9.0;
				mask[1][1]=1.0/9.0;
				mask[1][2]=1.0/9.0;
			mask[2]=vector<float>(3);
				mask[2][0]=1.0/9.0;
				mask[2][1]=1.0/9.0;
				mask[2][2]=1.0/9.0;
			Image::filter(ID,mask);
		}
		static void highpassFilter(string ID){
			vector<vector<float> > mask(3);
			mask[0]=vector<float>(3);
				mask[0][0]=-1.0;
				mask[0][1]=-1.0;
				mask[0][2]=-1.0;
			mask[1]=vector<float>(3);
				mask[1][0]=-1.0;
				mask[1][1]=8.0;
				mask[1][2]=-1.0;
			mask[2]=vector<float>(3);
				mask[2][0]=-1.0;
				mask[2][1]=-1.0;
				mask[2][2]=-1.0;
			Image::filter(ID,mask);
		}
		static void clarityFilter(string ID){
			vector<vector<float> > mask(3);
			mask[0]=vector<float>(3);
				mask[0][0]=0;
				mask[0][1]=-1.0;
				mask[0][2]=0;
			mask[1]=vector<float>(3);
				mask[1][0]=-1.0;
				mask[1][1]=5.0;
				mask[1][2]=-1.0;
			mask[2]=vector<float>(3);
				mask[2][0]=0;
				mask[2][1]=-1.0;
				mask[2][2]=0;
			Image::filter(ID,mask);
		}
		static std::vector<int> histogram(string ID){
			std::vector<int> ret(256);
			IplImage* temp=images.find(ID)->second;	
			int step       = temp->widthStep/sizeof(uchar);
			uchar* data    = (uchar *)temp->imageData;
			for(int i=0;i<temp->height;i++)
				for(int j=0;j<temp->width;j++)
					ret[data[(i)*step+(j)]]+=1;
			return ret;
		}
		static void histogramTrans(string ID,string trans,float p=1){
			//std::vector<int> ret(256);
			//ret=histogram(ID);
			IplImage* temp=images.find(ID)->second;	
			int step       = temp->widthStep/sizeof(uchar);
			uchar* data    = (uchar *)temp->imageData;
			if(trans=="negative"){
				for(int i=0;i<temp->height;i++)
					for(int j=0;j<temp->width;j++)
						data[(i)*step+(j)]=255-data[(i)*step+(j)];
			}
			if(trans=="highContrast"){
				for(int i=0;i<temp->height;i++)
					for(int j=0;j<temp->width;j++){
						if(data[(i)*step+(j)]<80) data[(i)*step+(j)]=0;
						else if(data[(i)*step+(j)]>190) data[(i)*step+(j)]=255;
						else data[(i)*step+(j)]=255.0*(data[(i)*step+(j)]-80)/110.0;
					}
			}
			if(trans=="power"){
				for(int i=0;i<temp->height;i++)
					for(int j=0;j<temp->width;j++){
						float t=255*pow(data[(i)*step+(j)]/255.0,p);
						if(t>255) t=255;
						data[(i)*step+(j)]=t;
					}
			}
			if(trans=="log"){
				float b=255.0/log(1+255.5*p);
				for(int i=0;i<temp->height;i++)
					for(int j=0;j<temp->width;j++){
						float t=b*log(1.0+p*data[(i)*step+(j)]);
						data[(i)*step+(j)]=t;
					}
			}
			if(trans=="exp"){
				float b=255.0/log(1+255.5*p);
				for(int i=0;i<temp->height;i++)
					for(int j=0;j<temp->width;j++){
						float t=1.0/p*(exp(data[(i)*step+(j)]/b)-1);
						data[(i)*step+(j)]=t;
					}
			}
		}
		
};

map<string,IplImage*> Image::images;

#endif

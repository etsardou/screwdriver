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

#ifndef GRAPHPLUS_HPP
#define GRAPHPLUS_HPP

#include <iostream>
#include <cstdlib>
#include <string>
#include <map>
#include <sys/time.h>
#include <cmath>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <set>

class Node{

	
public:
	unsigned int ID;
	std::string name;
	std::vector<unsigned int> neighIds;
	std::vector<float> neighWeights;
	
	Node(unsigned int ID){
		this->ID=ID;
		{
			std::string s;
			std::stringstream out;
			out << ID;
			name = out.str();
		}
	}
	Node(unsigned int ID,std::string name){
		this->ID=ID;
		this->name=name;
	}
	unsigned int getID(){
		return ID;
	}
	std::string getName(){
		return name;
	}
	std::vector<unsigned int> getNeighbors(){
		return neighIds;
	} 
	std::vector<float> getNeighborWeights(){
		return neighWeights;
	} 
	bool makeNeighbor(unsigned int neighId,float weight=0,bool allowDoubles=true){
		if(allowDoubles==false){
			if (this->ID==neighId)
				return false;
			for(unsigned int i=0;i<neighIds.size();i++)
				if(neighId==neighIds[i])
					return false;
		}
		neighIds.push_back(neighId);
		neighWeights.push_back(weight);
		return true;
	}
	bool clearNeighbor(unsigned int ID){
		for(unsigned int i=0;i<neighIds.size();i++){
			if(neighIds[i]==ID){
				neighIds.erase(neighIds.begin()+i);
				neighWeights.erase(neighWeights.begin()+i);
				return true;
			}
		}
		return false;
	}
	void clearNeighbors(){
		neighIds.clear();
		neighWeights.clear();
	}
	bool hasNeighbor(unsigned int ID){
		for(unsigned int i=0;i<neighIds.size();i++)
			if(neighIds[i]==ID)
				return true;
		return false;
	}
	unsigned int getNeighborNumber(unsigned int id){
		unsigned int ret=0;
		for(unsigned int i=0;i<neighIds.size();i++){
			if(neighIds[i]==id)
				ret++;
		}
		return ret;
	}
	bool checkLoop(std::vector<Node> nodes,std::set<unsigned int> &visited,int father){
		bool result=false;
		for(unsigned int i=0;i<neighIds.size();i++){
			if(neighIds[i]!=(unsigned int)father){
				if(visited.find(neighIds[i])!=visited.end())
					return true;
				else{
					visited.insert(neighIds[i]);
					result|=nodes[neighIds[i]].checkLoop(nodes,visited,this->ID);
				}
			}
		}
		return result;
	}
};

class Graph{
	std::vector<Node> nodes;
	unsigned int nodeById(unsigned int ID){
		for(unsigned int i=0;i<nodes.size();i++)
			if(nodes[i].getID()==ID)
				return i;
		return 0;
	}
public:
	Graph(void){
		srand(time(0));
	}
	void clearNodes(){
		nodes.clear();
	}
	void addNode(){
		int tempId=nodes.size();
		nodes.push_back(Node(tempId));
	}
	void addNode(unsigned int ID,std::string name=""){
		nodes.push_back(Node(ID,name));
	}
	void makeNeighbor(unsigned int id1,unsigned int id2, float weight=0){
		for(unsigned int i=0;i<nodes.size();i++){
			if(nodes[i].getID()==id1){
				nodes[i].neighIds.push_back(id2);
				nodes[i].neighWeights.push_back(weight);
				break;
			}
		}
		if(id1==id2) 
			return;
		for(unsigned int i=0;i<nodes.size();i++){
			if(nodes[i].getID()==id2){
				nodes[i].neighIds.push_back(id1);
				nodes[i].neighWeights.push_back(weight);
				break;
			}
		}
	}
	void makeRandomNeighbor(unsigned int ID){
		unsigned int N=nodes.size();
		makeNeighbor(ID,rand()%N,(rand()%1000)/10000.0);
	}
	void makeRandomNeighbor(bool allowSelf=true){
		unsigned int N=nodes.size();
		if(N<2) 
			return;
		unsigned int a=rand()%N,b=rand()%N;
		if(!allowSelf){
			while(a==b){
				a=rand()%N;
				b=rand()%N;
			}
		}	
		makeNeighbor(a,b,(rand()%1000)/10000.0);
	}
	void makeFullConnected(void){
		for(unsigned int i=0;i<nodes.size();i++)
			for(unsigned int j=i+1;j<nodes.size();j++)
				makeNeighbor(i,j,(rand()%1000)/10000.0);
	}
	void createDotFile(bool byName=false){
		ofstream myfile;
		myfile.open ("file.dot");
		myfile << "graph Output {\n";
		for(unsigned int i=0;i<nodes.size();i++){
			std::string first;
			std::string second;
			if(byName)
				first=nodes[i].getName();
			else{
				std::string s;
				std::stringstream out;
				out << nodes[i].getID();
				first = out.str();
			}
			if(nodes[i].neighIds.size()==0){
				myfile<<"\t"<<first<<"\n";
			}
			else{
				for(unsigned int j=0;j<nodes[i].neighIds.size();j++){
					if(nodes[i].getID()>nodes[i].neighIds[j]) continue;
					if(byName)
						second=nodes[nodes[i].neighIds[j]].getName();
					else{
						std::string s;
						std::stringstream out;
						out << nodes[nodes[i].neighIds[j]].getID();
						second = out.str();
					}
					std::string weightStr;
					{
						std::string s;
						std::stringstream out;
						out << nodes[i].neighWeights[j];
						weightStr = out.str();
					}
					myfile<<"\t"<<first<<" -- "<<second<<" [label=\""<<weightStr<<"\" ,fontsize=10]\n";
				}
			}
		}
		myfile << "}\n";
		myfile.close();
	}
	int drawGraph(std::string filename,bool byName=false){
		createDotFile(byName);
		return system ((std::string("dot -Tpng file.dot -o ")+filename).c_str());
	}
	void clearNeighbors(unsigned int ID){
		for(unsigned int i=0;i<nodes.size();i++){
			if(ID==nodes[i].getID())
				nodes[i].clearNeighbors();
		}
	}
	void clearAllNeighbors(){
		for(unsigned int i=0;i<nodes.size();i++){
			nodes[i].clearNeighbors();
		}
	}
	void clearNeighbor(unsigned int ID,unsigned int neighId){
		for(unsigned int i=0;i<nodes.size();i++){
			if(ID==nodes[i].getID())
				nodes[i].clearNeighbor(neighId);
		}
	}
	unsigned int getNodeNumber(){
		return nodes.size();
	}
	unsigned int getNodeNeighborNumber(unsigned int ID){
		return nodes[nodeById(ID)].neighIds.size();
	}
	unsigned int getEdgeNumber(){
		unsigned int num=0;
		for(unsigned int i=0;i<nodes.size();i++){
			num+=getNodeNeighborNumber(nodes[i].ID);
			if(nodes[i].hasNeighbor(nodes[i].ID))
						num+=nodes[i].getNeighborNumber(nodes[i].ID);					
		}
		return num/2;
	}
	bool hasLoop(bool selfConnectionsCheck=true){
		if(selfConnectionsCheck && hasSelfConnections())
			return true;
		if(hasDoubleNeighbors()) return true;
		std::vector<std::set<unsigned int> > subs=getSubgraphs();
		bool is=false;
		std::set<unsigned int> visited;
		
		for(unsigned int i=0;i<subs.size();i++){
			visited.clear();
			is|=nodes[*subs[i].begin()].checkLoop(nodes,visited,-1);
		}
		return is;
	}
	bool hasSelfConnections(){
		for(unsigned int i=0;i<nodes.size();i++){
			for(unsigned int j=0;j<nodes[i].neighIds.size();j++)
				if(nodes[i].ID==nodes[i].neighIds[j])
					return true;
		}
		return false;
	}
	bool hasDoubleNeighbors(){
		for(unsigned int i=0;i<nodes.size();i++){
			for(unsigned int j=0;j<nodes[i].neighIds.size();j++){
				for(unsigned int k=0;k<nodes[i].neighIds.size();k++){
					if(j==k) continue;
					if(nodes[i].neighIds[k]==nodes[i].neighIds[j])
						return true;
				}
			}
		}
		return false;
	}
	bool isConnected(void){
		std::set<unsigned int> ids;
		std::set<unsigned int> checkNeighs;
		if(nodes.size()==0) 
			return true;
		checkNeighs.insert(0);
		ids.insert(0);
		while(checkNeighs.size()!=0){
			unsigned int tempId=*checkNeighs.begin();
			checkNeighs.erase(tempId);
			std::vector<unsigned int> tempIds=nodes[tempId].getNeighbors();
			for(unsigned int i=0;i<tempIds.size();i++){
				if(ids.find(tempIds[i])==ids.end()){
					checkNeighs.insert(tempIds[i]);
					ids.insert(tempIds[i]);
				}
			}
		}
		if(ids.size()==nodes.size())
			return true;
		return false;
	}
	bool isConnected(unsigned int start,unsigned int finish){
		std::set<unsigned int> ids;
		std::set<unsigned int> checkNeighs;
		if(nodes.size()==0 || start>=nodes.size() || finish>=nodes.size()) 
			return false;
		checkNeighs.insert(start);
		ids.insert(start);
		while(checkNeighs.size()!=0){
			unsigned int tempId=*checkNeighs.begin();
			checkNeighs.erase(tempId);
			std::vector<unsigned int> tempIds=nodes[tempId].getNeighbors();
			for(unsigned int i=0;i<tempIds.size();i++){
				if(ids.find(tempIds[i])==ids.end()){
					if(tempIds[i]==finish) 
						return true;
					checkNeighs.insert(tempIds[i]);
					ids.insert(tempIds[i]);
				}
			}
		}
		return false;
	}
	std::vector<std::set<unsigned int> > getSubgraphs(){
		std::set<unsigned int> ids;
		std::set<unsigned int> checkNeighs;
		std::vector<std::set<unsigned int> > ret;
		std::set<unsigned int> remainingNodes;
		for(unsigned int i=0;i<nodes.size();i++)
			remainingNodes.insert(nodes[i].ID);
		while(remainingNodes.size()!=0){
			unsigned int firstInsertion=*remainingNodes.begin();
			checkNeighs.insert(firstInsertion);
			ids.insert(firstInsertion);
			while(checkNeighs.size()!=0){
				unsigned int tempId=*checkNeighs.begin();
				checkNeighs.erase(tempId);
				std::vector<unsigned int> tempIds=nodes[tempId].getNeighbors();
				for(unsigned int i=0;i<tempIds.size();i++){
					if(ids.find(tempIds[i])==ids.end()){
						checkNeighs.insert(tempIds[i]);
						ids.insert(tempIds[i]);
					}
				}
			}
			ret.push_back(ids);
			for(std::set<unsigned int>::const_iterator it=ids.begin();it!=ids.end();it++)
				remainingNodes.erase(*it);
			ids.clear();
			checkNeighs.clear();
		}
		return ret;
	}
	std::set<unsigned int> getLargerSubgraph(){
		std::vector<std::set<unsigned int> > subs=getSubgraphs();
		unsigned int max=0;
		int maxId=-1;
		for(unsigned int i=0;i<subs.size();i++)
			if(subs[i].size()>max){
				max=subs[i].size();
				maxId=i;
			}
		return subs[maxId];
	}
	bool isRegular(){	//All nodes have the same degree
		if(nodes.size()==0)
			return false;
		unsigned int num=nodes[0].neighIds.size();
		for(unsigned int i=1;i<nodes.size();i++)
			if(num!=nodes[i].neighIds.size())
				return false;
		return true;
	}
	bool isNull(){
		for(unsigned int i=0;i<nodes.size();i++)
			if(nodes[i].neighIds.size()!=0)
				return false;
		return true;
	}
	bool isCircleGraph(){
		for(unsigned int i=0;i<nodes.size();i++)
			if(nodes[i].neighIds.size()!=2)
				return false;
		if(!hasLoop())
			return false;
		return true;
	}
	bool isTree(){
		if(isConnected() && !hasLoop())
			return true;
		return false;
	}
	bool isEulerian(){
		if(!isConnected())
			return false;
		for(unsigned int i=0;i<nodes.size();i++)
			if(nodes[i].neighIds.size()%2)
				return false;
		return true;
	}
	bool isSemiEulerian(){
		if(!isConnected())
			return false;
		unsigned int count=0;
		for(unsigned int i=0;i<nodes.size();i++)
			if(nodes[i].neighIds.size()%2){
				count++;
				if(count>2)
					return false;
			}
		if(count!=2)
			return false;
		return true;
	}
	unsigned int numberOfNodesStrength(unsigned int strength){
		unsigned int count=0;
		for(unsigned int i=0;i<nodes.size();i++)
			if(nodes[i].neighIds.size()==strength)
				count++;
		return count;
	}
	std::vector<std::vector<unsigned int> > getAdjacencyMatrix(){
		std::vector<std::vector<unsigned int> > ret;
		for(unsigned int i=0;i<nodes.size();i++){
			ret.push_back(std::vector<unsigned int>(nodes.size()));
			for(unsigned int j=0;j<nodes[i].neighIds.size();j++){
					ret[i][nodes[i].neighIds[j]]++;
			}
		}
		return ret;
	}
	bool isPathGraph(){
		if(isTree()){
			for(unsigned int i=0;i<nodes.size();i++)
				if(nodes[i].neighIds.size()>2)
					return false;
		}
		else
			return false;
		return true;
	}
	void printInfo(){
		cout<<"|------------------------------------------------------|\n";
		cout<<"| Graph info :\n";
		cout<<"| \n";
		cout<<"| Nodes number : "<<getNodeNumber()<<"\n";
		cout<<"| Edges number : "<<getEdgeNumber()<<"\n";
		//----------------------------------------------------
		cout<<"| Connections :\n";
		for(unsigned int i=0;i<nodes.size();i++){
			cout<<"|\t"<<nodes[i].ID<<" :  ";
			for(unsigned int j=0;j<nodes[i].neighIds.size();j++)
				cout<<nodes[i].neighIds[j]<<" ";
			cout<<"\n";
		}
		//----------------------------------------------------
		cout<<"| Subgraphs :\n";
		std::vector<std::set<unsigned int> > subs=getSubgraphs();
		for(unsigned int i=0;i<subs.size();i++){
			cout<<"|\t"<<i<<" : ";  
			for(std::set<unsigned int>::const_iterator it=subs[i].begin();it!=subs[i].end();it++)
				cout<<*it<<" ";
			cout<<"\n";
		}
		//----------------------------------------------------
		cout<<"| Larger subgraph : ";
		std::set<unsigned int> largeSub=getLargerSubgraph();
		for(std::set<unsigned int>::const_iterator it=largeSub.begin();it!=largeSub.end();it++)
				cout<<*it<<" ";
		cout<<"\n";
		//----------------------------------------------------
		std::string s;
		s= isConnected()?"true":"false";
		cout<<"| Is connected : "<<s<<"\n";
		s= hasLoop()?"true":"false";
		cout<<"| Has loop : "<<s<<"\n";
		s= isConnected(0,1)?"true":"false";
		cout<<"| Check <connection 0-1> : "<<s<<"\n";
		s= hasSelfConnections()?"true":"false";
		cout<<"| Has self connections : "<<s<<"\n";
		s= hasDoubleNeighbors()?"true":"false";
		cout<<"| Has double neighbors : "<<s<<"\n";
		s= isRegular()?"true":"false";
		cout<<"| Is regular : "<<s<<"\n";
		s= isNull()?"true":"false";
		cout<<"| Is null : "<<s<<"\n";
		s= isCircleGraph()?"true":"false";
		cout<<"| Is circle graph : "<<s<<"\n";
		s= isTree()?"true":"false";
		cout<<"| Is tree : "<<s<<"\n";
		s= isPathGraph()?"true":"false";
		cout<<"| Is path graph : "<<s<<"\n";
		s= isEulerian()?"true":"false";
		cout<<"| Is eulerian : "<<s<<"\n";
		s= isSemiEulerian()?"true":"false";
		cout<<"| Is semi-eulerian : "<<s<<"\n";
		
		//---------------------------------------------
		std::vector<std::vector<unsigned int> > adj=getAdjacencyMatrix();
		cout<<"| Adjacency matrix :";
		for(unsigned int i=0;i<adj.size();i++){
			cout<<"\n\t";
			for(unsigned int j=0;j<adj[i].size();j++)
				cout<<adj[i][j]<<"\t";
		}
		cout<<"\n";
		cout<<"|------------------------------------------------------|\n";
	}
};

#endif

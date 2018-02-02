/*********************************************************
  FileName     [ BotDiscover.cpp ]
  Description  [ discover bot based on anomalous windows ]
  Author       [ Cheng-Yeh (Gary) Chen ]
  Copyright    [ MIT ]
*********************************************************/
#include <map>
#include <math.h>

#include "BotDiscover.h"

using namespace std;
using Eigen::MatrixXd;

extern size_t window_num;

BotDiscover::BotDiscover()
{
	_anomalyNumber = 0;
}

BotDiscover::BotDiscover(const vector<bool>& anomaly, const vector< vector< vector<string> > >& timeList)
{
	_anomalyNumber = 0;
	_anomaly = anomaly;
	setSCG(timeList);
	cout << "Finish setSCG();\n";
	setSCG2();
}

BotDiscover::~BotDiscover()
{
	/*for(map<string, SCG_Node*>::iterator it = _anomalyList.begin(); it != _anomalyList.end(); ++it){
		if(it -> second){
			delete (it -> second);
			it -> second = 0;
		}
	}*/
}

void
BotDiscover::setSCG(const vector< vector< vector<string> > >& timeList, const double tau)
{
	// node for collect total interaction
	map<string, double> node;
	int count = 0;
	// compute total interaction
	for(size_t i = 0; i < _anomaly.size(); ++i){
		if(_anomaly[i]){
			++_anomalyNumber;
			for(size_t j = 0; j < timeList[i].size(); ++j){
				if(!node.emplace(timeList[i][j][3], 1).second){
					++node[timeList[i][j][3]];
				}
				else{
					if(!_anomalyList.emplace(timeList[i][j][3], newNode(count)).second)
						cout << "Error in BotDiscover::setTotalInteraction\n";
				}
				if(!node.emplace(timeList[i][j][6], 1).second){
					++node[timeList[i][j][6]];
				}
				else{
					if(!_anomalyList.emplace(timeList[i][j][6], newNode(count)).second)
						cout << "Error in BotDiscover::setTotalInteraction\n";
				}
				
				//(_anomalyList[timeList[i][j][3]] -> out_list)[i].insert(timeList[i][j][6]);
				//(_anomalyList[timeList[i][j][6]] -> in_list)[i].insert(timeList[i][j][3]);
			}
		}
	}
	// select pivots
	for(auto& x: node){
		//_anomalyList[x.first] -> total_interaction = x.second / _anomaly.size();
		if(x.second / _anomaly.size() > tau){
			if(_anomalyList.find(x.first) != _anomalyList.end())
				_anomalyList[x.first] -> pivot = true;
			else cout << "Error in BotDiscover:: select pivots\n";
		}
		else{
			if(_anomalyList.find(x.first) != _anomalyList.end())
				_anomalyList[x.first] -> pivot = false;
			else cout << "Error in BotDiscover:: select pivots\n";
		}
	}
	cout << "scg2\n";
	// compute interaction with pivots
	for(size_t i = 0; i < _anomaly.size(); ++i){
		if(_anomaly[i]){
			for(size_t j = 0; j < timeList[i].size(); ++j){
				if(_anomalyList[timeList[i][j][3]] -> pivot){
					(_anomalyList[timeList[i][j][3]] -> out_list)[i].push_back(timeList[i][j][6]);
					(_anomalyList[timeList[i][j][6]] -> in_list)[i].push_back(timeList[i][j][3]);
				}
				if(_anomalyList[timeList[i][j][6]] -> pivot){
					(_anomalyList[timeList[i][j][6]] -> out_list)[i].push_back(timeList[i][j][3]);
					(_anomalyList[timeList[i][j][3]] -> in_list)[i].push_back(timeList[i][j][6]);
				}
			}
		}
	}
	cout << "scg3\n";
	// remove nodes that don't appear in SCG
	vector<string> removed;
	for(auto& x: _anomalyList){
		if(mean(x.first) == 0){
			// delete SCG_Node in heap
			deleteNode(x.second);
			removed.push_back(x.first);
		}
	}
	// delete node in _anomalyList
	for(size_t i = 0; i < removed.size(); ++i){
		_anomalyList.erase(removed[i]);
	}
}
/*
void
BotDiscover::rebuild()
{
	int count = 0;
				
				}
				else{
					++node[timeList[i][j][3]];
				}
				if(node.insert(make_pair(timeList[i][j][6], 1)).second){
				
				}
				else{
					++node[timeList[i][j][6]];
				}
				
			}
		}
	}
	
	for(map<string, SCG_Node*>::iterator it = _anomalyList.begin(); it != _anomalyList.end(); ++it){
		const SCG_Node pvt = *(it -> second);
		if(pvt.pivot){
			cout << "_scgList.size() = " << _scgList.size() << endl;
			if(_scgList.size() != count){
				cout << "Error! _scgList.size() != count\n";
				return;
			}
			if( !_scgList.insert( make_pair(it -> first, newNode(true, count)) ).second ){
				--count;
				//cout << "Failed~~~~~~~~~~~~~~~~~~~~~~~Node:" << it -> first << "e" << endl;
			}
			else{
				//cout << "Succeed~~~~~~~~~~~~~~~~~~~~~~~Node:" << it -> first << "e" << endl;
			}
			
			for(size_t i = 0; i < pvt.out_list.size(); ++i){
				for(set<string>::iterator it1 = pvt.out_list[i].begin(); it1 != pvt.out_list[i].end(); ++ it1){
					if(degreeOneFilter(*it1))continue;
					if(_scgList.find(*it1) == _scgList.end()){
						if( !_scgList.insert( make_pair(*it1, newNode(false, count)) ).second )
							cout << "Error2 in BotDiscover::rebuild\n";
						(*_anomalyList[*it1]).in_list[i].insert(it -> first);
						_scgList[it -> first] -> out_list[i].insert(*it1);
					}
					else{
						(_scgList.find(*it1) -> second -> in_list)[i].insert(it -> first);
					}
				}
				
				for(set<string>::iterator it1 = pvt.in_list[i].begin(); it1 != pvt.in_list[i].end(); ++ it1){
					if(degreeOneFilter(*it1))continue;
					if(_scgList.find(*it1) == _scgList.end()){
						if(!_scgList.insert(make_pair(*it1, newNode(false, count))).second)
							cout << "Error4 in BotDiscover::rebuild\n";
						(*_anomalyList[*it1]).out_list[i].insert(it -> first);
						_scgList[it -> first] -> in_list[i].insert(*it1);
					}
					else{
						(_scgList.find(*it1) -> second -> out_list)[i].insert(it -> first);
					}
				}
			}
		}
	}
}*/

void
BotDiscover::setSCG2(const double tau)
{
	int debug = 0;
	cout << "_anomalyList.size() = " << _anomalyList.size() << endl;
	cout << "SCGcheck\n";
	_SCG = MatrixXd::Zero(_anomalyList.size(), _anomalyList.size());
	cout << "SCGcheck2\n";
	cout << "_anomalyList.size() = " << _anomalyList.size() << endl;
	for(map<string, SCG_Node*>::iterator it1 = _anomalyList.begin(); it1 != _anomalyList.end(); ++it1){
		cout << "\r" << debug;
		for(map<string, SCG_Node*>::iterator it2 = it1; it2 != _anomalyList.end(); ++it2){
			if(it1 == it2)continue;
			if(corelation_coefficient(it1 -> first, it2 -> first) > tau){
				++debug;
				//if(debug % 10000 == 0)cout << "debug = " << debug;
				//cout << corelation_coefficient(it1 -> first, it2 -> first) << endl;
				//cout << it1 -> second -> id << "  " << it2 -> second -> id << " " << _scgList.size() << endl;
				_SCG(it1 -> second -> id, it2 -> second -> id) = 1;
			}
		}
	}
	cout << "SCG:\n";
	for(unsigned i = 0; i < _SCG.innerSize(); ++i){
		for(unsigned j = 0; j < _SCG.innerSize(); ++j)
			cout << _SCG(i, j) << " ";
		cout << endl;
	}
}

bool
BotDiscover::degreeOneFilter(string node)
{
	 return (_anomalyList[node] -> in_list.size() + _anomalyList[node] -> out_list.size() > 2) ? false : true;
}

SCG_Node*
BotDiscover::newNode(int& count)
{
	SCG_Node* ptr = new SCG_Node;
	ptr -> in_list = vector< vector<string> >(_anomaly.size(), vector<string>());
	ptr -> out_list = vector< vector<string> >(_anomaly.size(), vector<string>());
	ptr -> id = count++;
	return ptr;
}

void
BotDiscover::deleteNode(SCG_Node*& ptr)
{
	delete ptr;
	ptr = 0;
}

double
BotDiscover::mean(const string& i)
{
	const vector< vector<string> >& in = _anomalyList[i] -> in_list;
	const vector< vector<string> >& out = _anomalyList[i] -> out_list;
	double count = 0;
	for(size_t k = 0; k < in.size(); ++k){
		count += in[k].size();
		count += out[k].size();
	}
	return count / _anomalyNumber;
}

double
BotDiscover::deviation(const string& i)
{
	const vector< vector<string> >& in = _anomalyList[i] -> in_list;
	const vector< vector<string> >& out = _anomalyList[i] -> out_list;
	double bar = mean(i);
	double count = 0;
	for(size_t k = 0; k < in.size(); ++k)
		count += pow((out[k].size() + in[k].size() - bar), 2);
	
	return sqrt( count / (_anomalyNumber - 1) );
}

double
BotDiscover::corelation_coefficient(const string& i, const string& j)
{
	const vector< vector<string> >& in_i = _anomalyList[i] -> in_list;
	const vector< vector<string> >& out_i = _anomalyList[i] -> out_list;
	const vector< vector<string> >& in_j = _anomalyList[j] -> in_list;
	const vector< vector<string> >& out_j = _anomalyList[j] -> out_list;
	double bar_i = mean(i);
	double bar_j = mean(j);
	double count = 0;
	for(size_t k = 0; k < in_i.size(); ++k)
		count += (out_i[k].size() + in_i[k].size() - bar_i) * (out_j[k].size() + in_j[k].size() - bar_j);
	
	return count / ( (_anomalyNumber - 1) * deviation(i) * deviation(j) );
}

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

BotDiscover::BotDiscover()
{
	_anomalyNumber = 0;
}

BotDiscover::BotDiscover(const vector<bool>& anomaly, const vector< vector< vector<string> > >& timeList)
{
	_anomalyNumber = 0;
	_anomaly = anomaly;
	setPivot(timeList);
	rebuild();
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
BotDiscover::setPivot(const vector< vector< vector<string> > >& timeList, const double tau)
{
	// collect node
	map<string, double> node;
	//size_t count = 0;
	for(size_t i = 0; i < _anomaly.size(); ++i){
		if(_anomaly[i]){
			++_anomalyNumber;
			for(size_t j = 0; j < timeList[i].size(); ++j){
				if(node.insert(make_pair(timeList[i][j][3], 1)).second){
					//if(!_anomalyList.insert(make_pair(timeList[i][j][3], newNode(false, count))).second)
					//	cout << "Error in BotDiscover::setTotalInteraction\n";
				}
				else{
					++node[timeList[i][j][3]];
				}
				if(node.insert(make_pair(timeList[i][j][6], 1)).second){
					//if(!_anomalyList.insert(make_pair(timeList[i][j][6], newNode(false, count))).second)
					//	cout << "Error in BotDiscover::setTotalInteraction\n";
				}
				else{
					++node[timeList[i][j][6]];
				}
				
				//(_anomalyList[timeList[i][j][3]] -> out_list)[i].insert(timeList[i][j][6]);
				//(_anomalyList[timeList[i][j][6]] -> in_list)[i].insert(timeList[i][j][3]);
			}
		}
	}
	for(map<string, double>::iterator it = node.begin(); it != node.end(); ++it){
		//_anomalyList[it -> first] -> total_interaction = it -> second / _anomaly.size();
		
		if(it -> second / _anomaly.size() > tau){
			//_anomalyList[it -> first] -> pivot = true;
			_pivot.insert(it -> first);
			cout << it -> first << ": " << it -> second / _anomaly.size() << endl;
		}
		else{
			//_anomalyList[it -> first] -> pivot = false;
		}
	}
}

void
BotDiscover::rebuild()
{
	int count = 0;
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
}

void
BotDiscover::setSCG(const double tau)
{
	int debug = 0;
	cout << "_anomalyList.size() = " << _anomalyList.size() << endl;
	cout << "_scgList.size() = " << _scgList.size() << endl;
	cout << "SCGcheck\n";
	_SCG = MatrixXd::Zero(_scgList.size(), _scgList.size());
	cout << "SCGcheck2\n";
	cout << "_anomalyList.size() = " << _anomalyList.size() << endl;
	cout << "_scgList.size() = " << _scgList.size() << endl;
	for(map<string, SCG_Node*>::iterator it1 = _scgList.begin(); it1 != _scgList.end(); ++it1){
		cout << "\r" << debug;
		for(map<string, SCG_Node*>::iterator it2 = it1; it2 != _scgList.end(); ++it2){
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
	for(size_t i = 0; i < _SCG.innerSize(); ++i){
		for(size_t j = 0; j < _SCG.innerSize(); ++j)
			cout << _SCG(i, j) << " ";
		cout << endl;
	}
}

bool
BotDiscover::degreeOneFilter(string node)
{
	 return (_anomalyList[node] -> in_list.size() + _anomalyList[node] -> out_list.size() > 5) ? false : true;
}

SCG_Node*
BotDiscover::newNode(const bool& pvt, int& count)
{
	SCG_Node* ptr = new SCG_Node;
	ptr -> in_list = vector< set<string> >(_anomaly.size(), set<string>());
	ptr -> out_list = vector< set<string> >(_anomaly.size(), set<string>());
	ptr -> id = count++;
	ptr -> pivot = pvt;
	return ptr;
}

void
BotDiscover::deleteNode()
{
	
}

double
BotDiscover::mean(const string& i)
{
	const vector< set<string> >& in = _anomalyList[i] -> in_list;
	const vector< set<string> >& out = _anomalyList[i] -> out_list;
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
	const vector< set<string> >& in = _anomalyList[i] -> in_list;
	const vector< set<string> >& out = _anomalyList[i] -> out_list;
	double bar = mean(i);
	double count = 0;
	for(size_t k = 0; k < in.size(); ++k)
		count += pow((out[k].size() + in[k].size() - bar), 2);
	
	return sqrt( count / (_anomalyNumber - 1) );
}

double
BotDiscover::corelation_coefficient(const string& i, const string& j)
{
	const vector< set<string> >& in_i = _anomalyList[i] -> in_list;
	const vector< set<string> >& out_i = _anomalyList[i] -> out_list;
	const vector< set<string> >& in_j = _anomalyList[j] -> in_list;
	const vector< set<string> >& out_j = _anomalyList[j] -> out_list;
	double bar_i = mean(i);
	double bar_j = mean(j);
	double count = 0;
	for(size_t k = 0; k < in_i.size(); ++k)
		count += (out_i[k].size() + in_i[k].size() - bar_i) * (out_j[k].size() + in_j[k].size() - bar_j);
	
	return count / ( (_anomalyNumber - 1) * deviation(i) * deviation(j) );
}



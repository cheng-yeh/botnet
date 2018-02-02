/*********************************************************
  FileName     [ BotDiscover.h ]
  Description  [ discover bot based on anomalous windows ]
  Author       [ Cheng-Yeh (Gary) Chen ]
  Copyright    [ MIT ]
*********************************************************/
#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <unordered_set>
#include <set>
#include <Eigen/Dense>

#include "util.h"

using namespace std;
using Eigen::MatrixXd;
//using Eigen::VectorXd;


class BotDiscover
{
public:
	BotDiscover();
	BotDiscover(const vector<bool>& anomaly, const vector< vector< vector<string> > >& timeList);
	~BotDiscover();
	
	// set function
	void setSCG(const vector< vector< vector<string> > >& timeList, const double tau = 20);
	//void rebuild();
	void setSCG2(const double tau = 0.999);

private:
	bool degreeOneFilter(string node);
	void trimAnomalyList(vector<string>& removed);
	// node operation
	SCG_Node* newNode(int& count);
	void deleteNode(SCG_Node*& ptr);
	// statistic computation for SCGNode
	double mean(const string& i);
	double deviation(const string& i);
	double corelation_coefficient(const string& i, const string& j);
	// statistic computation for map
	//double mean(const string& i, const map<string, vector<double> > scg);
	//double deviation(const string& i, const map<string, vector<double> > scg);
	//double corelation_coefficient(const string& i, const string& j, const map<string, vector<double> > scg);

private:
	vector<bool> _anomaly;
	int _anomalyNumber;
	unordered_set<string> _pivot;
	map<string, SCG_Node*> _anomalyList;
	//map<string, SCG_Node*> _scgList;
	//vector< vector<double> > _total_interaction;
	MatrixXd _SCG;
};

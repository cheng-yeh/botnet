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
	void setPivot(const vector< vector< vector<string> > >& timeList, const double tau = 30);
	void rebuild();
	void setSCG(const double tau = 0.999);

private:
	bool degreeOneFilter(string node);
	// node operation
	SCG_Node* newNode(const bool& pvt, int& count);
	void deleteNode();
	// statistic computation
	double mean(const string& i);
	double deviation(const string& i);
	double corelation_coefficient(const string& i, const string& j);

private:
	vector<bool> _anomaly;
	int _anomalyNumber;
	map<string, SCG_Node*> _anomalyList;
	map<string, SCG_Node*> _scgList;
	//vector< vector<double> > _total_interaction;
	MatrixXd _SCG;
};

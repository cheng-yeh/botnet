/*************************************************************
  FileName     [ GraphDetector.h ]
  Description  [ anomoly detection using degree based method ]
  Author       [ Cheng-Yeh (Gary) Chen ]
  Copyright    [ MIT ]
*************************************************************/
#pragma once

#include <iostream>
#include <vector>
#include <Eigen/Dense>

#include "util.h"

using namespace std;
using Eigen::MatrixXd;
using Eigen::VectorXd;

//-----------------------------------------------------------------
//			Detector based on degree distribution
//-----------------------------------------------------------------
// please refer to the part of anomoly detection of
//		Wang, Jing, and Ioannis Ch Paschalidis.
//		"Botnet detection based on anomaly and community detection."
//		IEEE Transactions on Control of Network Systems 4.2 (2017): 392-404

class GraphDetector
{
public:
	GraphDetector();
	~GraphDetector();
	
	// turn _timeList into inter_graph
	void timelistToIntergraph();
	
	// procedures for detection
	void selectModel(const int s1 = 20,const int s2 = 40);
	void detect(vector<size_t>& anomaly);
	
	// public get function
	size_t getInterGraphSize(const size_t& graph) const { return _interGraph[graph] -> innerSize(); }
	const vector<bool>& getAnomaly() const { return _anomaly; }
	const vector< vector< vector<string> > >& getTimeList(){ return _timeList; }
	
private:
	// private get function
	int getDegree(const size_t graph, const size_t node) const;
	
	// models
	double log_ER(const vector<double>& degree, const double& lambda) const;
	double log_PA_plus(const vector<double>& degree, const double& gamma) const;
	double log_PA(const vector<double>& degree, const double& gamma, const double& theta) const;
	
	// max parameter
	double lambda_hat(const vector<double>& degree) const;
	double gamma_hat(const vector<double>& degree);
	
	// rate function
	double rate_function_ER(const vector<double>& degree, const double& lambda);
	double rate_function_BA(const vector<double>& degree, const double& alpha);
	double rate_function_CHJ(const vector<double>& degree, const double& p);
	
	// self-defined sum
	double special_sum(const vector<double>& degree, const int& i);

public:
	vector< vector< vector<string> > > _timeList;
	
private:
	vector<MatrixXd*> _interGraph;
	vector<bool> _anomaly;
	bool _selectedModel;
};

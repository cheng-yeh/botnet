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

//----------------------------------------------------------------
//			special time format class
//----------------------------------------------------------------
// parsing and comparison, and arithmetic operator for time format
// format: year/month/day hour:minute:second
class TimeKey
{
public:
	TimeKey(string time);
	~TimeKey(){}

	const double* operator() () const {
   		return _time;
   	}

	bool operator == (const TimeKey k) const {
   		for(size_t i = 0; i < 6; ++i)
   			if(this -> _time[i] != k()[i])
   				return false;
   		return true;
   	}

	bool operator > (const TimeKey k) const {
   		for(size_t i = 0; i < 6; ++i)
   			if(this -> _time[i] > k()[i])
   				return true;
   		return false;
   	}
   
	double operator - (const TimeKey k) const {
		double delta = 0;
		int _cross[6] = {0, 0, 0, 3600, 60, 1};
		for(size_t i = 0; i < 6; ++i){
			delta += (this -> _time[i] - k()[i]) * _cross[i];
		}
		return delta;
	}
	
	int operator / (const double mod) const {
		double delta = 0;
		int _cross[6] = {0, 0, 0, 3600, 60, 1};
		for(size_t i = 0; i < 6; ++i){
			delta += (this -> _time[i]) * _cross[i];
		}
		return delta / mod;
	}

private:
	double _time[6];
};

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
	
	// initialize the _timeList from Reader
	void readGraph(const vector< vector<string> >& raw_data, bool whole);
	
	// procedures for detection
	void selectModel(const int s1 = 20,const int s2 = 40);
	void detect(vector<size_t>& anomaly);
	
	// public get function
	size_t getWindowNum() const { return _interGraph.size(); }
	size_t getInterGraphSize(const size_t& graph, bool whole) const { 
		if(whole)return _interGraph[graph] -> innerSize();
		else return _interGraph_back[graph] -> innerSize();
	}
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
	
private:
	vector< vector< vector<string> > > _timeList;
	vector<MatrixXd*> _interGraph;
	vector<MatrixXd*> _interGraph_back;
	vector<bool> _anomaly;
	bool _selectedModel;
};

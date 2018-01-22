#pragma once

#include <iostream>
#include <vector>
#include <Eigen/Dense>

#include "util.h"

using namespace std;
using Eigen::MatrixXd;
using Eigen::VectorXd;

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
		int cross[6] = {0, 0, 0, 3600, 60, 1};
		for(size_t i = 0; i < 6; ++i){
			delta += (this -> _time[i] - k()[i]) * cross[i];
		}
		return delta;
	}
	
	int operator / (const double mod) const {
		double delta = 0;
		int cross[6] = {0, 0, 0, 3600, 60, 1};
		for(size_t i = 0; i < 6; ++i){
			delta += (this -> _time[i]) * cross[i];
		}
		return delta / mod;
	}

private:
	double _time[6];
};

class GraphDetector
{
public:
	GraphDetector();
	~GraphDetector();
	
	// initialize the _timeList from Reader
	void init(const vector< vector<string> >& raw_data);
	
	// s1 for the number of samples from the k interaction graphs
	// s2 for the number of samples from each interaction graph
	bool selectModel(const int s1 = 20,const int s2 = 40);
	
	// public get function
	size_t getWindowNum() const{ return _timeList.size(); }
	size_t getInterGraphSize(const size_t& graph) const{ return _interGraph[graph] -> innerSize(); }
	
	
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
	
	
private:
	vector< vector< vector<string> > > _timeList;
	vector<MatrixXd*> _interGraph;
	//vector< vector< >>
};

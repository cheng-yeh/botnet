/*********************************************************
Filename     [ util.h ]
Description  [ some global help functions and definition ]
Author       [ Cheng-Yeh (Gary) Chen]
Copyright    [ MIT ]
*********************************************************/
#pragma once

#include <iostream>
#include <cstring>
#include <string>
#include <math.h>
#include <Eigen/Dense>
#include <set>

#include "rnGen.h"

using namespace std;
using Eigen::MatrixXd;
using Eigen::VectorXd;

#define PRECISION 1e-7

// struct for global arguments
typedef struct globalArg_t{
	string totalList;  		// mandatory
	string botList;    		// optional
	string botOne;     		// mandatory
	size_t windowNumber;	// mandatory
	string outputFile; 		// optional
} globalArg;

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


struct SCG_Node
{
	vector< set<string> > in_list;
	vector< set<string> > out_list;
	double interaction = 0;
	bool pivot;
	size_t id;
};

bool Str2Double(const string& str, double& num);

double factorial(const double& num);
double derivative(double (*f)(double), const double& x, const double& h);

void sampling(vector<size_t>& sam, const size_t& num, const size_t& range);
double fi(double x);
void Degree2Distribution(vector<double>& distribution, const VectorXd& degree);

void VectorXd2Vector(const VectorXd& v1, vector<double>& v2);

float poisson(int k, double lambda);

/*********************************************************************
Author       [ Chung-Yang (Ric) Huang ]
Copyright    [ Copyleft(c) 2007-present LaDs(III), GIEE, NTU, Taiwan ]
*********************************************************************/
size_t StrGetTok(const string& str, string& tok, size_t pos = 0, const char del = ' ');
bool Str2Int(const string& str, int& num);

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
//#include <set>

#include "rnGen.h"

using namespace std;
using Eigen::MatrixXd;
using Eigen::VectorXd;

#define PRECISION 1e-7

struct SCG_Node
{
	vector< vector<string> > in_list;
	vector< vector<string> > out_list;
	//double total_interaction;
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

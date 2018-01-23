#pragma once

#include <iostream>
#include <cstring>
#include <string>
#include <math.h>
#include <Eigen/Dense>

#include "rnGen.h"

using namespace std;
using Eigen::MatrixXd;
using Eigen::VectorXd;

#define PRECISION 1e-7

/*********************************************************************
Author       [ Chung-Yang (Ric) Huang ]
Copyright    [ Copyleft(c) 2007-present LaDs(III), GIEE, NTU, Taiwan ]
*********************************************************************/

size_t StrGetTok(const string& str, string& tok, size_t pos = 0, const char del = ' ');
bool Str2Int(const string& str, int& num);
bool Str2Double(const string& str, double& num);

double factorial(const double& num);
double derivative(double (*f)(double), const double& x, const double& h);

void sampling(vector<size_t>& sam, const size_t& num, const size_t& range);
double fi(double x);
void Degree2Distribution(vector<double>& distribution, const VectorXd& degree);

void VectorXd2Vector(const VectorXd& v1, vector<double>& v2);

#include <set>
#include <map>
#include <boost/math/special_functions/zeta.hpp>
#include <math.h>

#include "util.h"

using namespace std;
using namespace boost::math;
using Eigen::MatrixXd;
using Eigen::VectorXd;

RandomNumGen  rnGen(0);

size_t
StrGetTok(const string& str, string& tok, size_t pos,
            const char del)
{
   	size_t begin = pos;
   	//cout << "begin = " << begin << " pos = " << pos << endl;
   	if (begin == string::npos) { tok = ""; return begin; }
   	size_t end = str.find_first_of(del, begin);
   	//cout << "end = " << end << endl;
   	tok = str.substr(begin, end - begin);
   	if(end == string::npos)return end;
   	return end + 1;
}

bool
Str2Int(const string& str, int& num)
{
   	num = 0;
   	size_t i = 0;
   	int sign = 1;
   	if (str[0] == '-') { sign = -1; i = 1; }
   	bool valid = false;
   	for (; i < str.size(); ++i) {
      	if (isdigit(str[i])) {
         	num *= 10;
         	num += int(str[i] - '0');
         	valid = true;
      	}
      	else return false;
   	}
   	num *= sign;
   	return valid;
}

bool
Str2Double(const string& str, double& num)
{
	num = 0;
	size_t i = 0;
	size_t zero;
	double temp;
	double sign = 1;
	if (str[0] == '-') { sign = -1; i = 1; }
	bool valid = false;
	for (; i < str.size(); ++i) {
		if (str[i] == '.')break;
		if (isdigit(str[i])) {
			num *= 10;
			num += int(str[i] - '0');
			valid = true;
		}
		else{
			return false;
		}
	}
	zero = i++;
	for (; i < str.size(); ++i) {
		if (isdigit(str[i])) {
			temp = (double)(str[i] - '0');
			for(size_t j = 0; j < i - zero; ++j){
				temp /= 10;
			}
			num += temp;
		}
		else{
			return false;
		}
	}

  	num *= sign;
  	return valid;
}

double
factorial(const double& num)
{
	double out = 1;
	for(size_t i = 1; i <= num; ++i)
		out *= i;
	return out;
}

double
derivative(double (*f)(double), const double& x, const double& h)
{
	return (f(x + h) - f(x)) / h;
}

void
sampling(vector<size_t>& sam, const size_t& num, const size_t& range)
{
	set<int> chosen;
	for(size_t i = 0; i < num; ++i)
		while(!chosen.insert(rnGen(range)).second){}
	
	for(set<int>::iterator it = chosen.begin(); it != chosen.end(); ++it)
		sam.push_back(*it);
}

double
fi(double x)
{
	return derivative(&zeta, x, PRECISION) / zeta(x);
}

void
Degree2Distribution(vector<double>& distribution, const VectorXd& degree)
{
	map<double, double> collect;
	size_t max_degree = 0;
	for(size_t i = 0; i < degree.size(); ++i){
		if(collect.insert(make_pair(degree(i), 1)).second){}
		else{
			++collect[degree(i)];
		}
		if(max_degree < degree(i))max_degree = degree(i);
	}
	
	for(map<double, double>::iterator it = collect.begin(); it != collect.end(); ++it)
		(it -> second) /= degree.size();
	
	for(size_t i = 0; i < max_degree + 1; ++i)
		if(collect.find(i) != collect.end())distribution.push_back(collect[i]);
		else distribution.push_back(0);
}

void
VectorXd2Vector(const VectorXd& v1, vector<double>& v2)
{
	for(size_t i = 0; i < v1.size(); ++i)
		v2.push_back(v1(i));
}

float
poisson(int k, double lambda) {
    // https://en.wikipedia.org/wiki/Poisson_distribution#Definition
    return pow(M_E, k * log(lambda) - lambda - lgamma(k + 1.0));
}

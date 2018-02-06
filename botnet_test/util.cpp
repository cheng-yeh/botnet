/*********************************************************
Filename     [ util.cpp ]
Description  [ some global help functions and definition ]
Author       [ Cheng-Yeh (Gary) Chen]
Copyright    [ MIT ]
*********************************************************/
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

TimeKey::TimeKey(string time)
{
	// initialize _time[6]
	string tok;
	size_t pos = 0;
	char del[6] = {'/', '/', ' ', ':', ':'};
	for(size_t i = 0; i < 5; ++i){
		pos = StrGetTok(time, tok, pos, del[i]);
		if(!Str2Double(tok, _time[i]))cerr << "Parsing error! " << tok << " is not a double!\n";
	}
	StrGetTok(time, tok, pos);
	if(!Str2Double(tok, _time[5]))cerr << "Parsing error! " << tok << " is not a double!\n";
}

size_t
StrGetTok(const string& str, string& tok, size_t pos,
            const char del)
{
   	size_t begin = pos;
   	if (begin == string::npos) { tok = ""; return begin; }
   	size_t end = str.find_first_of(del, begin);
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
	for(unsigned i = 0; i < degree.size(); ++i){
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
	for(unsigned i = 0; i < v1.size(); ++i)
		v2.push_back(v1(i));
}

float
poisson(int k, double lambda) {
    // https://en.wikipedia.org/wiki/Poisson_distribution#Definition
    return pow(M_E, k * log(lambda) - lambda - lgamma(k + 1.0));
}

vector< set<string> > 
timeList2Set(const vector< vector< vector<string> > >& v)
{
	vector< set<string> > s;
	for(size_t i = 0; i < v.size(); ++i){
		for(size_t j = 0; j < v[i].size(); ++j){
			s[i].insert(v[i][j][3]);
			s[i].insert(v[i][j][6]);
		}
	}
	
	return s;
}

set<string>
Vector2Set(const vector<string>& v)
{
	set<string> s;
	for(size_t i = 0; i < v.size(); ++i)
		s.insert(v[i]);
	
	return s;
}

double
FPR(const vector<string>& result, const vector<string>& ans, const vector< set<string> >& timeList, const vector<bool>& anomaly)
{
	pair<double, double> tnfp = TNFP_bar(result, ans, timeList, anomaly);
	return tnfp.second / (tnfp.first + tnfp.second);
}

double
recall(const vector<string>& result, const vector<string>& ans, const vector< set<string> >& timeList, const vector<bool>& anomaly)
{
	pair<double, double> tpfn = TPFN_bar(result, ans, timeList, anomaly);
	return tpfn.first / (tpfn.first + tpfn.second);
}

double
precision(const vector<string>& result, const vector<string>& ans, const vector< set<string> >& timeList, const vector<bool>& anomaly)
{
	pair<double, double> tpfn = TPFN_bar(result, ans, timeList, anomaly);
	pair<double, double> tnfp = TNFP_bar(result, ans, timeList, anomaly);
	return tpfn.first / (tpfn.first + tnfp.first);
}

double
f1_score(const vector<string>& result, const vector<string>& ans, const vector< set<string> >& timeList, const vector<bool>& anomaly)
{
	double r = recall(result, ans, timeList, anomaly);
	double p = precision(result, ans, timeList, anomaly);
	return 2 * (r * p) / (r + p);
}

double
g_score(const vector<string>& result, const vector<string>& ans, const vector< set<string> >& timeList, const vector<bool>& anomaly)
{
	return sqrt( recall(result, ans, timeList, anomaly) * precision(result, ans, timeList, anomaly) );
}

pair<double, double>
TPFN_bar(const vector<string>& result, const vector<string>& ans, const vector< set<string> >& timeList, const vector<bool>& anomaly)
{
	pair<double, double> total = make_pair(0., 0.);
	
	for(size_t i = 0; i < anomaly.size(); ++i){
		if(anomaly[i]){
			pair<double, double> score = TPFN(result, ans, timeList[i]);
			total.first += score.first * cost(i);
			total.second += score.second * cost(i);
		}
	}
	
	return total;
}

pair<double, double>
TNFP_bar(const vector<string>& result, const vector<string>& ans, const vector< set<string> >& timeList, const vector<bool>& anomaly)
{
	pair<double, double> total = make_pair(0., 0.);
	
	for(size_t i = 0; i < anomaly.size(); ++i){
		if(anomaly[i]){
			pair<double, double> score = TNFP(result, ans, timeList[i]);
			total.first += score.first * cost(i);
			total.second += score.second * cost(i);
		}
	}
	
	return total;
}

double
cost(int t)
{
	return 1 + exp(-t);
}

pair<double, double>
TPFN(const vector<string>& result, const vector<string>& ans, const set<string> & timeList)
{
	const set<string>& s = Vector2Set(ans);
	set<string> inter;
	for(auto& x: timeList)
		if(s.find(x) != s.end())
			inter.insert(x);
	
	int miss = 0;
	for(auto& x: result)
		if(inter.find(x) == inter.end())
			++miss;
	
	return make_pair( (inter.size() - miss) / inter.size(), miss / inter.size() );
}

pair<double, double>
TNFP(const vector<string>& result, const vector<string>& ans, const set<string> & timeList)
{
	const set<string>& s = Vector2Set(ans);
	set<string> inter;
	for(auto& x: timeList)
		if(s.find(x) == s.end())
			inter.insert(x);
	
	int miss = 0;
	for(auto& x: result)
		if(inter.find(x) != inter.end())
			++miss;
	
	return make_pair( (inter.size() - miss) / inter.size(), miss / inter.size() );
}

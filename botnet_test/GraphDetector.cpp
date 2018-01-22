#include <map>
#include <math.h>
#include <boost/math/special_functions/zeta.hpp>

#include "GraphDetector.h"
//#include "util.h"

using namespace std;
using Eigen::MatrixXd;
using namespace boost::math;

extern size_t window_num;
extern RandomNumGen  rnGen;

TimeKey::TimeKey(string time)
{
	string tok;
	size_t pos = 0, count = -1;
	char del[6] = {'/', '/', ' ', ':', ':'};
	for(size_t i = 0; i < 5; ++i){
		pos = StrGetTok(time, tok, pos, del[i]);
		if(!Str2Double(tok, _time[i]))cerr << "Parsing error! " << tok << " is not a double!\n";
	}
	StrGetTok(time, tok, pos);
	if(!Str2Double(tok, _time[5]))cerr << "Parsing error! " << tok << " is not a double!\n";
}

GraphDetector::GraphDetector()
{

}

GraphDetector::~GraphDetector()
{
	for(size_t i = 0; i < _interGraph.size(); ++i){
		delete _interGraph[i];
		_interGraph[i] = 0;
	}
}

void
GraphDetector::init(const vector< vector<string> >& rawdata)
{
	TimeKey begin = TimeKey(rawdata[0][0]);
	TimeKey end = TimeKey(rawdata[rawdata.size() - 1][0]);
	double delta = end - begin;
	cout.precision(10);
	cout << "The total duration is " << delta << " seconds\n";
	if(window_num == 0)cout << "Need to assign number of windows\n";
	else cout << "The window size is " << delta / window_num << " seconds\n";
	
	double window_size = delta / window_num;
	
	size_t j = 0;
	for(size_t i = 0; i < window_num; ++i){
		vector< vector<string> > temp;

		for(; j < rawdata.size(); ++j){
			TimeKey key = TimeKey(rawdata[j][0]);

			if(int( (key - begin) / window_size) == i){
				temp.push_back(rawdata[j]);
			}
			else{
				_timeList.push_back(temp);
				break;
			}
		}
	}
	for(size_t i = 0; i < _timeList.size(); ++i){
		size_t count = 0;
		map<string, size_t> node;
		for(size_t j = 0; j < _timeList[i].size(); ++j){
			if(node.insert( make_pair(_timeList[i][j][3], count) ).second)++count;
			if(node.insert( make_pair(_timeList[i][j][6], count) ).second)++count;
		}
		if(count != node.size())cout << "Error!!!!!!!!! count != node.size()\n";
		cout << "count = " << count << endl;
		MatrixXd* ptr = new MatrixXd(node.size(), node.size());
		for(size_t k = 0; k < node.size(); ++k)
			for(size_t l = 0; l < node.size(); ++l)
				(*ptr)(k, l) = 0;
		
		for(size_t j = 0; j < _timeList[i].size(); ++j){
			(*ptr)(node[_timeList[i][j][3]], node[_timeList[i][j][6]]) = 1;
		}
		_interGraph.push_back(ptr);
	}
	cout << "(0,0) = " << getDegree(0,0) << endl;
	cout << "Init successfully!\n";
}



bool
GraphDetector::selectModel(const int s1,const int s2)
{
	// sampling
	vector<size_t> sample;
	//vector< vector<double> > chosen;
	vector<double> chosen;
	sampling(sample, s1, getWindowNum());
	
	for(size_t i = 0; i < s1; ++i){
		//chosen_graph.push_back(rnGen(_interGraph.size()));
		size_t graphSize = getInterGraphSize( sample[i] );
		VectorXd v( graphSize );
		for(size_t j = 0; j < v.size(); ++j)
			v(j) = 1;
		//vector<double> distribution;
		vector<size_t> sample2;
		sampling(sample2, s2, graphSize);
		for(size_t j = 0; j < s2; ++j){
			double v1 = (*_interGraph[sample[i]]).row(sample2[j]) * v;
			double v2 = (*_interGraph[sample[i]]).col(sample2[j]).transpose() * v;
			chosen.push_back(v1 + v2);
		}
		//chosen.push_back(distribution);
	}
	cout << "lambda_hat = " << lambda_hat(chosen) << endl;
	cout << "gamma_hat = " << gamma_hat(chosen) << endl;
	
	cout << "max(log_ER) = " << log_ER(chosen, lambda_hat(chosen)) << endl;
	cout << "max(log_PA) = " << log_PA(chosen, gamma_hat(chosen), 2.44) << endl;
}

int
GraphDetector::getDegree(const size_t graph, const size_t node) const
{
	VectorXd v(getInterGraphSize(graph));
	for(size_t i = 0; i < v.size(); ++i)
		v(i) = 1;
	int v1 = (*_interGraph[graph]).row(node) * v;
	int v2 = (*_interGraph[graph]).col(node).transpose() * v;
	return (v1 + v2);
}

double
GraphDetector::log_ER(const vector<double>& degree, const double& lambda) const
{
	double sum1 = 0, sum2 = 0;
	for(size_t i = 0; i < degree.size(); ++i){
		sum1 += degree[i];
		sum2 += log(factorial(degree[i]));
	}
	return log(lambda) * sum1 - lambda * degree.size() - sum2;
}

double
GraphDetector::log_PA_plus(const vector<double>& degree, const double& gamma) const
{
	double sum = 0;
	for(size_t i = 0; i < degree.size(); ++i)
		if(degree[i])
			sum += log(degree[i]);
			
	return - gamma * sum - degree.size() * log(boost::math::zeta(gamma));
}

double
GraphDetector::log_PA(const vector<double>& degree, const double& gamma, const double& theta) const
{
	double sum = 0;
	for(size_t i = 0; i < degree.size(); ++i)
		if(!degree[i])
			sum += theta;

	return log_PA_plus(degree, gamma) - sum;
}

double
GraphDetector::lambda_hat(const vector<double>& degree) const
{
	double sum = 0;
	for(size_t i = 0; i < degree.size(); ++i)
		sum += degree[i];
	return sum / degree.size();
}

double
GraphDetector::gamma_hat(const vector<double>& degree)
{
	double sum = 0;
	for(size_t i = 0; i < degree.size(); ++i)
		if(degree[i])
			sum -= log(degree[i]);
	sum /= degree.size();
	cout << "log(20) = " << log(20) << endl;
	cout << "sum = " << sum << endl;
	// use iteration to get a close answer
	double delta = 1, y = 2;
	while(delta > PRECISION || delta < - PRECISION){
		y += delta / derivative(fi, y, PRECISION);
		delta = sum - fi(y);
		cout << delta << endl;
	}
	return y;
}


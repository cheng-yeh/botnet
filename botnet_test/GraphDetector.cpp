/*************************************************************
  FileName     [ GraphDetector.cpp ]
  Description  [ anomoly detection using degree based method ]
  Author       [ Cheng-Yeh (Gary) Chen ]
  Copyright    [ MIT ]
*************************************************************/
#include <map>
#include <math.h>
#include <boost/math/special_functions/zeta.hpp>

#include "GraphDetector.h"

using namespace std;
//using Eigen::MatrixXd;
using namespace boost::math;

extern globalArg args;
extern RandomNumGen  rnGen;

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
GraphDetector::timelistToIntergraph(){
	vector<MatrixXd*>* ptr2 = &_interGraph;
	
	for(size_t i = 0; i < _timeList.size(); ++i){
		size_t count = 0;
		map<string, size_t> node;
		bool bot = false;
		for(size_t j = 0; j < _timeList[i].size(); ++j){
			if(node.insert( make_pair(_timeList[i][j][3], count) ).second)++count;
			if(node.insert( make_pair(_timeList[i][j][6], count) ).second)++count;
			if(!bot)
				if(_timeList[i][j][14].find("Botnet") != string::npos)
					bot = true;
		}
		_anomaly.push_back(bot);
		cout << i << ":" << bot << " ";
		if(count != node.size())cout << "Error!!!!!!!!! count != node.size()\n";
		cout << "count = " << count << endl;
		
		MatrixXd* ptr3 = new MatrixXd(node.size(), node.size());
		for(size_t k = 0; k < node.size(); ++k)
			for(size_t l = 0; l < node.size(); ++l)
				(*ptr3)(k, l) = 0;
		
		for(size_t j = 0; j < _timeList[i].size(); ++j){
			(*ptr3)(node[_timeList[i][j][3]], node[_timeList[i][j][6]]) = 0.5;
			(*ptr3)(node[_timeList[i][j][6]], node[_timeList[i][j][3]]) = 0.5;
		}
		(*ptr2).push_back(ptr3);
	}
	
	cout << "(0,0) = " << getDegree(0,0) << endl;
	cout << "Init successfully!\n";
	//_timeList.clear();
	for(size_t i = 0; i < _anomaly.size(); ++i)
		cout << _anomaly[i] << ", ";
}

// s1 for the number of samples from the k interaction graphs
// s2 for the number of samples from each interaction graph
void
GraphDetector::selectModel(const size_t s1,const size_t s2)
{
	// sampling
	vector<size_t> sample;
	//vector< vector<double> > chosen;
	vector<double> chosen;
	sampling(sample, s1, args.windowNumber);
	
	for(size_t i = 0; i < s1; ++i){
		//chosen_graph.push_back(rnGen(_interGraph.size()));
		size_t graphSize = getInterGraphSize(sample[i]);
		VectorXd v = VectorXd::Ones(graphSize);
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
	
	_selectedModel = log_ER(chosen, lambda_hat(chosen)) > log_PA(chosen, gamma_hat(chosen), 2.44);
}

void
GraphDetector::detect(vector<size_t>& anomaly)
{
	vector< vector<double> > all_degree;
	vector< vector<double> > all_distribution;
	for(size_t i = 0; i < args.windowNumber; ++i){
		vector<double> distribution;
		vector<double> degree;
		
		VectorXd v = VectorXd::Ones( (*_interGraph[i]).innerSize() );
		VectorXd v1 = ( (*_interGraph[i]) * v ).transpose();
		VectorXd v2 = ( (*_interGraph[i]).transpose() * v ).transpose();
		
		Degree2Distribution(distribution, v1 + v2);
		all_distribution.push_back(distribution);
		
		VectorXd2Vector(v1 + v2, degree);
		all_degree.push_back(degree);
	}
	
	for(size_t i = 0; i < args.windowNumber; ++i){
		if(!_anomaly[i]){
			cout << "[";
			//for(size_t j = 0; j < all_distribution[i].size(); ++j){
			//	if(all_distribution[i][j])
			//		cout << "[" << j << ", " << all_distribution[i][j] << "], ";
			//}
			//cout << "], ";
		}
	}
	
	vector<double> divergence;
	
	if(_selectedModel){
		cout << "Choose ER\n";
		for(size_t i = 0; i < args.windowNumber; ++i){
			double lambda = lambda_hat(all_degree[i]);
			double er = rate_function_ER(all_distribution[i], lambda);
			//divergence.push_back( (ba > chj) ? chj : ba );
			divergence.push_back(er);
			//cout << divergence[i] << ", ";
		}
	}
	else{
		cout << "Choose PA\n";
		for(size_t i = 0; i < args.windowNumber; ++i){
			double gamma = gamma_hat(all_degree[i]);
			double ba = rate_function_BA(all_distribution[i], gamma - 3);
			double chj = rate_function_CHJ(all_distribution[i], 1 - 1 / (gamma - 1));
			divergence.push_back( (ba > chj) ? chj : ba );
			//cout << divergence[i] << ", ";
			//if(divergence[i] < 1.93){
			//	for(size_t j = 0; j < all_distribution[i].size(); ++j){
			//		if(all_distribution[i][j]){
			//			cout << "[" << j << "," << all_distribution[i][j] << "], ";
			//		}
			//	}
			//	cout << endl;
			//}
		}
	}
}

int
GraphDetector::getDegree(const size_t graph, const size_t node) const
{
	VectorXd v = VectorXd::Ones(getInterGraphSize(graph));
	int v1 = (*_interGraph[graph]).row(node) * v;
	//int v2 = (*_interGraph[graph]).col(node).transpose() * v;
	return (v1);
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
	// use iteration to get a close answer
	double delta = 1, y = 2;
	while(delta > PRECISION || delta < - PRECISION){
		y += delta / derivative(fi, y, PRECISION);
		delta = sum - fi(y);
	}
	return y;
}

double
GraphDetector::rate_function_ER(const vector<double>& degree, const double& lambda)
{
	double sum = 0;
	for(size_t i = 1; i < degree.size(); ++i){
		
		if(degree[i] && poisson(i, lambda)){
			sum += degree[i] * log( degree[i] / poisson(i, lambda) );
			//cout << "i = " << i << endl;
			//cout << "degree[i] = " << degree[i] << endl;
			//cout << "poisson(i, lambda) = " << poisson(i, lambda) << endl;
			//cout << 
		}
	}
	double ave = lambda_hat(degree);
	sum += 0.5 * (ave - lambda) + 0.5 * ave * log(lambda) - 0.5 * ave * log(ave);
	return sum;
}
double
GraphDetector::rate_function_BA(const vector<double>& degree, const double& alpha)
{
	double item1 = 0, item2 = 0;
	for(size_t i = 0; i < degree.size(); ++i){
		if(!degree[i])continue;
		double special = special_sum(degree, i);
		item1 += (1 - special) * log( (1 - special) * (2 + alpha) / ( (i + 1 + alpha) * degree[i] ) );
		//cout << "i = " << i << " item1 = " << item1 << endl;
		//cout << "degree = " << degree[i] << endl;
		//cout << (1 - special) * (2 + alpha) / ( (i + 1 + alpha) * degree[i] ) << endl;
		//cout << ( (i + 1 + alpha) * degree[i] ) << endl;
		//cout << (1 - special) << endl;
	}
	for(size_t i = 0; i < degree.size(); ++i)
		item2 += i * degree[i];
	item1 += (1 - item2) * log(2 + alpha);
	//cout << "item1 = " << item1;
	return item1;
}
double
GraphDetector::rate_function_CHJ(const vector<double>& degree, const double& p)
{
	double item1 = (1 - degree[0]) * log( (1 - degree[0]) / p);
	double item2 = 0;
	for(size_t i = 1; i < degree.size(); ++i){
		if(!degree[i])continue;
		double special = special_sum(degree, i);
		item1 += (1 - special) * log( (1 - special) * 2 / ( (1 - p) * (1 + i) * degree[i] ) );
	}
	for(size_t i = 0; i < degree.size(); ++i)
		item2 += i * degree[i];
	
	item1 += (1 - item2) * log( 2 / (1 - p) );
	
	return item2;
	
}

double
GraphDetector::special_sum(const vector<double>& degree, const int& i)
{
	//double sum = 0;
	//for(size_t j = 0; j <= i; ++j)
	//	sum += degree[j];
	return degree[i] * i;
}

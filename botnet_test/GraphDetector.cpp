#include <map>

#include "GraphDetector.h"
//#include "util.h"

using namespace std;
using Eigen::MatrixXd;

extern size_t window_num;

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
		cout << "node.size() = " << node.size() << endl;
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

void
GraphDetector::sampling(const int s1,const int s2)
{
	
}

int
GraphDetector::getDegree(const size_t graph, const size_t node) const
{
	cout << "_interGraph[graph] -> size() = " << _interGraph[graph] -> innerSize() << endl;
	VectorXd v(_interGraph[graph] -> innerSize());
	for(size_t i = 0; i < v.size(); ++i)
		v(i) = 1;
	cout << "v1 = " << (*_interGraph[graph]).row(node) * v << endl;
	cout << "v2 = " << (*_interGraph[graph]).col(node).transpose() * v << endl;
	int v1 = (*_interGraph[graph]).row(node) * v;
	int v2 = (*_interGraph[graph]).col(node).transpose() * v;
	return (v1 + v2);
}

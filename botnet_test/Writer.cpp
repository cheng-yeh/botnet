/**************************************************
  FileName     [ Writer.h ]
  Description  [ write output to specified  files ]
  Author       [ Cheng-Yeh (Gary) Chen ]
  Copyright    [ MIT ]
**************************************************/
#include "Writer.h"

using namespace std;

extern globalArg args;

Writer::Writer(void)
{

}

Writer::~Writer(void)
{

}

void
Writer::GroundTruthWriter(const vector< vector<string> >& data, const vector<string>& bot, string& filename)
{
	cout << "writer1\n";
	ofstream of;
	of.open(filename);
	cout << "writer2\n";
	of << bot << "\n";
	cout << bot << "\n";
	for(size_t i = 0; i < data.size(); ++i){
		if(data[i][3] == bot && data[i][6] != bot){
			of << data[i][6] << "\n";
			cout << data[i][6] << "\n";
		}
		else if(data[i][3] != bot && data[i][6] == bot){
			of << data[i][3] << "\n";
			cout << data[i][3] << "\n";
		}
	}
	cout << "writer3\n";
	of.close();
}

void
Writer::SADWriter(const string& data, string& filename)
{

}

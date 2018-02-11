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
	set<string> related;
	ofstream of;
	of.open(filename);
	
	for(size_t b = 0; b < bot.size(); ++b){
		for(size_t i = 0; i < data.size(); ++i){
			if(data[i][3] == bot[b] && data[i][6] != bot[b])
				related.insert(data[i][6]);

			else if(data[i][3] != bot[b] && data[i][6] == bot[b])
				related.insert(data[i][3]);
		}
	}
	
	for(size_t b = 0; b < bot.size(); ++b)
		of << bot[b] << "\n";
	for(auto& x: related)
		of << x << "\n";
	of.close();
	
	cout << "size fo bot:" << bot.size() << endl;
	cout << "size of related bot:" << related.size() << endl;
}

void
Writer::SADPWriter(const vector< vector<double> > data, string& filename)
{
	ofstream of;
	of.open(filename);
	
	of << "\"" << filename << ": mDim = " << data.size() << ", nBLOCK = 1, {" << data.size() << "}\n";
	
	of << "   " << data.size() << " = mDim\n   1 = nBLOCK\n   " << data.size() << " = bLOCKsTRUCT\n{";
	
	for(size_t i = 1; i < data.size(); ++i)
		of << "1, ";
	
	of << "1}\n{ {";
	
	for(size_t i = 0; i < data.size(); ++i){
		for(size_t j = 0; j < data.size(); ++j){
			of << " " << data[i][j];
			if(j != data.size() - 1)
				of << ", ";
		}
		if(i != data.size() - 1)
			of << "}, {";
	}
	
	of << "} }\n";
	
	for(size_t i = 0; i < data.size(); ++i){
		of << "{ {";
		for(size_t j = 0; j < data.size(); ++j){
			for(size_t k = 0; k < data.size(); ++k){
				of << " " << ( (j == k) ? 1 : 0 );
				if(k != data.size() - 1)
					of << ", ";
			}
			if(j != data.size() - 1)
				of << "}, {";
		}
		of << "} }\n";
	}
}

/***********************************************************
  FileName     [ Reader.h ]
  Description  [ reader for formats needed in this project ]
  Author       [ Cheng-Yeh (Gary) Chen ]
  Copyright    [ MIT ]
***********************************************************/
#include "Reader.h"

#include <fstream>
#include <algorithm>
using namespace std;

extern size_t window_num;

Reader::Reader(void)
{
}


Reader::~Reader(void)
{
}

void
Reader::ReadFromBinetflow(const std::string& fname)
{
	// read and parse from the file to a 2d vector of string
	ifstream file(fname.c_str());
	if(!file.is_open())
		return;
	bool skip = true;
	int length = -1;
	char buff[512];
	while(file.getline(buff, 512))
	{
		// skip the first line
		if(skip){
			skip = false;
			continue;
		}
		
		vector<string> data;
		size_t pos = 0, end = -1;
		string tok = "\0";
		while(pos != end){
			if(end + 1 != pos){
				data.push_back(tok);
				pos = end;
			}
			end = StrGetTok(buff, tok, pos, ',');
		}
		if(length == -1)length = data.size();
		if(length != data.size()){
			cerr << "Format Error!\n";
			return;
		}
		raw_data.push_back(data);
	}
	file.close();
}

const vector< vector<string> >&
Reader::getRawData() const
{
	return raw_data;
}

void
Reader::rawToTimelist(vector< vector< vector<string> > >& _timeList)
{
	TimeKey begin = TimeKey(raw_data[0][0]);
	TimeKey end = TimeKey(raw_data[raw_data.size() - 1][0]);
	double delta = end - begin;
	cout.precision(10);
	cout << "The total duration is " << delta << " seconds\n";
	if(window_num == 0)cout << "Need to assign number of windows\n";
	else cout << "The window size is " << delta / window_num << " seconds\n";
	
	double window_size = delta / window_num;
	
	size_t j = 0;
	for(size_t i = 0; i < window_num; ++i){
		vector< vector<string> > temp;

		for(; j < raw_data.size(); ++j){
			TimeKey key = TimeKey(raw_data[j][0]);

			if(int( (key - begin) / window_size) == i){
				temp.push_back(raw_data[j]);
			}
			else{
				_timeList.push_back(temp);
				break;
			}
		}
	}
}

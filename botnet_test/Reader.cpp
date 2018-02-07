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

extern globalArg args;

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
	if(!file.is_open()){
		cout << "Error! Cannot open file " << fname << endl;
		return;
	}
	int length = -1;
	char buff[512];
	
	// skip the first line
	file.getline(buff, 512);
	
	while(file.getline(buff, 512))
	{
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
		if(length != int(data.size())){
			cerr << "Format Error!\n";
			return;
		}
		raw_data.push_back(data);
	}
	file.close();
}

void
Reader::ReadFromBotList(const std::string& fname)
{
	ifstream file(fname.c_str());
	if(!file.is_open()){
		cout << "Error! Cannot open file " << fname << endl;
		return;
	}
	char buff[512];
	while(file.getline(buff, 512))
	{
		string data = buff;
		botlist.insert(data);
	}
}

const vector< vector<string> >&
Reader::getRawData() const
{
	return raw_data;
}

const set<string>&
Reader::getBotList() const
{
	return botlist;
}

void
Reader::rawToTimelist(vector< vector< vector<string> > >& _timeList)
{
	TimeKey begin = TimeKey(raw_data[0][0]);
	TimeKey end = TimeKey(raw_data[raw_data.size() - 1][0]);
	double delta = end - begin;
	cout.precision(10);
	cout << "The total duration is " << delta << " seconds\n";
	if(args.windowNumber == 0)cout << "Need to assign number of windows\n";
	else cout << "The window size is " << delta / args.windowNumber << " seconds\n";
	
	double window_size = delta / args.windowNumber;
	
	size_t j = 0;
	for(int i = 0; i < args.windowNumber; ++i){
		vector< vector<string> > temp;

		for(; j < raw_data.size(); ++j){
			TimeKey key = TimeKey(raw_data[j][0]);

			if((key - begin) / window_size == i){
				temp.push_back(raw_data[j]);
			}
			else{
				_timeList.push_back(temp);
				break;
			}
		}
	}
}

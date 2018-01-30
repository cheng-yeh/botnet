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
Reader::getData() const
{
	return raw_data;
}

/***********************************************************
  FileName     [ Reader.h ]
  Description  [ reader for formats needed in this project ]
  Author       [ Cheng-Yeh (Gary) Chen ]
  Copyright    [ MIT ]
***********************************************************/
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <set>

#include "util.h"

using namespace std;

class Reader
{
public:
	Reader(void);
	virtual ~Reader(void);
	
	void ReadFromBinetflow(const std::string& fname);
	
	const vector< vector<string> >& getRawData() const;
	void rawToTimelist(vector< vector< vector<string> > >& _timeList);
	
private:
	vector< vector<string> > raw_data;
};


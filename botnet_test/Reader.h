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
	
	vector< vector<string> > getData() const;

private:

private:
	vector< vector<string> > raw_data;
};


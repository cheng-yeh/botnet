/**************************************************
  FileName     [ Writer.h ]
  Description  [ write output to specified  files ]
  Author       [ Cheng-Yeh (Gary) Chen ]
  Copyright    [ MIT ]
**************************************************/
#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>

#include "util.h"

using namespace std;

class Writer
{
public:
	Writer(void);
	Writer(const string& data, string& filename);
	~Writer(void);
	
	// multiple writers
	void GroundTruthWriter(const vector< vector<string> >& data, const vector<string>& bot, string& filename);
	void SADPWriter(const vector< vector<double> > data, string& filename);
	
private:

};

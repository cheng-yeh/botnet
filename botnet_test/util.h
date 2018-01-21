#pragma once

#include <iostream>
#include <cstring>
#include <string>

#include "rnGen.h"

using namespace std;

/*********************************************************************
Author       [ Chung-Yang (Ric) Huang ]
Copyright    [ Copyleft(c) 2007-present LaDs(III), GIEE, NTU, Taiwan ]
*********************************************************************/

size_t StrGetTok(const string& str, string& tok, size_t pos = 0, const char del = ' ');
bool Str2Int(const string& str, int& num);
bool Str2Double(const string& str, double& num);

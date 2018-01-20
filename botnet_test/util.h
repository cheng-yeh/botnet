#pragma once

#include <iostream>
#include <cstring>
#include <string>

using namespace std;

/*********************************************************************
Author       [ Chung-Yang (Ric) Huang ]
Copyright    [ Copyleft(c) 2007-present LaDs(III), GIEE, NTU, Taiwan ]
*********************************************************************/
size_t
StrGetTok(const string& str, string& tok, size_t pos = 0,
            const char del = ' ')
{
   	size_t begin = pos;
   	//cout << "begin = " << begin << " pos = " << pos << endl;
   	if (begin == string::npos) { tok = ""; return begin; }
   	size_t end = str.find_first_of(del, begin);
   	//cout << "end = " << end << endl;
   	tok = str.substr(begin, end - begin);
   	if(end == string::npos)return end;
   	return end + 1;
}

bool
Str2Int(const string& str, int& num)
{
   	num = 0;
   	size_t i = 0;
   	int sign = 1;
   	if (str[0] == '-') { sign = -1; i = 1; }
   	bool valid = false;
   	for (; i < str.size(); ++i) {
      	if (isdigit(str[i])) {
         	num *= 10;
         	num += int(str[i] - '0');
         	valid = true;
      	}
      	else return false;
   	}
   	num *= sign;
   	return valid;
}

bool
Str2Double(const string& str, double& num)
{
	num = 0;
	size_t i = 0;
	size_t zero;
	double temp;
	double sign = 1;
	if (str[0] == '-') { sign = -1; i = 1; }
	bool valid = false;
	for (; i < str.size(); ++i) {
		if (str[i] == '.')break;
		if (isdigit(str[i])) {
			num *= 10;
			num += int(str[i] - '0');
			valid = true;
		}
		else{
			return false;
		}
	}
	zero = i++;
	for (; i < str.size(); ++i) {
		if (isdigit(str[i])) {
			temp = (double)(str[i] - '0');
			for(size_t j = 0; j < i - zero; ++j){
				temp /= 10;
			}
			num += temp;
		}
		else{
			return false;
		}
	}

  	num *= sign;
  	return valid;
}


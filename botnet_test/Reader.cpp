/*                                                                            
    Copyright 2014
    Alexander Belyi <alexander.belyi@gmail.com>,
    Stanislav Sobolevsky <stanly@mit.edu>                                               
                                                                            
    This file is part of Combo algorithm.

    Combo is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Combo is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Combo.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Reader.h"
//#include "util.h"

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
		for(size_t i = 0; i < data.size(); ++i){
			//cout << data[i] << "~~~~";
		}
		cout << data.size() << endl;
		if(length != data.size()){
			cerr << "Format Error!\n";
			return;
		}
		raw_data.push_back(data);
	}
	file.close();
}

vector< vector<string> >
Reader::getData() const
{
	return raw_data;
}

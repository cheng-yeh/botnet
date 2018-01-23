/*                                                                            
    Copyright 2014
    Alexander Belyi <alexander.belyi@gmail.com>,
    Stanislav Sobolevsky <stanly@mit.edu>                                               
                                                                            
    This is the main file of Combo algorithm.

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

#include <ctime>

#include "Reader.h"
#include "GraphDetector.h"
#include "FlowDetector.h"

#include <cmath>
#include <iostream>
#include <algorithm>
using namespace std;

//settings
const bool debug_verify = false;

size_t window_num = 0;

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		cout << "Error: provide path to binetflow(.binetflow) file" << endl;
		return -1;
	}
	if(argc > 2)
		window_num = atoi(argv[2]);
		
	string fileName = argv[1];
	srand(time(0));

	Reader R;
	string ext = fileName.substr(fileName.rfind('.'), fileName.length() - fileName.rfind('.'));

	if(ext == ".binetflow")
		R.ReadFromBinetflow(fileName);

	clock_t startTime = clock();
	
	GraphDetector GD;
	GD.init(R.getData());
	cout << "Start selecting\n";
	GD.selectModel();
	vector<size_t> anomaly;
	GD.detect(anomaly);

	return 0;
}

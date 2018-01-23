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
	if(argc > 3)
		window_num = atoi(argv[3]);
		
	string fileName1 = argv[1];
	string fileName2 = argv[2];
	srand(time(0));

	Reader* original = new Reader();
	Reader* background = new Reader();
	string ext1 = fileName1.substr(fileName1.rfind('.'), fileName1.length() - fileName1.rfind('.'));
	string ext2 = fileName2.substr(fileName2.rfind('.'), fileName2.length() - fileName2.rfind('.'));

	if(ext1 == ".binetflow")
		original -> ReadFromBinetflow(fileName1);
		
	if(ext2 == ".binetflow")
		background -> ReadFromBinetflow(fileName2);

	clock_t startTime = clock();
	
	GraphDetector GD;
	GD.readGraph(original -> getData(), true);
	delete original;
	original = 0;
	cout << "Start\n";
	GD.readGraph(background -> getData(), false);
	cout << "Start selecting\n";
	//GD.selectModel();
	
	cout << "Start detecting\n";
	vector<size_t> anomaly;
	GD.detect(anomaly);

	return 0;
}

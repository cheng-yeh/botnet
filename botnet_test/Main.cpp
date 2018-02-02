/********************************************************
  FileName     [ Main.h ]
  Description  [ The main file for this botnet detector ]
  Author       [ Cheng-Yeh (Gary) Chen ]
  Copyright    [ MIT ]
********************************************************/
#include <ctime>

#include "Reader.h"
#include "GraphDetector.h"
#include "FlowDetector.h"
#include "BotDiscover.h"

#include <cmath>
#include <iostream>
#include <algorithm>
using namespace std;

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
		
	string fileName1 = argv[1];
	srand(time(0));

	Reader* R = new Reader();
	string ext1 = fileName1.substr(fileName1.rfind('.'), fileName1.length() - fileName1.rfind('.'));

	if(ext1 == ".binetflow")
		R -> ReadFromBinetflow(fileName1);
		
	//clock_t startTime = clock();
	
	GraphDetector GD;
	R -> rawToTimelist(GD._timeList);
	GD.timelistToIntergraph();
	cout << "Start\n";
	//GD.readGraph(background -> getData(), false);
	//cout << "Start selecting\n";
	//GD.selectModel();
	
	//cout << "Start detecting\n";
	//vector<size_t> anomaly;
	//GD.detect(anomaly);
	
	BotDiscover BD(GD.getAnomaly(), GD.getTimeList());
	cout << "Start discovering\n";
	cout << "GD.getTimeList() = " << GD.getTimeList().size() << endl;
	//BD.setSCG2();
	return 0;
}

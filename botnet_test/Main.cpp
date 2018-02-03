/********************************************************
  FileName     [ Main.h ]
  Description  [ The main file for this botnet detector ]
  Author       [ Cheng-Yeh (Gary) Chen ]
  Copyright    [ MIT ]
********************************************************/
#include <cmath>
#include <iostream>
#include <stdlib.h>
#include <getopt.h>
#include <algorithm>

#include "Reader.h"
#include "Writer.h"
#include "GraphDetector.h"
//#include "FlowDetector.h"
#include "BotDiscover.h"

using namespace std;

globalArg args;

int main(int argc, char** argv)
{
	// Using Getopt to parse commands.
	// Reference: 
	// https://www.gnu.org/software/libc/manual/html_node/Getopt-Long-Option-Example.html#Getopt-Long-Option-Example
	int c;
	
	while (1)
	{
		static struct option long_options[] =
		{
			/* These options don’t set a flag.
			   We distinguish them by their indices. */
			{"TotalList",   required_argument, 0, 'a'},
			{"AllBot",      no_argument,       0, 'b'},
			{"BotOne",         required_argument, 0, 'c'},
			{"WindowNum",   required_argument, 0, 'd'},
			{"OutputFile",  no_argument,       0, 'e'},
			{"Help",        no_argument,       0, 'h'},
			{0, 0, 0, 0}
		};
		/* getopt_long stores the option index here. */
		int option_index = 0;

		c = getopt_long (argc, argv, "acb:d:", long_options, &option_index);

		/* Detect the end of the options. */
		if (c == -1)
			break;

		switch (c)
		{
			case 0:
				/* If this option set a flag, do nothing else now. */
				if (long_options[option_index].flag != 0)
					break;
				cout << "option " << long_options[option_index].name << endl;
				if (optarg)
					cout << " with arg " << optarg << endl;
				cout << "\n";
				break;

			case 'a':
				args.totalList = optarg;
				break;

			case 'b':
				args.botList = optarg;
				break;

			case 'c':
				args.botOne = optarg;
				break;
			
			case 'd':
				args.windowNumber = atoi(optarg);
				break;
				
			case 'e':
				args.outputFile = optarg;
				break;
			
			case 'h':
				cout << "Usage: <--TotalList <string fileName>>\n       [--BotList [string fileName]]\n"     
				     << "       <--WindowNumber <int window_number>>\n       [--OutputFile <string outputFile>]\n"
				     << "       [--Help]\n";
				break;
				
			case '?':
				/* getopt_long already printed an error message. */
				break;

			default:
				abort ();
		}
	}
	
	Reader* R = new Reader();
	string ext1 = args.totalList.substr(args.totalList.rfind('.'), args.totalList.length() - args.totalList.rfind('.'));

	if(ext1 == ".binetflow")
		R -> ReadFromBinetflow(args.totalList);
		
	// write the correct answer
	Writer W;
	W.GroundTruthWriter( R -> getRawData(), args.botOne, args.outputFile );
	
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

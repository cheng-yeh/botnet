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

Reader* R = 0;
Writer* W = 0;
GraphDetector* GD = 0;
BotDiscover* BD = 0;

static struct option long_options[] =
{
	// parameters to be set
	{"totalList",  			required_argument, 0, 'a'},
	{"allBot",     			required_argument, 0, 'b'},
	{"botOne",     			required_argument, 0, 'c'},
	{"windowNum",  			required_argument, 0, 'd'},
	{"outputFile", 			required_argument, 0, 'e'},
	{"pivotTau", 			required_argument, 0, 'f'},
	{"scgTau",				required_argument, 0, 'g'},
	// choice of operation
	{"Read-File", 			required_argument, 0, 'i'},
	{"Write-File",		 	required_argument, 0, 'j'},
	{"Anomaly-Detection", 	required_argument, 0, 'k'},
	{"Bot-Detection", 		required_argument, 0, 'l'},
	{"Scoring", 			required_argument, 0, 'm'},
	// help message	
	{"help",       			no_argument,       0, 'h'},
	{"Quit",       			no_argument,       0, 'n'},
	{0, 0, 0, 0}
};

// flags for operation order
bool flag_read_flow = false, flag_read_bot = false, flag_write_ref = false, flag_write_result = false,
	 flag_anomaly_flow = false, flag_anomaly_degree = false, flag_bot_scg = false, flag_scoring = false;

// forward declaration of operation functions
bool reading(bool flag);
bool write_ref();
bool write_result();
bool anomaly_flow();
bool anomaly_degree();
bool bot_scg();
bool scoring();
 
int main(int argc, char** argv)
{
	// Using Getopt to parse commands.
	// Reference: 
	// https://www.gnu.org/software/libc/manual/html_node/Getopt-Long-Option-Example.html#Getopt-Long-Option-Example
	int c, pos = 0, mandatory = 0;
	string opt;
	while (1)
	{
		/* getopt_long stores the option index here. */
		int option_index = 0;
		c = getopt_long (argc, argv, "ha:b:c:d:e:f:g:", long_options, &option_index);

		/* Detect the end of the options. */
		if (c == -1)
			break;
		switch (c)
		{
			case 0:
				break;

			case 'a':
				++mandatory;
				args.totalList = optarg;
				break;

			case 'b':
				args.botList = optarg;
				break;

			case 'c':
				opt = optarg;
				while(opt.find(",", pos) != string::npos){
					args.botOne.push_back(opt.substr(pos, opt.find(",", pos) - pos));
					pos = opt.find(",", pos) + 1;
				}
				args.botOne.push_back(opt.substr(pos));
				break;
			
			case 'd':
				++mandatory;
				if(!Str2Int(optarg, args.windowNumber)){
					cout << "Error! The argument of --windowNumber must be an integer!\n";
					return 1;
				}
				break;
				
			case 'e':
				args.outputFile = optarg;
				break;
			
			case 'f':
				if(!Str2Double(optarg, args.pivotTau)){
					cout << "--PivotTau must follow a double.";
					return 1;
				}
				break;
			
			case 'g':
				if(!Str2Double(optarg, args.scgTau)){
					cout << "--ScgTau must follow a double.";
					return 1;
				}
				break;
				
			case 'h':
				cout << "Usage: <--totalList <(fileName)>>               <--windowNumber <(window_number)>>\n"     
				     << "       [--botOne <(bot1_IP,bot2_IP,...)>]       [--botList <(fileName)>]\n"
				     << "       [--outputFile <(outputFile)>]            [--help]\n"
				     << "       [--pivotTau <(threshold)>]               [--scgTau <(threshold)>]\n";
				break;
				
			case '?':
				/* getopt_long already printed an error message. */
				break;

			default:
				abort ();
		}
	}
	if(mandatory != 2){
		cout << "Error! Both --totalList and --windowNumber need to be specified!\n";
		return 1;
	}
		 
	while(1){
		// command line, generate argc and argv for getopt
		char str[255];
		cout << "\nbotnet >> ";
		cin.getline(str, 256, '\n');
		
		size_t pos = 0;
		string tok = " ";
		vector<string> buff;
		while(tok != ""){
			pos = StrGetTok(str, tok, pos, ' ');
			if(tok != "")buff.push_back(tok);
		}
		int _argc = buff.size();
		char* _argv[_argc];
		for(size_t i = 0; i < _argc; ++i){
			char* ch = new char[buff[i].size()];
			for(size_t j = 0; j < buff[i].size(); ++j)
				ch[j] = buff[i][j];
			_argv[i] = ch;
		}
		
cout << "debug" << endl;
cout << argc << "  " << _argc << endl;
		for(size_t i = 0; i < _argc; ++i){
			cout << _argv[i] << endl;
			cout << argv[i] << endl;
		}

		while(1){
			// getopt
			int option_index = 0;
cout << "ch\n";
			c = getopt_long (_argc - 1, _argv, "hni:j:k:l:m:", long_options, &option_index);
cout << "c = " << c << endl;
			/* Detect the end of the options. */
			if (c == -1)
				break;
			switch (c)
			{
				case 0:
					break;
					
				case 'h':
					cout << "Usage: [--Read-File <flow | bot>] | [--Write-File <ref | result>] |\n"     
				     	 << "       [--Anomaly-Detection <flow | degree>] | [--Bot-Detection <scg>] |\n"
				     	 << "       [--Scoring] | [--help] | [--Quit]\n";
					break;

				case 'i':
					if(optarg == "flow")flag_read_flow = reading(true);
					else if(optarg == "bot")flag_read_bot = reading(false);
					else cout << "Command " << optarg << "not found.\n";
					break;

				case 'j':
					if(optarg == "ref")flag_write_ref = write_ref();
					else if(optarg == "result")flag_write_result = write_result();
					else cout << "Command " << optarg << "not found.\n";
					break;
			
				case 'k':
					if(optarg == "flow")flag_anomaly_flow = anomaly_flow();
					else if(optarg == "degree")flag_anomaly_degree = anomaly_degree();
					else cout << "Command " << optarg << "not found.\n";
					break;
				
				case 'l':
					if(optarg == "scg")flag_bot_scg = bot_scg();
					else cout << "Command " << optarg << "not found.\n";
					break;
			
				case 'm':
					flag_scoring = scoring();
					break;
				
				case 'n':
					return 0;
					break;
			
				default:
					cout << "command not found.";
					break;
			}
		}
	}
	return 0;
}

bool reading(bool flag){
	string target;
	
	if(R == 0)R = new Reader();
	if(flag)target = args.totalList;
	else target = args.botList;
	
	string ext1 = target.substr(target.rfind('.'), target.length() - target.rfind('.'));
	if(ext1 == ".binetflow")
		R -> ReadFromBinetflow(target);
	else if(ext1 == ".txt"){
		R -> ReadFromBotList(target);
	}
	else{
		cout << "Input file must be a binetflow file.\n";
		return false;
	}
	return true;
}

bool write_ref(){
	if(flag_read_flow && flag_read_bot){
		if(W == 0)W = new Writer();
		W -> GroundTruthWriter( R -> getRawData(), args.botOne, args.outputFile );
		return true;
	}
	
	cout << "Need to do --Read <flow> and --Read <bot> first.\n";
	return false;
}

bool write_result(){
	
	return false;
}

bool anomaly_flow(){
	if(flag_read_flow){}
	cout << "This part isn't yet complete.\n";
	return false;
}

bool anomaly_degree(){
	if(flag_read_flow){
		if(GD == 0)GD = new GraphDetector();
		R -> rawToTimelist(GD -> _timeList);
		GD -> timelistToIntergraph();
		//GD -> readGraph(R -> getRawData(), false);
		//cout << "Start selecting\n";
		//GD -> selectModel();
		cout << "Start detecting\n";
		vector<size_t> anomaly;
		GD -> detect(anomaly);
		return true;
	}
	return false;
}

bool bot_scg(){
	if(flag_anomaly_degree || flag_anomaly_flow){
		if(BD == 0)BD = new BotDiscover(GD -> getAnomaly(), GD -> getTimeList());
		
		return true;
	}
	return false;
}

bool scoring(){
	if(flag_read_bot){
		bool flag;
		if(flag_anomaly_degree){
			cout << "This part isn't yet complete.\n";
			return false;
		}
		else if(flag_anomaly_flow){
			flag = true;
		}
		if(flag_bot_scg){
			const vector<string>& result = BD -> get_ipList();
			const vector< set<string> >& time = (flag) ? timeList2Set(GD -> getTimeList()) : timeList2Set(GD -> getTimeList());
			
			cout << "FPR = " << FPR(result, R -> getBotList(), time, GD -> getAnomaly());
			cout << "Recall = " << recall(result, R -> getBotList(), time, GD -> getAnomaly());
			cout << "Precision = " << precision(result, R -> getBotList(), time, GD -> getAnomaly());
			cout << "F1-score = " << f1_score(result, R -> getBotList(), time, GD -> getAnomaly());
			cout << "G-score = " << g_score(result, R -> getBotList(), time, GD -> getAnomaly());
			return true;
		}
	}
	return false;
}

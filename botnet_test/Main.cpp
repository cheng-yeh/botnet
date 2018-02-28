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
#include "Combo_beta.h"

using namespace std;

// struct for storing global arguments
globalArg args;

// instance of related classes
Reader R = Reader();
Writer W = Writer();
GraphDetector GD = GraphDetector();
BotDiscover BD;
Combo CB = Combo();
Graph G = Graph();

// options for getopt()
static struct option long_options[] =
{
	// parameters to be set
	{"totalList",           required_argument, 0, 'a'},
	{"allBot",              required_argument, 0, 'b'},
	{"botOne",              required_argument, 0, 'c'},
	{"windowNum",           required_argument, 0, 'd'},
	{"outputFile",          required_argument, 0, 'e'},
	{"pivotTau",            required_argument, 0, 'f'},
	{"scgTau",              required_argument, 0, 'g'},
	// choice of operation
	{"Read-File",           required_argument, 0, 'i'},
	{"Write-File",          required_argument, 0, 'j'},
	{"Anomaly-Detection",   required_argument, 0, 'k'},
	{"Bot-Detection",       required_argument, 0, 'l'},
	{"Combo",               no_argument,       0, 'o'},
	{"Scoring",             no_argument,       0, 'm'},
	// help message	
	{"help",                no_argument,       0, 'h'},
	// exit the whole program
	{"Quit",                no_argument,       0, 'n'},
	{0, 0, 0, 0}
};

// flags for operation precedence
bool flag_allbot = false, flag_botone = false, flag_outputfile = false, flag_read_flow = false, 
     flag_read_bot = false, flag_write_ref = false, flag_write_result = false, flag_anomaly_flow = false,
     flag_anomaly_degree = false, flag_bot_scg = false, flag_combo = false, flag_scoring = false;

// forward declaration of operational functions
bool reading(bool flag);
bool writing(int flag);
bool anomaly_flow();
bool anomaly_degree();
bool bot_scg();
bool combo();
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
			case 'a':
				++mandatory;
				args.totalList = optarg;
				break;

			case 'b':
				args.botList = optarg;
				flag_allbot = true;
				break;

			case 'c':
				opt = optarg;
				while(opt.find(",", pos) != string::npos){
					args.botOne.push_back(opt.substr(pos, opt.find(",", pos) - pos));
					pos = opt.find(",", pos) + 1;
				}
				args.botOne.push_back(opt.substr(pos));
				flag_botone = true;
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
				flag_outputfile = true;
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
				
			default:
				cout << "command not found.";
				break;
		}
	}
	if(mandatory != 2){
		cout << "Error! Both --totalList and --windowNumber need to be specified!\n";
		return 1;
	}
		 
	while(1){
		// command line for operation control
		// generate argc and argv for getopt
		char str[255];
		cout << "\nbotnet >> ";
		cin.getline(str, 256, '\n');
		
		size_t pos = 0;
		string tok = " ";
		vector<string> buff;
		buff.push_back("./botnet");
		while(tok != ""){
			pos = StrGetTok(str, tok, pos, ' ');
			if(tok != "")buff.push_back(tok);
		}
		
		int _argc = buff.size();
		char** _argv = (char**)alloca(_argc);
		for(int i = 0; i < _argc; ++i){
			_argv[i] = (char*)alloca(buff[i].size() + 1);
			memcpy(_argv[i], buff[i].c_str(), buff[i].size() + 1);
		}
		
		// reset parameters in getopt
		optind = 0;
		opterr = 0;
		optopt = 0;
		
		while(1){
			// getopt
			int option_index = 0;
			c = getopt_long (_argc, _argv, "hi:j:k:l:m:n", long_options, &option_index);
			/* Detect the end of the options. */
			if (c == -1)
				break;
			switch (c)
			{
				case 'h':
					cout << "Usage: [--Read-File <flow | bot>] | [--Write-File <ref | result>] |\n"     
				     	 << "       [--Anomaly-Detection <flow | degree>] | [--Bot-Detection <scg>] |\n"
				     	 << "       [--Combo] | [--Scoring] | [--help] | [--Quit]\n";
					break;

				case 'i':
					if(string(optarg) == "flow")flag_read_flow = reading(true);
					else if(string(optarg) == "bot")flag_read_bot = reading(false);
					else cout << "Command " << optarg << " not found.\n";
					break;

				case 'j':
					if(string(optarg) == "ref")flag_write_ref = writing(0);
					else if(string(optarg) == "result")flag_write_result = writing(1);
					else cout << "Command " << optarg << " not found.\n";
					break;
			
				case 'k':
					if(string(optarg) == "flow")flag_anomaly_flow = anomaly_flow();
					else if(string(optarg) == "degree")flag_anomaly_degree = anomaly_degree();
					else cout << "Command " << optarg << " not found.\n";
					break;
				
				case 'l':
					if(string(optarg) == "scg")flag_bot_scg = bot_scg();
					else cout << "Command " << optarg << " not found.\n";
					break;
				
				case 'o':
					flag_combo = combo();
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
	
	if(flag){
		target = args.totalList;
	}
	else{
		if(flag_allbot){
			target = args.botList;
		}
		else{
			cout << "Must provide --botList <(fileName)>.\n";
			return false;
		}
	}
	
	string ext1 = target.substr(target.rfind('.'), target.length() - target.rfind('.'));
	if(flag){
		if(ext1 == ".binetflow")
			R.ReadFromBinetflow(target);
		else{
			cout << "Must provide --totalList <(fileName)> with a binetfile.\n";
			return false;
		}
	}
	else{
		if(ext1 == ".txt" || ext1 == "")
			R.ReadFromBotList(target);
		else{
			cout << "Must provide --botList <(fileName)> with a text file.\n";
			return false;
		}
	}
	return true;
}

bool writing(int flag){
	string excp = "";
	string ext = args.outputFile.substr(args.outputFile.rfind('.'), args.outputFile.length() - args.outputFile.rfind('.'));
	
	if(flag_read_flow && flag_outputfile){
		if(flag == 0){
			if(flag_botone){
				if(ext == ".txt"){
					W.GroundTruthWriter( R.getRawData(), args.botOne, args.outputFile );
					return true;
				}
				else{
					cout << "OutputFile must be a .txt file.\n";
					return false;
				}
			}
			else excp += " [--BotOne <(bot1_IP, bot2_IP,...)>]";
		}
		else{
			if(flag_bot_scg){
				if(ext == ".dat"){
					W.SADPWriter( BD.get_SCG(), args.outputFile );
					return true;
				}
				else{
					cout << "OutputFile must be a .dat file.\n";
					return false;
				}
			}
			else excp += " do [--Bot-Detection <scg>]";
		}
	}
	
	cout << "Need to set up [--OutputFile <(filename)>]," << excp << " and do [--Read <flow>] first.\n";
	return false;
}

bool write_result(){
	
	return false;
}

bool anomaly_flow(){
	if(flag_read_flow){}
	cout << "This part isn't yet complete.\n";
	cout << "Need to do [--Read <flow>] first.\n";
	return false;
}

bool anomaly_degree(){
	if(flag_read_flow){
		R.rawToTimelist(GD._timeList);
		cout << "timeList.size() = " << (GD._timeList).size() << endl;
		cout << "timeList[0].size() = " << (GD._timeList[0].size()) << endl;
		GD.timelistToIntergraph();
		//GD.readGraph(R.getRawData(), false);
		//cout << "Start selecting\n";
		//GD.selectModel();
		cout << "Start detecting\n";
		vector<size_t> anomaly;
		cout << "Check degree\n";
		GD.detect(anomaly);
		return true;
	}
	
	cout << "Need to do [--Read <flow>] first.\n";
	return false;
}

bool bot_scg(){
	if(flag_anomaly_degree || flag_anomaly_flow){
		if(!flag_bot_scg)BD = BotDiscover(GD.getAnomaly(), GD.getTimeList());
		return true;
	}
	cout << "Need to do [--Anomaly <flow>] or [--Anomaly <degree>] first.\n";
	return false;
}

bool combo(){
	if(flag_bot_scg){
		G.setMatrix(BD.get_SCG());
cout << "c1\n";
		CB.setPivotalInteraction(BD.convert_pivotalInteraction());
cout << "c2\n";
		CB.setWeight(1e-6, 1e-6);
cout << "c3\n";
		CB.RunCombo(G, size_t(2));
		cout << "Modularity = " << G.Modularity() << endl;
G.Print();
		return true;
	}
	cout << "Need to do [--Bot-Detection <scg>] first.\n";
	return false;
}
bool scoring(){
	if(flag_read_bot){
		bool flag;
		if(flag_anomaly_flow){
			cout << "This part isn't yet complete.\n";
			return false;
		}
		else if(flag_anomaly_degree){
			flag = true;
		}
		if(flag_bot_scg){
			const vector<string>& result = BD.get_ipList();
			const vector< set<string> >& time = (flag) ? timeList2Set(GD.getTimeList()) : timeList2Set(GD.getTimeList());
			
			cout << "FPR = " << FPR(result, R.getBotList(), time, GD.getAnomaly());
			cout << "Recall = " << recall(result, R.getBotList(), time, GD.getAnomaly());
			cout << "Precision = " << precision(result, R.getBotList(), time, GD.getAnomaly());
			cout << "F1-score = " << f1_score(result, R.getBotList(), time, GD.getAnomaly());
			cout << "G-score = " << g_score(result, R.getBotList(), time, GD.getAnomaly());
			return true;
		}
	}
	cout << "Need to do [--Bot-Detection <scg>] and [--Read <bot>] first.\n";
	return false;
}

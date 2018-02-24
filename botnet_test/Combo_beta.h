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

#pragma once

#include <iostream>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <map>

#include "Graph.h"

#define INF 1000000000
#define THRESHOLD 1e-6

using namespace std;

class Combo{
public:
	Combo(double w1 = 1, double w2 = 1);
	~Combo();
	void setInteraction(const map<string, double>& total);
	void RunCombo(Graph& G, size_t max_comunities);

private:
	void reCalc(Graph& G, vector< vector<double> >& moves, vector< vector<int> >& splits_communities, size_t origin, size_t dest);
	double Split(vector< vector<double> >& Q, const vector<double>& correctionVector, vector<int>& splitCommunity);
	double PerformKernighansShift(const vector< vector<double> >& Q, const vector<double>& correctionVector, const vector<int>& communitiesOld, vector<int>& communitiesNew);
	double ModGain(const vector< vector<double> >& Q, const vector<double>& correctionVector, const vector<int>& community);
	double BestGain(const vector< vector<double> >& moves, size_t& origin, size_t& dest);
	void DeleteEmptyCommunities(Graph& G, vector< vector<double> >& moves, vector< vector<int> >& splits_communities, int origin);
	vector<double> Sum(const vector< vector<double> >& matrix);
	
private:
	// settings
	const bool debug_verify = false;
	const int RAND_MAX2 = RAND_MAX >> 1;
	const double autoC1 = 2;
	const double autoC2 = 1.5;
	bool use_fixed_tries = false;
	double best_gain = 1.0;
	
	// parameters
	map<string, double> _total;
	double _w1, _w2;
	 
	
};

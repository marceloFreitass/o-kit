#pragma once

#include <vector>
#include <utility>
#include <iostream>
#include <numeric>
#include <iomanip>
using namespace std;


#define EPSILON 0.00000001


class Node{



	private:
		vector<vector<bool>> A;
		vector<double> lambdaValues;
		long double bins;

		vector<pair<int, int>> together;
		vector<pair<int, int>> separated;
		
		bool feasible;
		bool isRoot;


	public:
		
		Node(bool isRoot);
		Node();
		
		void setLambdaValues(vector<double> lambdaValues);
		void setA(vector<vector<bool>> A);
		void setTogetherPairs(vector<pair<int, int>> together);
		void setSeparatedPairs(vector<pair<int, int>> separated);
		void setFeasible();
		void setBins(double bins);

		void addTogetherPair(pair<int, int> t);
		void addSeparatedPair(pair<int, int> s);
		
		vector<pair<int, int>> getTogether();
		vector<pair<int, int>> getSeparated();
		bool getType();
		bool getFeasible();
		long double getBins();
		vector<vector<bool>> getA();
		vector<double> getLambdasValues();
};
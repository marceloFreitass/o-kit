#ifndef KRUSKAL_H
#define KRUSKAL_H

#include <cstdio>
#include <iostream>
#include <vector>
#include <queue>
#include <utility>

using namespace std;

typedef pair<int, int> ii;
typedef vector <vector<double> >vvi;
typedef vector<ii> vii;

#define INFINITE 99999999

class Kruskal{
public:

	Kruskal(vvi dist);
	Kruskal();

	double MST(int nodes);
	double oneMST(vvi dist, int nodes);
	vii getEdges();
	vector<vector<int>> getSolution(int size);


private:

	priority_queue <pair<double,ii> > graph;
	vector <int> pset;
	vii edges;

	void initDisjoint(int n);
	int findSet(int i);
	void unionSet(int i, int j);
	bool isSameSet(int i, int j);
	
};

#endif
#include "Kruskal.h"

Kruskal::Kruskal(vvi dist){
	for(int i = 0; i < dist.size(); ++i){
		for(int j = 0; j < dist[i].size(); ++j){
			graph.push( make_pair(-dist[i][j], make_pair(i, j)) );
		}	
	}
	
}

void Kruskal::initDisjoint(int n){
	pset.resize(n);
	for (int i = 0; i < n; ++i){
		pset[i] = i;
	}
}

int Kruskal::findSet(int i){
	return (pset[i] == i) ? i : (pset[i] = findSet(pset[i]));
}

void Kruskal::unionSet(int i, int j){
	pset[findSet(i)] = findSet(j);
}

bool Kruskal::isSameSet(int i, int j){
	return (findSet(i) == findSet(j))? true:false;
}

vii Kruskal::getEdges(){
	return edges;
}

double Kruskal::MST(int nodes){
	initDisjoint(nodes);

	double cost = 0;

	while(!graph.empty()){
		pair<double, ii> p = graph.top();
		graph.pop();

		if(!isSameSet(p.second.first, p.second.second)){
			edges.push_back(make_pair(p.second.first, p.second.second));
			cost += (-p.first);
			unionSet(p.second.first, p.second.second);
		}
	}

	return cost;
}

vector<vector<int>> Kruskal::getSolution(int size){
	vector<vector<int>> solution(size, vector<int>(size));
	for(int i = 0; i < edges.size(); i++){
		solution[edges[i].first][edges[i].second] = 1;
		solution[edges[i].second][edges[i].first] = 1;
	}

	return solution;

}

double Kruskal::oneMST(vvi dist, int nodes){
	initDisjoint(nodes);
	//DA pra otimizar, inves de pegar a copia do graph, ja fazer o fakegraph direto e dps ajustar os custos para pegar o original
	double cost = 0;
	priority_queue <pair<double,ii>> fakeGraph;
	priority_queue <pair<double,ii>> temp = graph;
	for(int i = 0; i < dist.size(); ++i){
		for(int j = 0; j < dist[i].size(); ++j){
			if(i == 0 || j == 0){
				fakeGraph.push( make_pair(-INFINITE, make_pair(i, j)));
				continue;
			}
			fakeGraph.push( make_pair(-dist[i][j], make_pair(i, j)) );
		}	
	}
	
	vector <pair<double,ii>> vertice0costs;
	while(!temp.empty()){

		pair<double, ii> p = temp.top();
		if(p.second.first == 0 || p.second.second == 0){
			vertice0costs.push_back(p);
		}
		
		temp.pop();
	}
	
		while(!fakeGraph.empty()){
			pair<double, ii> p = fakeGraph.top();
			fakeGraph.pop();

			if(!isSameSet(p.second.first, p.second.second)){
				edges.push_back(make_pair(p.second.first, p.second.second));
				cost += (-p.first);
				unionSet(p.second.first, p.second.second);
		}
	}
	
	cost -= INFINITE;
	edges.pop_back();

	for(int i = 0; i <= 2; i += 2){
		cost += (-vertice0costs[i].first);
		edges.push_back(vertice0costs[i].second);
	}

	return cost;
}
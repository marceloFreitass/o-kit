#include "Kruskal.h"
#include "Data.h"
#include "Node.h"
#include "Lagrange.h"
#include <vector>
#include <utility>
#include <ctime>
#include <chrono>
#include <list>

using namespace std;

bool operator < (Node a, Node b){
	return a.getLB() > b.getLB();
}

int getOptCost(std::string instanceName){ //Getting the cost from ILS heuristic

	std::string path = "ILS.txt";
	string word = instanceName;
    std::string line;
    ifstream inTSP(path);

    while(getline(inTSP, line)){
		size_t pos = line.find(word, 0);

		if(pos != string::npos){
			line = line.substr(line.find(":") + 1);
			return stoi(line);
		}
	}
	return -1;
}


void setInfiniteCosts(Node* node, int dimension, vector<vector<double>> originalCost, vector<vector<double>> &newCost){ //Covers the forbidden edges on a copy of the original cost matrix, setting the edges cost in matrix to infinite

	newCost = originalCost;
	
	int n =  node->getForbiddenEdges().size();
	vector<pair<int, int>> FE = node->getForbiddenEdges();
	for(int i = 0; i < n; i++){
		newCost[node->getForbiddenEdges()[i].first][node->getForbiddenEdges()[i].second] = INFINITE;
		newCost[node->getForbiddenEdges()[i].second][node->getForbiddenEdges()[i].first] = INFINITE;
	}
}

Node BB(int type, int dimension, vector<vector<double>> cost, int optCost){ //Returns the node with the optimal solution

	Node root, optSolution;
	vector<Node> tree;
	priority_queue<Node> bestBoundTree;
	double upperBound = optCost + 1; //UB starts with optCost from ILS +1
	double lowerBound;

	vector<vector<double>> copyCost = cost;
	Lagrange dual(upperBound, cost);
	
	//Solves the root problem and setting its members
	dual.subgradientMethod(root);
	root.setFeasible();
	root.setChosen();
	
	optSolution = root;
	
	if(type <= 2){
		tree.push_back(root);
	}
	else{
		bestBoundTree.push(root);
	}

	while(!tree.empty() || !bestBoundTree.empty()){
		Node currentNode;
		//Depth
		//Choosing and erasing the last node
		if(type == 1){
			currentNode = tree.back();
			tree.pop_back(); 
		}
		//Breadth
		//Chosing and erasing the first node
		else if(type == 2){
			currentNode = tree.front();
			
		}
		//Best bound
		//Chosing and erasing the node with highest lower bound
		else{
			currentNode = bestBoundTree.top();
			bestBoundTree.pop();
		}

		if(currentNode.getFeasible()){ //If finds a feasible solution, checks if your LB <= UB, if so, thats the new UB
			//And thats the optimal solution
			if(currentNode.getLB() < upperBound){
				upperBound = currentNode.getLB();
				//cout << "A\n";
				dual.setUpperBound(upperBound + 1);
				optSolution = currentNode; 
				}
			}
		
		else{ //Branching if not feasible

			//Get all chosen edges
			vector<pair<int, int>> chosenEdges = currentNode.getChosenEdges();
			int n = chosenEdges.size();
			
			for(int i = 0; i < n; i++){
				//Loops through the largest degree vertice, creating a new node based on forbidden edges of that vertix
				Node newNode;
				newNode.setForbiddenEdges(currentNode.getForbiddenEdges());
				newNode.setMultipliers(currentNode.getMultipliers());
				dual.setMultipliers(currentNode.getMultipliers());
				newNode.addForbiddenEdge(make_pair(chosenEdges[i].first, chosenEdges[i].second));

				setInfiniteCosts(&newNode, dimension, cost, copyCost);
				
				dual.setOriginalCost(copyCost);
				dual.subgradientMethod(newNode);
				
				
				if(upperBound - newNode.getLB() > 0.000001){ // Lower bound has to be less or equal to UB, if not, the node isnt added
					newNode.setFeasible();
					newNode.setChosen();

					if(type <= 2){
						tree.push_back(newNode);
					}
					else{
						bestBoundTree.push(newNode);
					}
					
				}
			}
		}

		if(type == 2){
			tree.erase(tree.begin());
		}
	
		
	}
	if(!optSolution.getFeasible()){
		cout << "NOT FEASIBLE SOLUTION\n";
	}
	return optSolution;
}



int main(int argc, char **argv){
	Node otimo;
	Kruskal kruskal;
    Data * data = new Data(argc, argv[1]);
	data->readData();
	string instance = data->getInstanceName();
	instance = instance.substr(instance.rfind('\\') + 1); //Getting the instance name
	int optCost = getOptCost(instance);


	vector<vector<double>> cost;

	for(int i = 0; i < data->getDimension(); i++){
		vector<double>temp;
		for(int j = 0; j < data->getDimension(); j++){
			temp.push_back(data->getDistance(i,j));
		}
		cost.push_back(temp);
	}
    

	auto begin = chrono::high_resolution_clock::now(); 
	otimo = BB(1, data->getDimension(), cost, optCost);
	auto end = chrono::high_resolution_clock::now();

	auto time = chrono::duration_cast<chrono::milliseconds>(end - begin);

	cout << "Cost: " << otimo.getLB() << endl;
	cout << "Time: " << time.count()/1000.0 << endl;
    
}
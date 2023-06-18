#include "Kruskal.h"
#include "Data.h"
#include "Node.h"
#include "Lagrange.h"
#include <vector>
#include <utility>

using namespace std;

//parei na hora de setar de proibir propriamente na matriz

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


void setForbiddenArcs(Node* node, int dimension, vector<vector<double>> originalCost, vector<vector<double>> &newCost){ //Covers the forbidden arcs on a copy of the original cost matrix, setting the arcs cost in matrix to infinite

	for(int i = 0; i < dimension; i++){
		for(int j = 0; j < dimension; j++){
			newCost[i][j] = cost[i][j];
		}
	}
	
	int n =  node->getForbiddenArcs();

	for(int i = 0; i < n; i++){
		newCost[node->forbiddenArcs[i].first - 1][node->forbiddenArcs[i].second - 1] = INFINITE;
	}
}

Node BB(int type, int dimension, vector<vector<double>> cost, int optCost){ //Returns the node with the optimal solution

	Node root, optSolution;
	vector<Node> tree;
	priority_queue<Node> bestBoundTree;
	double upperBound = optCost + 1	; //UB starts with optCost from ILS +1
	double lowerBound;

	vector<vector<double>> copyCost = cost;
	Lagrange dual(upperBound, cost);
	dual.subgradientMethod(root);
	//Solves the root problem and setting its members
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
			tree.erase(tree.end()); 
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
				optSolution = currentNode; 
				}
			}
		
		else{
			//Get all chosen edges
			vector<pair<int, int>> chosenEdges = node.getChosenEdges();
			int n = chosenEdges.size();
			
			for(int i = 0; i < n - 1; i++){
				//Loops through the smallest subtour, creating a new node based on forbidden arcs of that subtour
				Node newNode;
				newNode.setForbiddenEdges(currentNode.getForbiddenEdges());

				newNode.addForbiddenEdges(chosenEdges[i]);
				pair<int, int> forbiddenArcs;
				//forbiddenArcs = make_pair(chosenE[i],currentNode.subtours[currentNode.chosen][i+1]);
				

				setForbiddenArcs(&newNode, dimension, cost, copyCost); //create a copy of the original matrix, setting the arcs values on the copy matrix to infinite
				
				hungarian_problem_t p = getMatrixSolutionHungarian(dimension, copyCost, lowerBound); //Solve the problem with the copy matrix
				if(lowerBound <= upperBound){ // Lower bound has to be less or equal to UB, if not, the node isnt added
					newNode.subtours = getSubtours(p);
					newNode.lowerBound = lowerBound;
					newNode.feasible = (newNode.subtours.size() == 1);
					newNode.chosen = smallerSubtour(newNode.subtours);
					
					if(type <= 2){
						tree.push_back(newNode);
					}
					else{
						bestBoundTree.push(newNode);
					}
					
				}

				hungarian_free(&p);		
			}
		}
		if(type == 2){
			tree.erase(tree.begin());
		}
	
		
	}

	for (int i = 0; i < dimension; i++) delete [] copyCost[i];
	delete [] copyCost;
	return optSolution;
}



int main(int argc, char **argv){
	//Ajeitar a gambiarra no getSolution;
	
    Data * data = new Data(argc, argv[1]);
	data->readData();
	string instance = data->getInstanceName();
	instance = instance.substr(instance.rfind('\\') + 1); //Getting the instance name
	int optCost = getOptCost(instance);
	vector<vector<double>> cost;
	vector<pair<int,int>> pares;
	vector<int> dummy;
	vector<vector<int>> solution;

	for(int i = 0; i < data->getDimension(); i++){
		vector<double>temp;
		for(int j = 0; j < data->getDimension(); j++){
			temp.push_back(data->getDistance(i,j));
		}
		cost.push_back(temp);
	}
    
    for(int i = 0; i < cost.size(); i++){
		for(int j = 0; j < cost[i].size(); j++){
			cout << cost[i][j] << " ";
		}
		cout << endl;
	}

	Kruskal teste(cost);
	cout << "Custo fake: " << teste.oneMST(cost, cost.size()) << endl;
	solution = teste.getSolution(data->getDimension());
	pares = teste.getEdges();

	for(int i = 0; i < pares.size(); i++){
		cout << pares[i].first << "->" << pares[i].second << endl;
	}
	vector<double> a(data->getDimension(), 0);
	Lagrange lagrange(optCost + 1, cost);

	Node root;
	lagrange.subgradientMethod(root);
	root.setChosen();
	



	
    
}
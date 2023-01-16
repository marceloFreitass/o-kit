#include <iostream>
using namespace std;

#include "Data.h"
#include "hungarian.h"
#include <vector>
#include <numeric>
#include <algorithm>
#include <list>

struct Node{

	vector<pair<int, int>> forbiddenArcs;
	vector<vector<int>> subtours;
	double lowerBound; //objective value of a hungarian resolution
	int chosen; //index of smaller subtour on subtours vector
	bool feasible;
};

void showSubtours(vector<vector<int>> subtours){

	int n = subtours.size();
	for(int i = 0; i < n; i++){
		for(int j = 0; j < subtours[i].size(); j++){

			if(j == subtours[i].size() - 1){
				cout << subtours[i][j];
			}
			else{
				cout << subtours[i][j] << "->";
			}
		}
		cout << endl;
	}
}
vector<vector<int>> getSubtours(hungarian_problem_t pointer){
	vector<vector<int>> totalSubtours;
	vector<int> subtour;
	vector<int> acess(pointer.num_rows + 1);
	vector<int>::iterator Iindex;
	int index;
	int flag_passag = 0; //Checks if acessed every "i"
	for(int i = 0; i < pointer.num_rows; i++){
		
		if(i == 0){ //Initializing the first subtour, starting with 1
			subtour.push_back(i+1);
			acess[i + 1] = 1;
		}

		for(int j = 0; j < pointer.num_cols;){
			if(pointer.assignment[i][j] == 1){
				subtour.push_back(j + 1); //Adds on subtour
				
				if(acess[j + 1] == 1){ //If its acess before, ends the subtour
					totalSubtours.push_back(subtour);
					subtour.clear();
					if(flag_passag == pointer.num_rows){ //Checks flag
						return totalSubtours;
					}

					Iindex = find(acess.begin() + 1, acess.end(), 0); //Find the first element that wasnt acessed yet
					index = Iindex - acess.begin();
					subtour.push_back(index); //its from there that will starts next subtour
					acess[index] = 1;
					flag_passag = accumulate(acess.begin(), acess.end(), 0); //Sums all
					i = index - 1;
					
				}
				else{ //If not, now its acessed and i will jump for the element j
					acess[j + 1] = 1;
					flag_passag = accumulate(acess.begin(), acess.end(), 0); //Sums all
					i = j;
					j = 0;
				}
			}
			else{ //While not found 1
				j++;
			}
		}
	}

	return totalSubtours;
}

hungarian_problem_t getMatrixSolutionHungarian(int dimension, double **matrix, double& obj_value){ //Solves the problem basis on a cost matrix, and returns a pointer to it, also passes obj_value by reference

	hungarian_problem_t p;
	int mode = HUNGARIAN_MODE_MINIMIZE_COST;
	hungarian_init(&p, matrix, dimension, dimension, mode);

	obj_value = hungarian_solve(&p);

	return p;
}


int smallerSubtour(vector<vector<int>> subtours){ //Returns the index of the first smallest subtour on subtours matrix

	int smallerI;
	int smaller;
	int n = subtours.size();

	for(int i = 0; i < n; i++){
		if(i == 0){
			smaller = subtours[i].size();
			smallerI = i;
			continue;
		}
		if(subtours[i].size() < smaller){
			smaller = subtours[i].size();
			smallerI = i;
		}
	}

	return smallerI;
}


void setForbiddenArcs(Node* node, int dimension, double** cost, double** newCost){ //Covers the forbidden arcs on a copy of the original cost matrix, setting the arcs cost in matrix to infinite

	for(int i = 0; i < dimension; i++){
		for(int j = 0; j < dimension; j++){
			newCost[i][j] = cost[i][j];
		}
	}
	
	int n =  node->forbiddenArcs.size();

	for(int i = 0; i < n; i++){
		newCost[node->forbiddenArcs[i].first - 1][node->forbiddenArcs[i].second - 1] = INFINITE;
	}
}
void BB(int dimension, double **cost){

	Node root, optSolution;
	
	double upperBound = INFINITE;
	double lowerBound;

	hungarian_problem_t p = getMatrixSolutionHungarian(dimension, cost, lowerBound);
	double **copyCost = new double*[dimension];
	for(int i = 0; i < dimension; i++){
		copyCost[i] = new double[dimension];
		for(int j = 0; j < dimension; j++){
			copyCost[i][j] = cost[i][j];
		}
	}
	//Solves the root problem and setting its members
	root.subtours = getSubtours(p);
	root.lowerBound = lowerBound;
	root.feasible = (root.subtours.size() == 1);
	root.chosen = smallerSubtour(root.subtours);
	optSolution = root;
	hungarian_free(&p);

	vector<Node> tree;
	tree.push_back(root);

	while(!tree.empty()){
		//Profundidade
		//Choosing and erasing the last node
		Node currentNode = tree.back();
		tree.erase(tree.end()); 

		if(currentNode.feasible){ //If finds a feasible solution, checks if your LB <= UB, if so, thats the new UB
			//And thats the optimal solution
			if(currentNode.lowerBound < upperBound){
				upperBound = currentNode.lowerBound;
				optSolution = currentNode; 
				}
			}
		
		else{
			
			int n = currentNode.subtours[currentNode.chosen].size();
			for(int i = 0; i < n - 1; i++){
				//Loops through the smallest subtour, creating a new node based on forbidden arcs of that subtour
				Node newNode;
				newNode.forbiddenArcs = currentNode.forbiddenArcs;

				pair<int, int> forbiddenArcs;
				forbiddenArcs = make_pair(currentNode.subtours[currentNode.chosen][i],currentNode.subtours[currentNode.chosen][i+1]);
				
				newNode.forbiddenArcs.push_back(forbiddenArcs);

				setForbiddenArcs(&newNode, dimension, cost, copyCost); //create a copy of the original matrix, setting the arcs values on the copy matrix to infinite
				
				hungarian_problem_t p = getMatrixSolutionHungarian(dimension, copyCost, lowerBound); //Solve the problem with the copy matrix
				if(lowerBound <= upperBound){ // Lower bound has to be less or equal to UB, if not, the node isnt added
					newNode.subtours = getSubtours(p);
					newNode.lowerBound = lowerBound;
					newNode.feasible = (newNode.subtours.size() == 1);
					newNode.chosen = smallerSubtour(newNode.subtours);
					
					tree.push_back(newNode);
				}

				hungarian_free(&p);		
			}
			
		}
	}
	cout << "Cost: " << optSolution.lowerBound << endl;
}


int main(int argc, char** argv) {

	vector<vector<int>> Subtours;
	

	Data * data = new Data(argc, argv[1]);
	data->readData();

	double **cost = new double*[data->getDimension()];
	for (int i = 0; i < data->getDimension(); i++){
		cost[i] = new double[data->getDimension()];
		for (int j = 0; j < data->getDimension(); j++){
			cost[i][j] = data->getDistance(i,j);
		}
	}
	

	
	BB(data->getDimension(), cost);

	
	for (int i = 0; i < data->getDimension(); i++) delete [] cost[i];
	delete [] cost;
	delete data;

	return 0;
}
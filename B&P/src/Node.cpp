#include "Node.h"

Node::Node(bool isRoot){

	feasible = 0;
	this->isRoot = isRoot;
};

Node::Node(){
	feasible = 0;
}

void Node::setLambdaValues(vector<double> lambdaValues){
	this->lambdaValues = lambdaValues;
}

void Node::setBins(double bins){
	this->bins = bins;
}

void Node::setFeasible(){
	//cout << "ENTROU NO SET VIAVEL\n";
	// cout << setprecision (15) << "CAIXAS: " << bins << endl;
	// cout << "CAIXAS(int): " << (long int)bins << endl;
	// cout << "CONTA: " << abs(bins - (long int)bins) << endl;
	// feasible = (abs(bins - int(bins)) <= EPSILON); //Integer solution
	// cout << "FEASIBLE: " << feasible << endl;

	int sumInt = 0;
	double sum = 0;

	for(int i = 0; i < lambdaValues.size(); i++){
		sumInt += lambdaValues[i];
		sum += lambdaValues[i];
	}
	//cout << "soma inteira: " << sumInt << endl;
	//cout << "soma: " << sum << endl;

	feasible = (abs(sumInt - sum) <= EPSILON);
	//cout << "FEASIBLE: " << feasible << endl;
}

void Node::setA(vector<vector<bool>> A){
	this->A = A;
}

void Node::setTogetherPairs(vector<pair<int, int>> together){
	this->together = together;
}

void Node::setSeparatedPairs(vector<pair<int, int>> separated){
	this->separated = separated;
}

void Node::addSeparatedPair(pair<int, int> s){
	separated.push_back(s);
}

void Node::addTogetherPair(pair<int, int> t){
	together.push_back(t);
}

vector<pair<int, int>> Node::getTogether(){return together;}
vector<pair<int, int>> Node::getSeparated(){return separated;}
bool Node::getType(){return isRoot;}
bool Node::getFeasible(){return feasible;}
long double Node::getBins(){return bins;}
vector<vector<bool>> Node::getA(){return A;}
vector<double> Node::getLambdasValues(){return lambdaValues;}

#include "Node.h"

Node::Node(bool isRoot){

	feasible = 0;
	this->isRoot = isRoot;
};

Node::Node(){
	feasible = 0;
}

void Node::setBins(double bins){
	this->bins = bins;
}

void Node::setFeasible(){

	feasible = ((int)(bins) == bins); //Integer solution
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

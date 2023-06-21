#include "Node.h"

Node::Node(){
    lowerBound = 999999;
    feasible = 0;
}

void Node::setMST(vector<pair<int,int>> MST){
    this->MST = MST;
}
void Node::addForbiddenEdge(pair<int, int> arc){
    forbiddenEdges.push_back(arc);
}

void Node::setForbiddenEdges(vector<pair<int, int>> forbiddenEdges){
    this->forbiddenEdges = forbiddenEdges;
}

void Node::setLB(double lowerBound){
    this->lowerBound = lowerBound;
}

void Node::setSubgradient(vector<int> subgradient){
    this->subgradient = subgradient;
}

void Node::setMultipliers(vector<double> multipliers){
    this->multipliers = multipliers;
}

void Node::setChosen(){
    int highestDegree;
    int highestDegreeIndex;

    int n = subgradient.size();
    for(int i = 0; i < n; i++){
        if(i == 0){
            highestDegree = subgradient[i];
            highestDegreeIndex = i;
            continue;
        }
        if(subgradient[i] <= highestDegree){ //more negative, bigger degree
            highestDegree = subgradient[i];
            highestDegreeIndex = i;
        }
    }

    chosen = highestDegreeIndex;
}

void Node::setFeasible(){
    
    int n = subgradient.size();
    for(int i = 0; i < n; i++){
        if(subgradient[i] != 0){
            feasible = 0;
            return;
        }
    }
    feasible = 1;
}

vector<pair<int, int>> Node::getMST(){
    return MST;
}

vector<int> Node::getSubgradient(){
    return subgradient;
}

double Node::getLB(){
    return lowerBound;
}

bool Node::getFeasible(){
    return feasible;
}

vector<double> Node::getMultipliers(){
    return multipliers;
}

vector<pair<int, int>> Node::getForbiddenEdges(){
    return forbiddenEdges;
}

vector<pair<int, int>> Node::getChosenEdges(){

    int n = MST.size();
    
    vector<pair<int, int>> chosenEdges;
    for(int i = 0; i < n; i++){
        if(MST[i].first == chosen || MST[i].second == chosen){
            chosenEdges.push_back(MST[i]);
        }
    }

    return chosenEdges;
}
#include "Lagrange.h"


using namespace std;

Lagrange::Lagrange(double upperBound, vector<vector<double>> originalCost){

    EPSILON = 1;
    setUpperBound(upperBound);
    setOriginalCost(originalCost);
    lagrangianCosts = originalCost;
    
    int n = originalCost.size();
    vector<double> initialMultiplier(n, 0); // multipliers <- 0
    setMultipliers(initialMultiplier);
}

void Lagrange::setOriginalCost(vector<vector<double>> originalCost){
    this->originalCost = originalCost;
}

void Lagrange::setLagrangianCosts(vector<vector<double>> lagrangianCosts){
    this->lagrangianCosts = lagrangianCosts;
}

void Lagrange::setSubgradient(vector<int> subgradient){
    this->subgradient = subgradient;
}

void Lagrange::setMultipliers(vector<double> multipliers){
    this->multipliers = multipliers;
}

void Lagrange::setStepsize(double stepSize){
    this->stepSize = stepSize;
}

void Lagrange::setUpperBound(double upperBound){
    this->upperBound = upperBound;
}

vector<int> Lagrange::getDegree(Node* node){

    vector<int> degrees(originalCost.size(), 0);
    int degree = 0;
    vector<pair<int, int>> MST = node->getMST();
    int n = MST.size();
    for(int i = 0; i < n; i++){
        degrees[MST[i].first]++;
        degrees[MST[i].second]++;
    }
    

    return degrees;
}

void Lagrange::calculateSubgradient(Node& node){

    vector<int> subgradient;
    vector<int> degrees = getDegree(&node);

    int n = degrees.size();

    for(int i = 0; i < n; i++){
        subgradient.push_back(2 - degrees[i]);
    }

    setSubgradient(subgradient);
    node.setSubgradient(subgradient);
}

void Lagrange::calculateStepSize(Node* node){

    double stepSize;
    int squareSubgradientTotal = 0;
    vector<int> subgradient = node->getSubgradient();
    int n = subgradient.size();

    if(checkSubgradient()){  //if subgradient <- 0, then squareSubgradientTotal = 0, avoiding division per 0
        stepSize = 0;
        setStepsize(stepSize);
        return;
    }
    for(int i = 0; i < n; i++){
        squareSubgradientTotal += (subgradient[i] * subgradient[i]);
    }

    stepSize = EPSILON * ((upperBound - node->getLB())/squareSubgradientTotal);
    setStepsize(stepSize);
}

void Lagrange::updateMultipliers(Node& node){

    vector<double> nextMultipliers = node.getMultipliers();
    int n = nextMultipliers.size();
    vector<int> subgradient = node.getSubgradient();
    
    for(int i = 0; i < n; i++){

        nextMultipliers[i] += stepSize * subgradient[i];
        
    }

    node.setMultipliers(nextMultipliers);
    setMultipliers(nextMultipliers);
    
}

void Lagrange::updateLagrangianCosts(){

    
    int n = originalCost.size();

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            lagrangianCosts[i][j] = originalCost[i][j] - multipliers[i] - multipliers[j];
        }
    }
}

bool Lagrange::checkMultipliers(){

    int n = multipliers.size();

    for(int i = 0; i < n; i++){
        if(multipliers[i] != 0){
            return false;
        }
    }
    return true;
}

bool Lagrange::checkSubgradient(){

    int n = subgradient.size();

    for(int i = 0; i < n; i++){
        if(subgradient[i] != 0){
            return false;
        }
    }
    return true;
}

void Lagrange::subgradientMethod(Node& node){
    
    int n = originalCost.size();
    vector<double> bestMultipliers;
    vector<int> bestSubgradient;
    vector<pair<int, int>> bestMST;
    
    EPSILON = 1;
    double EPSILON_MIN = 0.0005;
    double bestLB = -numeric_limits<double>::infinity();
    int k = 0;

    Kruskal *kruskal;
    while(1){
        
        kruskal = new Kruskal();

        double LB = kruskal->oneMST(lagrangianCosts, lagrangianCosts.size());
        
        node.setMST(kruskal->getEdges());
        node.setMultipliers(multipliers); 
        for(int i = 0; i < n; i++){
            LB += 2 * multipliers[i]; //cost = Lcij * xij + 2λi
        }
        node.setLB(LB);

        calculateSubgradient(node);
        calculateStepSize(&node);
        updateMultipliers(node);
        updateLagrangianCosts();

        if(node.getLB() > bestLB){
            bestLB = node.getLB();
            bestMultipliers = node.getMultipliers();
            bestSubgradient = node.getSubgradient();
            bestMST = node.getMST();
            k = 0;
        }
        
        if(checkSubgradient()){ //stop criterrion
            break;
        }
        
        
        else if(node.getLB() <= bestLB){ //if not improving
            k++;

            if(k >= 30){
                k = 0;
                EPSILON = EPSILON / 2;
            }
        }

        if(EPSILON < EPSILON_MIN || (checkMultipliers() and checkSubgradient())){
            break;
            
        }
    }
    delete kruskal;

    node.setLB(bestLB);
    node.setMultipliers(bestMultipliers);
    node.setMST(bestMST);
    node.setSubgradient(bestSubgradient);
   
}





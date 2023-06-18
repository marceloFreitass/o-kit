#include "Lagrange.h"


using namespace std;

Lagrange::Lagrange(double upperBound, vector<vector<double>> originalCost){

    setUpperBound(upperBound);
    setOriginalCost(originalCost);
    EPSILON = 1;
    int n = originalCost.size();
    //Inicializando matriz de custos lagrangiano que ficara mudando de acordo com os multiplicadores
    for(int i = 0; i < n; i++){
		vector<double>temp;
		for(int j = 0; j < n; j++){
			temp.push_back(originalCost[i][j]);
		}
		lagrangianCosts.push_back(temp);
	}
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

    cout << "SUBGRADIENT: {";

    for(int i = 0; i < n; i++){
        cout << subgradient[i] << ",";
    }
    cout << "}\n";

    setSubgradient(subgradient);
    node.setSubgradient(subgradient);
}

void Lagrange::calculateStepSize(Node* node){
    double stepSize;
    int squareSubgradientTotal = 0;
    vector<int> subgradient = node->getSubgradient();
    int n = subgradient.size();

    for(int i = 0; i < n; i++){
        squareSubgradientTotal += (subgradient[i] * subgradient[i]);
    }

    if(squareSubgradientTotal == 0){
        stepSize = 0;
        return;
    }

    stepSize = EPSILON * ((upperBound - node->getLB())/squareSubgradientTotal);
    cout << "StepSize: " << stepSize << endl;
    setStepsize(stepSize);
}

void Lagrange::updateMultipliers(Node& node){

    vector<double> nextMultipliers = node.getMultipliers();
    int n = nextMultipliers.size();
    
    for(int i = 0; i < n; i++){

        nextMultipliers[i] += stepSize * node.getSubgradient()[i];
        
    }
    cout << endl;

    

    node.setMultipliers(nextMultipliers);
    setMultipliers(nextMultipliers);

    cout << "MULTIPLIERS: {";

    for(int i = 0; i < n; i++){
        cout << multipliers[i] << ",";
    }
    cout << endl;

    
}

void Lagrange::updateLagrangianCosts(){

    
    int n = originalCost.size();
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(i == j){
                continue;
            }
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

    double EPSILON_MIN = 0.0005;
    int n = originalCost.size();
    vector<vector<int>> solutionMatrix(n, vector<int>(n));

    vector<double> initialMultiplier(n, 0);
    setMultipliers(initialMultiplier);
    int k = 0;

    double bestLB = -numeric_limits<double>::infinity();
    vector<double> bestMultipliers;
    vector<int> bestSubgradient;
    vector<pair<int, int>> bestMST;
    
    while(1){
        
        Kruskal kruskal(lagrangianCosts);

        double LB = kruskal.oneMST(lagrangianCosts, lagrangianCosts.size());
        
        node.setMST(kruskal.getEdges());
        node.setMultipliers(multipliers); //multipliers = lagrange.getMultipliers()
        for(int i = 0; i < n; i++){
            LB += 2 * multipliers[i];
        }
        cout << "LB: " << LB << endl;
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
        if(checkSubgradient()){
            break;
        }
        else{
            k++;

            if(k > 30){
                k = 0;
                EPSILON /= 2;
            }
        }

        if(EPSILON < EPSILON_MIN || (checkMultipliers() and checkSubgradient())){
            break;
        }
    }

    node.setLB(bestLB);
    node.setMultipliers(bestMultipliers);
    node.setMST(bestMST);
    node.setSubgradient(bestSubgradient);
    
    cout << "Custo: " << node.getLB() << endl << endl;

    cout << "{";

    for(int i = 0; i < n; i++){
        cout << node.getMultipliers()[i] << ", ";
    }
    cout << "}\n";

    for(int i = 0; i < node.getMST().size(); i++){
		solutionMatrix[node.getMST()[i].first][node.getMST()[i].second] = 1;
		solutionMatrix[node.getMST()[i].second][node.getMST()[i].first] = 1;
	}

    cout << "Solution matrix:\n";

    for(int i = 0; i < solutionMatrix.size(); i++){
        for(int j = 0; j < solutionMatrix[i].size(); j++){
            cout << solutionMatrix[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
    
}





#include "aux.h"

void CG(MasterProblem& PM, Subproblem& SP, Node& node){

    
    PM.solve();
    
    while(1){
        SP.setObjFunc(PM.getDuals(), &node);
        double objValue = SP.solve(&node);
        if(1 - objValue < -EPSILON){
            
            PM.addColumn(SP.getXValues(&node));
            PM.solve();
        }
        else{
            break;
        }        
    }

    node.setA(PM.getA());
    //PM.showSolution();
    node.setBins(PM.getObjValue());
    node.setLambdaValues(PM.getLambdasValues());
    node.setFeasible();

}

pair<int, int> getMostFractional(IloNumArray lambdaValues, vector<vector<bool>> A){

     
    int qntPatterns = A.size();
    int itemsSize = A[0].size();

    double mostFractionalValue = 0;
    pair<int, int> selectedPair;
    cout << "LAMBDAS: {";

    for(int i = itemsSize; i < qntPatterns; i++){
        cout << lambdaValues[i] << ",";
    }
    cout << "}\n";
    vector<vector<double>> pairValues(itemsSize, vector<double>(itemsSize, 0.0));

    for(int k = itemsSize; k < qntPatterns; k++){
        if(lambdaValues[k] > EPSILON){
            for(int i = 0; i < itemsSize - 1; i++){
                if(A[k][i] == 1){
                    for(int j = i + 1; j < itemsSize; j++){
                        if(A[k][j] == 1){
                            pairValues[i][j] += lambdaValues[k];
                            //pairValues[j][i] = pairValues[i][j];
                        }
                    }
                }
            }            
        }
    }



    for(int i = 0; i < itemsSize - 1; i++){
        for(int j = i + 1; j < itemsSize; j++){
            if(i == 0 && j == 1){
                mostFractionalValue = abs(0.5 - pairValues[i][j]);
                selectedPair.first = i;
                selectedPair.second = j;
                continue;
            }
            if(abs(0.5 - pairValues[i][j]) < mostFractionalValue - EPSILON){
                mostFractionalValue = abs(0.5 - pairValues[i][j]);
                selectedPair.first = i;
                selectedPair.second = j;
            }
        }
    }

    cout << "PAR{" << selectedPair.first << "," << selectedPair.second << "}\n";
    return selectedPair;

}

void setTogetherRestrictions(Node* node, IloBoolVarArray& x, IloModel& modelSub, IloNumVarArray& lambda, vector<vector<bool>> A){

    vector<pair<int, int>> pairs = node->getTogether();
    int n = pairs.size();
    int qntItems = A[0].size();
    int qntPatterns = A.size();


    for(int i = 0; i < n; i++){
        modelSub.add(x[pairs[i].first] == x[pairs[i].second]);

        for(int j = qntItems; j < qntPatterns; j++){
            if(A[j][pairs[i].first] && !A[j][pairs[i].second] || !A[j][pairs[i].first] && A[j][pairs[i].second]){
                lambda[j].setUB(0.0);
            }
        }
    }
}

void setSeparatedRestrictions(Node* node, IloBoolVarArray& x, IloModel& modelSub, IloNumVarArray& lambda, vector<vector<bool>> A){

    vector<pair<int, int>> pairs = node->getSeparated();
    int n = pairs.size();
    int qntItems = A[0].size();
    int qntPatterns = A.size();
    
    for(int i = 0; i < n; i++){
        modelSub.add(x[pairs[i].first] + x[pairs[i].second] <= 1);
        for(int j = qntItems; j < qntPatterns; j++){
            if(A[j][pairs[i].first] && A[j][pairs[i].second]){
                lambda[j].setUB(0.0);
            }
        }
    }

    

}

void setNodeRestrictions(Node* node, IloBoolVarArray& x, IloModel& modelSub, IloNumVarArray& lambda, vector<vector<bool>> A){
    
    setSeparatedRestrictions(node, x, modelSub, lambda, A);
    setTogetherRestrictions(node, x, modelSub, lambda, A);


}

void resetLambda(IloNumVarArray& lambda){
    int n = lambda.getSize(); 
    for(int i = 0; i < n; i++){
        if(lambda[i].getUB() == 0){
            lambda[i].setUB(IloInfinity);
        }
        
    }
}



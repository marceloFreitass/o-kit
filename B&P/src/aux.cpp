#include "aux.h"

void CG(MasterProblem& PM, Subproblem& SP, Data * data, Node& node){

    
    PM.solve();

    // SP.setObjFunc(PM.getDuals());
    // SP.solve();

    while(1){
        SP.setObjFunc(PM.getDuals(), &node);
        long double objValue = SP.solve(&node);
        if(1 - objValue <= -EPSILON){ //NO MINKNAP TROCA O SINAL
            
            PM.addColumn(SP.getXValues(&node));
            PM.solve();
        }
        else{
            break;
        }        
    }

    node.setA(PM.getA());
    PM.showSolution();
    node.setBins(PM.getObjValue());
    //pair<int, int> sla = getMostFractional(PM.getLambdasValues(), PM.getA());

}

pair<int, int> getMostFractional(vector<double> lambdaValues, vector<vector<bool>> A){


    int qntPatterns = A.size();
    int itemsSize = A[0].size();

    double mostFractionalValue = 0;
    pair<int, int> selectedPair;

    vector<vector<double>> pairValues(itemsSize, vector<double>(itemsSize, 0));

    for(int k = itemsSize; k < qntPatterns; k++){
        if(lambdaValues[k] >= EPSILON){
            cout << "PADRAO: " << k << endl;
            for(int i = 0; i < itemsSize - 1; i++){
                if(A[k][i]){
                    for(int j = i + 1; j < itemsSize; j++){
                        if(A[k][j]){
                            cout << "Items " << i << " e " << j << " ";
                            cout << "LAMBDA: " << lambdaValues[k] << endl;
                            pairValues[i][j] += lambdaValues[k];
                            pairValues[j][i] = pairValues[i][j];
                        }
                    }
                }
            }            
        }
    }

    cout << "Pair values: \n";

    for(int i = 0; i < itemsSize; i++){
        for(int j = 0; j < itemsSize; j++){
            cout << pairValues[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;

    for(int i = 0; i < itemsSize - 1; i++){
        for(int j = i + 1; j < itemsSize; j++){
            if(i == 0 && j == 1){
                mostFractionalValue = abs(0.5 - pairValues[i][j]);
                selectedPair.first = i;
                selectedPair.second = j;
                continue;
            }
            if(abs(0.5 - pairValues[i][j]) < mostFractionalValue){
                mostFractionalValue = abs(0.5 - pairValues[i][j]);
                selectedPair.first = i;
                selectedPair.second = j;
            }
        }
    }

    cout << "selectedPair: {" << selectedPair.first << "," << selectedPair.second << "}\n";
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
                lambda[j].setUB(0);
            }
        }
    }
}

void setSeparatedRestrictions(Node* node, IloBoolVarArray& x, IloModel& modelSub, IloNumVarArray& lambda, vector<vector<bool>> A, Subproblem SP){

    vector<pair<int, int>> pairs = node->getSeparated();
    int n = pairs.size();
    int qntItems = A[0].size();
    int qntPatterns = A.size();

    cout << "A: \n";
    for(int i = 0; i < A.size(); i++){
        cout << "{";
        for(int j = 0; j < A[i].size(); j++){
            cout << A[i][j] << ",";
        }
        cout << "}\n";
    }
    cout << endl;

    
    for(int i = 0; i < n; i++){
        cout << "PAR{" << x[pairs[i].first] << "," << x[pairs[i].second] << "}" << endl;
        cout << modelSub.size() << endl;
        modelSub.add(x[pairs[i].first] + x[pairs[i].second] <= 1);
        SP.solver.exportModel("teste2.lp");
        cout << "C\n";
        for(int j = qntItems; j < qntPatterns; j++){
            if(A[j][pairs[i].first] && A[j][pairs[i].second]){
                lambda[j].setUB(0);
            }
        }
    }
    
    cout << "B\n";

}

void setNodeRestrictions(Node* node, IloBoolVarArray& x, IloModel& modelSub, IloNumVarArray& lambda, vector<vector<bool>> A, Data& data, IloNumArray pi, Subproblem SP){
    

    Subproblem *tempSP = new Subproblem(&data);
    tempSP->setObjFunc(pi, node);
    
    tempSP->x = SP.x;
    tempSP->solver = SP.solver;
    tempSP->envSub = SP.envSub;
   
    setSeparatedRestrictions(node, tempSP->getReferenceSubVariables(), tempSP->getReferenceModel(), lambda, A, *tempSP);
    setTogetherRestrictions(node, tempSP->getReferenceSubVariables(), tempSP->getReferenceModel(), lambda, A);
    cout << "?\n";
    tempSP->solver.exportModel("teste.lp");

    
    modelSub = tempSP->modelSub;
    
    x = tempSP->getSubVariables();
    
    //delete tempSP;


}



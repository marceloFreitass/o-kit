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
    PM.showSolution();
    node.setBins(PM.getObjValue());
    pair<int, int> sla = getMostFractional(PM.getLambdasValues(), PM.getA());

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
            if(abs(0.5 - pairValues[i][j]) > mostFractionalValue){
                mostFractionalValue = abs(0.5 - pairValues[i][j]);
                selectedPair.first = i;
                selectedPair.second = j;
            }
        }
    }

    cout << "selectedPair: {" << selectedPair.first << "," << selectedPair.second << "}\n";
    return selectedPair;

}
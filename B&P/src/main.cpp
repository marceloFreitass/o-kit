#include "Data.h"
#include "MasterProblem.h"
#include "Subproblem.h"
#include "Node.h"
#include "aux.h"
#include <iostream>
#include <chrono>
#include <utility>
#include <cmath>
#define EPSILON 0.00000001

using namespace std;

void BP(Data& data){
    double upperBound = data.getQuantItems() + 1;

    MasterProblem PM(&data);
    Subproblem SP(&data);

    vector<Node> tree;

    Node root(1);
    Node bestNode;
    CG(PM, SP, root);
    //SP.solver.exportModel("antes.lp");
    //PM.solver.exportModel("antesMestre.lp");
    //cout << "EXPORTOU MODELO INICIAL\n\n";
    // ver isso de recuperar os lambda
    //botar restricao no subproblema de acordo com o vector<pair>
    //Criar um novo subproblema(resetar modelo sub)
    

    tree.push_back(root);
    while(!tree.empty()){

        Node currentNode;

        currentNode = tree.back();
        tree.pop_back();
        // cout << "ESCOLHEU OUTRO NO\n";
        // cout << "CAIXAS NO: " << currentNode.getBins() << endl;
        // cout << "VIAVEL: " << currentNode.getFeasible() << endl;

        if(!currentNode.getFeasible()){
            pair<int, int> branchPair = getMostFractional(currentNode.getLambdasValues(), currentNode.getA()); //ajeita
            for(int i = 0; i < 2; i++){

                
                Node newNode(0);
                newNode.setA(currentNode.getA());
                newNode.setSeparatedPairs(currentNode.getSeparated());
                newNode.setTogetherPairs(currentNode.getTogether());
                
                if(i == 0){
                    newNode.addTogetherPair(branchPair);
                    // << "---PAR JUNTO---\n\n";
                    
                    //setNodeRestrictions(&newNode, SP.getReferenceSubVariables(), SP.getReferenceModel(), PM.getVariables(), newNode.getA(), data, PM.getDuals(), SP);
                    
                }

                else{
                    
                    newNode.addSeparatedPair(branchPair);
                   // cout << "---PAR SEPARADO---\n\n";
                }
                
                Subproblem newSP(&data);
                setNodeRestrictions(&newNode, newSP.getReferenceSubVariables(), newSP.getReferenceModel(), PM.getVariables(), newNode.getA());
                CG(PM, newSP, newNode);
                cout << "LB: " << newNode.getBins() << endl;
                cout << "FEASIBLE: " << newNode.getFeasible() << endl; 
                cout << "UB: " << upperBound << endl;

                if(newNode.getFeasible()){
                    if(newNode.getBins() < upperBound){
                    cout << "NOVO UPPERBOUND\n";
                    bestNode = newNode;
                    upperBound = newNode.getBins();
                    continue;
                    }
                }
                if(upperBound - currentNode.getBins() > 1 + EPSILON){
                    cout << "ADD\n";
                    tree.push_back(newNode);
                }

                resetLambda(PM.getVariables());
                // PM.solver.clear();
                // PM.solver.end();
                //cout << "Resetou\n";

            }
        }
        else{
            if(currentNode.getBins() < upperBound){
                //cout << "NOVO UPPERBOUND\n";
                bestNode = currentNode;
                upperBound = currentNode.getBins();
            }
        }

        cout << "ARVORE: " << tree.size() << endl;

    }

    int cont = 0;
    int items = 0;
    cout << "\nSOLUCAO\n\n";
    vector<vector<bool>> bestPatterns = bestNode.getA();
    for(int i = 0; i < bestPatterns.size(); i++){
        if(bestNode.getLambdasValues()[i] == 1){
            cout << "CAIXA " << cont + 1 << ": ";
            for(int j = 0; j < data.getQuantItems(); j++){
                if(bestPatterns[i][j]){
                    cout << j + 1 << " ";
                    items++;
                }
            }
            cont++;
            cout << endl;
        }
        

    }
    cout << endl;

    cout << "Quantidade de caixas: " << bestNode.getBins() << endl;
    if(items == data.getQuantItems()){
        cout << "SOLUCAO VIAVEL!\n";
    }
    else{
        cout << "INVIAVEL.\n";
    }

}

int main(int argc, char** argv){


    Node root;

    Data data(argv[1]);
    vector<int> weight;
    auto begin = chrono::high_resolution_clock::now();
    
    

    BP(data);


    //weight = data.getItemWeight();
    
    
    auto end = chrono::high_resolution_clock::now();
    auto time = chrono::duration_cast<chrono::milliseconds>(end - begin);

    
    cout << "Tempo: " << time.count()/1000.0 << endl;
}
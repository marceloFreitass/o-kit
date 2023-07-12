#include "Data.h"
#include "MasterProblem.h"
#include "Subproblem.h"
#include "Node.h"
#include "aux.h"
#include <iostream>
#include <chrono>
#include <utility>

#define EPSILON 0.00000001

using namespace std;

void BP(Data& data){
    double upperBound = INFINITY;

    MasterProblem PM(&data);
    Subproblem SP(&data);

    vector<Node> tree;

    Node root(1);
    Node bestNode;
    CG(PM, SP, &data, root);
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
        tree.erase(tree.end());
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
                setNodeRestrictions(&newNode, newSP.getReferenceSubVariables(), newSP.getReferenceModel(), PM.getVariables(), newNode.getA(), data);
                CG(PM, newSP, &data, newNode);
                if(upperBound - newNode.getBins() - 1>= EPSILON){
                    //cout << "ADD\n";
                    tree.push_back(newNode);
                }
                if(i == 0){
                    // newSP.solver.exportModel("Node1S.lp");
                    // cout << "AAAAAA\n";
                    // PM.solver.exportModel("Node1M.lp");
                }
                
                else{
                    // newSP.solver.exportModel("Node2S.lp");
                    // PM.solver.exportModel("Node2M.lp");
                }
                resetLambda(PM.getVariables());
                PM.solver.clear();
                PM.solver.end();
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

    }

    // vector<vector<bool>> bestPatterns = bestNode.getA();
    // cout << "TAMANHO DE A: " << bestPatterns.size() << endl;
    // for(int i = 0; i < bestPatterns.size(); i++){
    //     if(bestNode.getLambdasValues()[i] == 1){
    //         cout << "{";
    //         for(int j = 0; j < bestPatterns[i].size(); j++){
    //             cout << bestPatterns[i][j] << ",";
    //         }
    //         cout << "} " << bestNode.getLambdasValues()[i] << "\n";
    //     }

    // }
    // cout << endl;

    cout << "Quantidade de caixas: " << bestNode.getBins() << endl;

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
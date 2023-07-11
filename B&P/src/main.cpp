#include "Data.h"
#include "MasterProblem.h"
#include "Subproblem.h"
#include "Node.h"
#include "aux.h"
#include <iostream>
#include <chrono>
#include <utility>

using namespace std;

void BP(Data& data){
    double upperBound = INFINITY;

    MasterProblem PM(&data);
    Subproblem SP(&data);

    vector<Node> tree;

    Node root, bestNode;
    CG(PM, SP, &data, root);
    SP.solver.exportModel("antes.lp");
    PM.solver.exportModel("antesMestre.lp");
    // ver isso de recuperar os lambda
    //botar restricao no subproblema de acordo com o vector<pair>
    //Criar um novo subproblema(resetar modelo sub)
    
    root.setFeasible();

    tree.push_back(root);

    while(!tree.empty()){


        Node currentNode;

        currentNode = tree.back();
        tree.pop_back();

        if(currentNode.getFeasible()){
            if(currentNode.getBins() < upperBound){
                bestNode = currentNode;
                upperBound = currentNode.getBins();
            }
        }

        else{
            pair<int, int> branchPair = getMostFractional(PM.getLambdasValues(), currentNode.getA()); //ajeita
            for(int i = 0; i < 2; i++){


                Node newNode(0);
                newNode.setA(currentNode.getA());
                newNode.setSeparatedPairs(currentNode.getSeparated());
                newNode.setTogetherPairs(currentNode.getTogether());

                if(i == 0){
                    newNode.addSeparatedPair(branchPair);
                    setNodeRestrictions(&newNode, SP.getReferenceSubVariables(), SP.getReferenceModel(), PM.getVariables(), newNode.getA(), data, PM.getDuals(), SP);

                    
                }

                else{
                    newNode.addTogetherPair(branchPair);

                }
                //aq
                if(i == 0){
                    SP.solver.exportModel("Node1S.lp");
                    cout << "AAAAAA\n";
                    PM.solver.exportModel("Node1M.lp");
                }
                
                else{
                    //SP.solver.exportModel("Node2.lp");
                }

            }
        }

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
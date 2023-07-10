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
    //fazer instancia menor e testar
    //ajeitar os lambda depois do branch, setando 0 se tiver sido proibido
    // ver isso de recuperar os lambda
    //botar restricao no subproblema de acordo com o vector<pair>
    
    root.setFeasible();

    tree.push_back(root);

    while(!tree.empty()){


        Node currentNode;

        currentNode = tree.back();
        tree.erase(currentNode);

        if(currentNode.getFeasible()){
            if(currentNode.getBins() < upperBound){
                bestNode = currentNode;
                upperBound = currentNode.getBins();
            }
        }

        else{
            pair<int, int> branchPair = getMostFractional(lambdaValues, A); //ajeita
            for(int i = 0; i < 2; i++){


                Node newNode;
                newNode.setSeparatedPairs(currentNode.getSeparated());
                newNode.setTogetherPairs(currentNode.getTogether());

                if(i == 0){
                    newNode.addSeparated(branchPair);
                }
                else{
                    newNode.addTogether(branchPair);
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
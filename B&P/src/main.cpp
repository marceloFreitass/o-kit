#include "Data.h"
#include "MasterProblem.h"
#include "Subproblem.h"
#include <iostream>
#include <chrono>

using namespace std;

int main(int argc, char** argv){

    Data data(argv[1]);
    vector<int> weight;
    auto begin = chrono::high_resolution_clock::now();
    //weight = data.getItemWeight();
    
    MasterProblem PM(&data);
    PM.solve();

    Subproblem SP(&data);
    // SP.setObjFunc(PM.getDuals());
    // SP.solve();

    while(1){
        SP.setObjFunc(PM.getDuals());
        long double objValue = SP.solve();
        
        if(1- objValue <= -EPSILON){
            PM.addColumn(SP.getXValues());
            PM.solve();
        }
        else{
            break;
        }
        

    }
    auto end = chrono::high_resolution_clock::now();
    auto time = chrono::duration_cast<chrono::milliseconds>(end - begin);

    PM.showSolution();
    cout << "Tempo: " << time.count()/1000.0 << endl;
}
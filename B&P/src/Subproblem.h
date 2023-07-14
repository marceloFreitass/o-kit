#pragma once

#include "Data.h"
#include "Node.h"
#include <vector>
#include <ilcplex/ilocplex.h>
#include <iostream>

#define EPSILON 0.00000001
#define bigM 1000000

using namespace std;

class Subproblem{

    private:

        Data* data;
        
        
        IloNumArray pi;
       
        IloObjective objFunc;

        

        long int* profit;
        int* xMinknap;

    public:
        IloEnv envSub;
        IloBoolVarArray x;
        IloModel modelSub;


        IloCplex solver;
        
        Subproblem(Data * data);
        void setObjFunc(IloNumArray pi, Node* node);
        double solve(Node* node);
        vector<bool> getXValues(Node* node);

        IloModel getModel();
        IloModel& getReferenceModel();
        IloBoolVarArray getSubVariables();
        IloBoolVarArray& getReferenceSubVariables();


};
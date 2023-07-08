#pragma once

#include "Data.h"
#include <vector>
#include <ilcplex/ilocplex.h>
#include <iostream>

#define EPSILON 0.00000001
#define bigM 1000000

using namespace std;

class Subproblem{

    private:

        Data* data;
        IloEnv envSub;
        IloModel modelSub;
        IloNumArray pi;
        IloBoolVarArray x;
        IloObjective objFunc;

        IloCplex solver;

        long int* profit;
        int* xMinknap;

    public:

        Subproblem(Data * data);
        void setObjFunc(IloNumArray pi);
        long double solve();
        IloNumArray getXValues();



};
#pragma once

#include "Data.h"
#include <ilcplex/ilocplex.h>
#include <iostream>

#define bigM 1000000
#define EPSILON 0.00000001
using namespace std;

class MasterProblem{

    private:


        Data* data;
        IloEnv env;
        IloModel model;
        IloNumVarArray lambda;
        IloRangeArray constraints;
        IloObjective objFunc;

        IloCplex solver;

        vector<vector<bool>> A; // 1 se o item j ta no padrao i

    public:

        MasterProblem(Data * data);
        void solve();
        IloNumArray getDuals();
        void addColumn(IloNumArray newColumn);
        void showSolution();




};
#include "MasterProblem.h"

MasterProblem::MasterProblem(Data * data){
    //Inicializando com n padroes, sendo cada padrao uma caixa unitaria 1 caixa c 1 item
    
    
    this->data = data;

    int n = data->getQuantItems();
    A = vector<vector<bool>>(n, vector<bool>(n, 0));
    env = IloEnv();
    model = IloModel(env);
    lambda = IloNumVarArray(env, n, 0, IloInfinity);
    constraints = IloRangeArray(env);

    IloExpr sum(env);
    
    for(int i = 0; i < n; i++){
        char varName[30];
        sprintf(varName, "λ%d", i);
        lambda[i].setName(varName);
        sum += bigM * lambda[i];

        constraints.add(lambda[i] == 1);
    }
    
    objFunc = IloMinimize(env, sum);
    model.add(objFunc);
    model.add(constraints);
    for(int i = 0; i < n; i++){
        A[i][i] = true;
    }

    sum.end();
}

void MasterProblem::solve(){

    solver = IloCplex(model);
    solver.setOut(env.getNullStream());
    

    solver.solve();
    //cout << "STATUS MESTRE: " << solver.getCplexStatus() << endl;
    
    
}

IloNumArray MasterProblem::getDuals(){
    
    int n = data->getQuantItems();
    IloNumArray duals(env, n);
    for(int i = 0; i < n; i++){
        try{
        duals[i] = solver.getDual(constraints[i]);
        }
        catch(IloException& e){
            cout << "ERRO: " << e << endl;
            e.end();
        }
    }
   

    return duals;
}

void MasterProblem::addColumn(vector<bool> newColumn){
    int n = data->getQuantItems();
    IloNumColumn col = objFunc(1);
    
    for(int i = 0; i < n; i++){
        if(newColumn[i]){
            col += constraints[i](newColumn[i]);
        }
        
        
    }
    // cout << "NOVA CAIXA: {";

    // for(int i = 0; i < newColumn.size(); i++){
    //     cout << newColumn[i] << ",";
    // }
    // cout << "}\n";

    IloNumVar newLambda(col, 0.0, IloInfinity);
    char varName[30];
    sprintf(varName, "λ%d", lambda.getSize());
    newLambda.setName(varName);

    lambda.add(newLambda);

    A.push_back(newColumn);

}

void MasterProblem::showSolution(){
    int sum = 0;
    cout << "Quantidade de caixas: " << solver.getObjValue() << "\n";

    cout << "A: {";

    cout << A.size() << endl;

    cout << "Padroes utilizados: \n";

    for(int i = 0; i < A.size(); i++){
        if(solver.getValue(lambda[i]) >= EPSILON){
            cout << "{";
            for(int j = 0; j < A[i].size(); j++){
                cout << A[i][j] << ",";
                sum += A[i][j];
            }
            cout << "}" << " λ" << i << ": " << solver.getValue(lambda[i]) << endl;
        }
        
    }
}

vector<vector<bool>> MasterProblem::getA(){return A;}
double MasterProblem::getObjValue(){return solver.getObjValue();}

IloNumArray MasterProblem::getLambdasValues(){
    
    int qntPatterns = A.size();
    //vector<double> lambdaValues(qntPatterns, 0);
    IloNumArray lambdaValues(env, qntPatterns);

    solver.getValues(lambdaValues, lambda);
    // for(int i = 0; i < qntPatterns; i++){
    //     try{
    //         lambdaValues[i] = solver.getValue(lambda[i]);
    //         if(lambdaValues[i] >= 1 - EPSILON){
    //             lambdaValues[i] = 1;
    //         }
    //     }
    //     catch(IloException& e){
    //         cout << "ERRO: " << e << endl;
    //         e.end();
    //     }
    // }

    return lambdaValues;

}

IloNumVarArray& MasterProblem::getVariables(){return lambda;}
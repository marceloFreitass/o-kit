#include "Subproblem.h"
#include "minknap.c"

Subproblem::Subproblem(Data * data){

    this->data = data;

    int n = data->getQuantItems();

    envSub = IloEnv();
}

void Subproblem::setObjFunc(IloNumArray pi, Node * node){
    int n = data->getQuantItems();

    this->pi = pi;

    if(node->getType()){
              //MINKNAP

        profit = new long int[n];
        for(int i = 0; i < n; i++){
            profit[i] = bigM * pi[i];
        }

    }

    else{
    
        //      MODELO
        modelSub = IloModel(envSub);
        x = IloBoolVarArray(envSub, n);
        
        IloExpr sumWeight(envSub);
        for(int i = 0; i < n; i++){
            sumWeight += data->getItemWeight(i) * x[i];
        }
        modelSub.add(sumWeight <= data->getBinCapacity());

        IloExpr sum(envSub);

        for(int i = 0; i < n; i++){
            sum += pi[i] * x[i];
        }

        objFunc = IloMaximize(envSub, sum);
        modelSub.add(objFunc);
    }
   
}

long double Subproblem::solve(Node* node){
    
    int n = data->getQuantItems();
    //MODELO

    if(!node->getType()){
        
        solver = IloCplex(modelSub);
        solver.setOut(envSub.getNullStream());
        solver.solve();

        return solver.getObjValue();

    }
    else{

        //MINKNAP
         double sum = 0;
         int *w = new int[n];
         xMinknap = new int[n];

         for(int i = 0; i < n; i++){
             w[i] = data->getItemWeight(i);
         }
         long double z = minknap(n, profit, w, xMinknap, data->getBinCapacity());
         delete w;
         delete profit;
        

         for(int i = 0; i < n; i++){
             sum += xMinknap[i] * pi[i];
         }
         return sum;

    }
    


}

IloNumArray Subproblem::getXValues(Node* node){

    int n = data->getQuantItems();
    IloNumArray xValues(envSub, n);

        //MODELO
    if(!node->getType()){
        solver.getValues(xValues, x);
    }
    else{
        
        // MINKNAP

        for(int i = 0; i < n; i++){
            xValues[i] = xMinknap[i];
        }
        delete xMinknap;        
    }
    

    

    return xValues;
}


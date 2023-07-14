#include "Subproblem.h"
#include "minknap.c"

Subproblem::Subproblem(Data * data){

    this->data = data;

    int n = data->getQuantItems();

    envSub = IloEnv();

    modelSub = IloModel(envSub);
    x = IloBoolVarArray(envSub, n);

    IloExpr sumWeight(envSub);
    for(int i = 0; i < n; i++){
        sumWeight += data->getItemWeight(i) * x[i];
    }
    
    modelSub.add(sumWeight <= data->getBinCapacity());
    objFunc = IloMaximize(envSub);
    modelSub.add(objFunc);

}

void Subproblem::setObjFunc(IloNumArray pi, Node * node){
    int n = data->getQuantItems();

    this->pi = pi;
    // cout << "DUAIS: {";
    // for(int i = 0; i < n; i++){
    //     cout << pi[i] << ",";
    // }
    // cout << "}\n";
    if(node->getType()){
              //MINKNAP

        profit = new long int[n];
        for(int i = 0; i < n; i++){
            profit[i] = bigM * pi[i];
        }

    }

    else{
    
        //      MODELO

        IloExpr sum(envSub);
        for(int i = 0; i < n; i++){
            sum += pi[i] * x[i];
        }
       
        objFunc.setExpr(sum);
        
    }
   
}

double Subproblem::solve(Node* node){
    
    int n = data->getQuantItems();
    //MODELO

    if(!node->getType()){
        
        solver = IloCplex(modelSub);
        solver.setOut(envSub.getNullStream());

        solver.setParam(IloCplex::Param::TimeLimit, 2*60);
        solver.setParam(IloCplex::Param::Threads, 1);
        solver.setParam(IloCplex::Param::MIP::Tolerances::MIPGap, 1e-08);

        solver.solve();
        double value = solver.getObjValue();
        //cout << "STATUS SUB: " << solver.getCplexStatus() << endl;

        return value;

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
        

        //  for(int i = 0; i < n; i++){
        //      sum += xMinknap[i] * pi[i];
        //  }
         pi.clear();
         pi.end();
         return z / bigM;

    }
    
    


}

vector<bool> Subproblem::getXValues(Node* node){
    
    int n = data->getQuantItems();
    vector<bool> xValues(n, 0);

        //MODELO
    if(!node->getType()){


        for(int i = 0; i < n; i++){
            if(solver.getValue(x[i]) >= 0.9 - EPSILON){
                xValues[i] = 1;
            }
            else{
                xValues[i] = 0;
            }
        }

    }
    else{

        // MINKNAP

        for(int i = 0; i < n; i++){
            xValues[i] = xMinknap[i];
            //x[i].setLB(xMinknap[i]);
        }
        delete xMinknap;        
    }    

    return xValues;
}
IloModel Subproblem::getModel(){return modelSub;}
IloModel& Subproblem::getReferenceModel(){return modelSub;}
IloBoolVarArray Subproblem::getSubVariables(){return x;}
IloBoolVarArray& Subproblem::getReferenceSubVariables(){return x;}


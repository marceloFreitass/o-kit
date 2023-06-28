#include "separation.h"

int selectVertex(vector<bool>& chosed, int n){

    for(int i = 0; i < n; i++){

        if(chosed[i] == 0){
            chosed[i] = 1;
            return i;
        }
    }

    return -1;

}

void updateBool(vector<bool>&chosed, vector<int> Smin){

    int n = Smin.size();

    for(int i = 0; i < n; i++){
        chosed[Smin[i]] = 1;
    }
}


bool checkChosed(vector<bool> chosed){

    int n = chosed.size();
    for(int i = 0; i < n; i++){
        if(chosed[i] == 0){
            return 0;
        }
    }

    return 1;
}



double sumWeights(int v, vector<bool> chosed, double** x, bool inout){ //Soma os pesos de um vertice v em relacao aos vertices que nao estao em S
    //inout == 1 if belongs to s, 0 if not
    double sum = 0;
    int n = chosed.size();
    for(int i = 0; i < n; i++){
        if(chosed[i] == inout){
            if(i < v){
                sum += x[i][v];
            }
            else{
                sum += x[v][i];
            }
        }
    }

    return sum;
}

int maximumMaxBack(vector<bool>&chosed, double ** x, double& maximumMaxBackValue){

    int n = chosed.size();
    int maxV = -1;
    double maxValue = -numeric_limits<double>::infinity();
    double value;

    for(int i = 0; i < n; i++){
        if(chosed[i] == 0){

            value = sumWeights(i, chosed, x, 1);
            if(value > maxValue){
                maxValue = value;
                maxV = i;
            }
        }
    }
    maximumMaxBackValue = maxValue;
    return maxV;

}


vector<vector<int>> MaxBack(double** x, int n){

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(x[i][j] < EPSILON){
                //x[i][j] = 0;
            }
        }
    }

    vector<vector<int>> bestS;
    vector<int> S0, Smin, V;
    vector<bool> alreadyChosed(n, 0);


    double cutVal;
    double cutMin = numeric_limits<double>::infinity();

    for(int i = 0; i < n; i++){ //Tqv se usa o vertice 0
        V.push_back(i);
    }

    while(!checkChosed(alreadyChosed)){ //enquanto nao tiver usado todos

        int v = selectVertex(alreadyChosed, n);
        vector<bool> inS(n, 0);
        Smin.push_back(v);

        S0 = Smin;
        updateBool(inS, Smin);

        cutMin = sumWeights(v, inS, x, 0); //soma todos os pesos de um vertice v que entrou em S com todos os vertices que nao estao em S
        cutVal = cutMin;

        //De todos os vertices que nao estao em S, escolher o vertice em que a soma SumWeights é maxima em relacao aos vertices que estao em S

        while(S0.size() < V.size()){

            double maximumMaxBackValue;
            v = maximumMaxBack(inS, x, maximumMaxBackValue);

            S0.push_back(v);
            inS[v] = 1;

            cutVal += 2 - 2* maximumMaxBackValue;
            if(cutVal < cutMin){
                cutMin = cutVal;
                Smin = S0;
            }
        }
        updateBool(alreadyChosed, Smin);
        if(Smin.size() == n){
            break;
        }
        bestS.push_back(Smin);

    }

    //Escolher um vertice de v de V para iniciar a heuristica


    //Usar std::set
    return bestS;
}

int tightVertex(vector<bool> inA, double ** x, int n){

    double sum;
    double maxSum = -numeric_limits<double>::infinity();
    int maxI;
    for(int i = 0; i < n; i++){
        if(!inA[i]){

            sum = sumWeights(i, inA, x, 1);
            cout << "A: {";
            for(int j = 0; j < n; j++){
                if(inA[j]){
                    cout << j << ",";
                }
            }
            cout << "}\n";
            cout << "i: " << i << endl;

            if(sum > maxSum){
                maxSum = sum;
                cout << "MAX: " << maxSum << endl;
                maxI = i;
            }
        }
    }
    cout << "TIGHT:: " << maxI << endl;
    return maxI;


}

void shrinkGraph(vector<vector<int>>& V, double ** x, int n, int a, int b){
    cout << "SHRINK: \n\n";


    if(a > b){
        swap(a, b);
    }

    cout << "A/B: " << a << "/" << b << endl;
    x[a][b] = 0;
    V.erase(V.begin() + b);
    
    V[a].push_back(b);
    
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cout << x[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
    

    cout << "V: {";
    for(int i = 0; i < V.size(); i++){
        cout << "{";
        for(int j = 0; j < V[i].size(); j++){
            cout << V[i][j] << ",";
        }
        cout << "}";
    }
    cout << "}\n";



    return;
}

double MinCutPhase(vector<vector<int>>& V, double **x, int n, int a){

    vector<int> A;
    vector<bool> inA(n, 0);
    A.push_back(a);
    inA[a] = 1;
    double value = 0;


    while(A.size() < n){

        int tV = tightVertex(inA, x, n);
        A.push_back(tV);
        inA[tV] = 1;

    }

    cout << "ULTIMOS NOS ADICIONADOS: " << A[A.size() - 1] << " e " << A[A.size() - 2] << endl;

    int lastVertex = A[A.size() - 1];

    for(int i = 0; i < n; i++){
        if(lastVertex < i){
            value += x[lastVertex][i];
        }
        else{
            value += x[i][lastVertex];
        }
    }

    cout << "Value: " << value << endl;
    
    shrinkGraph(V, x, n, A[A.size() - 1], A[A.size() - 2]);
    return value;
}

vector<vector<int>> MinCut(double**x, int n){


    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(x[i][j] < EPSILON){
                x[i][j] = 0;
            }
            cout << x[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;



    vector<vector<int>> V;
    int a = 0;
    double cutOfThePhase;
    double minimumCut = numeric_limits<double>::infinity();

    for(int i = 0; i < n; i++){
        V.push_back({i});
    }

    while(V.size() > 1){

        cutOfThePhase = MinCutPhase(V, x, n, a);
        if(cutOfThePhase < minimumCut){
            minimumCut = cutOfThePhase;
        }

    }











}



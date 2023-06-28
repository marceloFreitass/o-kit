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

    cout << "MATRIX X: \n";

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cout << x[i][j] << " ";
        }
        cout << endl;
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
        cout << "CUT VAL INICIAL: " << cutVal << endl;
        cout << "Agora: \n\n";
        //De todos os vertices que nao estao em S, escolher o vertice em que a soma SumWeights é maxima em relacao aos vertices que estao em S

        while(S0.size() < V.size()){

            double maximumMaxBackValue;
            v = maximumMaxBack(inS, x, maximumMaxBackValue);
            cout << "S: \n";

            for(int i = 0; i < inS.size(); i++){
                cout << inS[i] << " ";
            }
            cout << endl;
            cout << "V: " << v << endl;
            cout << "best max back: " << maximumMaxBackValue << endl;
            S0.push_back(v);
            inS[v] = 1;

            cutVal += 2 - 2* maximumMaxBackValue;
            cout << "CUT VAL: " << cutVal << endl;
            if(cutVal < cutMin){
                cutMin = cutVal;
                Smin = S0;
            }
        }
        updateBool(alreadyChosed, Smin);
        cout << "ALREADY CHOSED: {";
        for(int i = 0; i < alreadyChosed.size(); i++){
            cout << alreadyChosed[i] << ",";
        }
        cout << "}\n";


        cout << "Smin: \n";
        for(int i = 0; i < Smin.size(); i++){
            cout << Smin[i] << " ";
        }
        cout << endl;
        cout << "Cutmin: " << cutMin << endl;
        if(Smin.size() == n){
            break;
        }
        bestS.push_back(Smin);

    }

    //Escolher um vertice de v de V para iniciar a heuristica


    //Usar std::set
    return bestS;
}
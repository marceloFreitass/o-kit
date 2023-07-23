#include "separation.h"

int selectVertex(vector<bool>& chosed, int n){ //Select

    auto it = find(chosed.begin(), chosed.end(), 0);

    if (it != chosed.end())
    {
        int index = it - chosed.begin();
        chosed[index] = 1;
        return index;
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
    int sum = 0;
    for(int i = 0; i < n; i++){
        if(chosed[i]){
            sum++;
        }
    }
    return sum == n;
}



double sumWeights(int v, vector<bool> chosed, double** x, bool inout){ //Sum the weights of v with respect to the vertices are in/out chosed
    //inout == 1 if belongs to s, 0 if not
    double sum = 0;
    int n = chosed.size();
    for(int i = 0; i < n; i++){
        if(chosed[i] == inout){
            if(i < v){
                if(x[i][v] > EPSILON){
                    sum += x[i][v];
                }
                
            }
            else{
                if(x[v][i] > EPSILON){
                    sum += x[v][i];
                }
                
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
                x[i][j] = 0;
            }
        }
    }

    vector<vector<int>> bestS;
    vector<int> S0, Smin, V;
    vector<bool> alreadyChosed(n, 0);

    double cutVal;
    double cutMin = numeric_limits<double>::infinity();

    for(int i = 0; i < n; i++){
        V.push_back(i);
    }

    while(!checkChosed(alreadyChosed)){ //until visited all vertices
        
        S0.clear();
        int v = selectVertex(alreadyChosed, n);
        vector<bool> inS(n, 0);
        S0.push_back(v);
        alreadyChosed[v] = 1;
        inS[v] = 1;

        cutMin = sumWeights(v, inS, x, 0); //sum all weights between v and all vertices that are not in S0
        cutVal = cutMin;

        //From all vertices that arent in S, choose the vertex whose SumWeights is maximum with respect to the vertices that are in S

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
    
    return bestS;
}


int index2D(vector<vector<int>> V, int element){

    int n = V.size();

    for(int i = 0; i < n; i++){
        for(int j = 0; j < V[i].size(); j++){
            if(V[i][j] == element){
                return i;
            }
        }
    }

    return -1;
}

int index(vector<int> V, int element){

    auto it = find(V.begin(), V.end(), element);

    if (it != V.end())
    {
        int index = it - V.begin();
        return index;
    }
    return -1;
}


int tightVertex(vector<vector<int>>& V, vector<bool> inA, double ** x, int n){

    double sum;
    double maxSum = -numeric_limits<double>::infinity();
    int maxI;
    int Vsize = V.size();
    for(int i = 0; i < Vsize;i++){

        if(!inA[V[i][0]]){

            sum = sumWeights(V[i][0], inA, x, 1);

            if(sum > maxSum){
                maxSum = sum;
                maxI = V[i][0];
            }
        }
    }
    return maxI;
}

vector<vector<int>> identifyEdges(double **x, int n, int a, int b){


    vector<vector<int>> EdgesAB(2, vector<int>(n)); //EDGES[0][i] == edges connected with a, EDGES[1][i] edges connected witb b
    EdgesAB[1].clear();
    for(int i = 0; i < n; i++){
        if(i < b){
            if(x[i][b] > EPSILON){
                EdgesAB[1].push_back(i);
            }
        }
        else if(i > b){
            if(x[b][i] > EPSILON){
                EdgesAB[1].push_back(i);
            }
        }
        
    }

    return EdgesAB;
}

vector<int> getComplement(vector<int> A, int n){

    vector<int> complement;
    for(int i = 0; i < n; i++){
        if(index(A, i) == -1){
            complement.push_back(i);
        }
    }
    return complement;


}

void shrinkGraph(vector<vector<int>>& V, double ** x, int n, int a, int b){

    if(a > b){
        swap(a, b);
    }
    //smaller vertex represents the "supervertex"

    vector<vector<int>> edgesToMerge;

    int indA = index2D(V, a);
    int indB = index2D(V, b);

    int bSize = V[indB].size();
    for(int i = 0; i < bSize; i++){ //updating the V subsets to represents a "supervertex"
        V[indA].push_back(V[indB][0]);
        V[indB].erase(V[indB].begin());
    }
    V.erase(V.begin() + indB);
    
    x[a][b] = 0;

    edgesToMerge = identifyEdges(x, n, a, b); //getting the vertices connected with b

    for(int i = 0; i < edgesToMerge[1].size(); i++){ //updating the x costs
        if(a < edgesToMerge[1][i]){
            if(b < edgesToMerge[1][i]){
                x[a][edgesToMerge[1][i]] += x[b][edgesToMerge[1][i]];
                x[b][edgesToMerge[1][i]] = 0;
            }
            else{
                x[a][edgesToMerge[1][i]] += x[edgesToMerge[1][i]][b];
                x[edgesToMerge[1][i]][b] = 0;
            }
        }
        else{

            if(b < edgesToMerge[1][i]){
                x[edgesToMerge[1][i]][a] += x[b][edgesToMerge[1][i]];
                x[b][edgesToMerge[1][i]] = 0;
            }
            else{
                x[edgesToMerge[1][i]][a] += x[edgesToMerge[1][i]][b];
                x[edgesToMerge[1][i]][b] = 0;
            }

        }
    }

    return;
}

double MinCutPhase(vector<vector<int>>& V, double **x, int n, int a, int& last1, int& last2, vector<int>& set){

    vector<int> A;
    vector<bool> inA(n, 0);
    A.push_back(a);
    inA[a] = 1;
    double value = 0;


    while(A.size() < V.size()){

        int tV = tightVertex(V, inA, x, n); //find the vertex that is not in A with the maximum weight attached to vertex that are in A
        A.push_back(tV);
        inA[tV] = 1;

    }

    int lastVertex = A[A.size() - 1]; //cut of the phase value = sum of weights of last added vertex

    for(int i = 0; i < n; i++){
        if(lastVertex < i){
            if(x[lastVertex][i] > EPSILON){
                value += x[lastVertex][i];
            }
            
        }
        else{
            if(x[i][lastVertex] > EPSILON){
                value += x[i][lastVertex];
            }
            
        }
    }

    last1 = A[A.size() - 1];
    last2 = A[A.size() - 2];
    
    int indSet1 = index2D(V, last1);
    if(value < 2 - EPSILON){ //getting the smaller subtour(less constraints) set
        if(V[indSet1].size() <= n/2){
            set = V[indSet1];
            
        }
        else{
            set = getComplement(V[indSet1], n);
        }
    }

    return value;
}

vector<vector<int>> MinCut(double**x, int n){

    srand(time(NULL));
    vector<int> set;
    vector<vector<int>> cutSet;
    vector<vector<int>> V;
    int last1, last2;
    double cutOfThePhase;

    int a = rand() % n;

    for(int i = 0; i < n; i++){
        for(int j = i+1; j < n; j++){
            if(x[i][j] < EPSILON){
                x[i][j] = 0;
            }
        }
    }

    for(int i = 0; i < n; i++){
        V.push_back({i});
    }

    while(1){
        
        cutOfThePhase = MinCutPhase(V, x, n, a, last1, last2, set);
        if(!set.empty()){
            cutSet.push_back(set);
            set.clear();
        }
        if(V.size() == 2){
            break;
        }
        shrinkGraph(V, x, n, last1, last2);

    }

    return cutSet;
}






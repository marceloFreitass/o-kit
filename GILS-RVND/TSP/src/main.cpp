#include "Data.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <ctime>
#include <chrono>

using namespace std;


typedef struct{

    vector<int> sequency;
    double cost;
    
} Solution;

typedef struct{
    
    int insertedNode;
    int removedEdge;
    double cost;

} insertionInfo;

void showSolution(const Solution *s){
    int n = s->sequency.size();
    for(int i = 0; i < n; i++){
        if(i == n - 1){
            cout << s->sequency[i] << endl;
        }
        else{
        cout << s->sequency[i] << "->";
        }
    }
}

void updateSolutionCost(Solution *s, Data& adjMatrix){
    s->cost = 0;
    size_t n = s->sequency.size();
    for(int i = 0; i < n - 1; i++){

        s->cost += adjMatrix.getDistance(s->sequency[i], s->sequency[i+1]);
    }
    
}

vector <insertionInfo> estimateInsertionCost(Solution *s, vector<int>& CL, Data& adjMatrix){
    
    int n = s->sequency.size();
    vector<insertionInfo> insertionCost((n - 1)*CL.size());
    int l = 0, a, b;
    int count = 0;

    for(a = 0, b = 1; count < n - 1; a++, b++){

        int i = s->sequency[a];
        int j = s->sequency[b];
   
        for(auto k : CL){

            insertionCost[l].cost = adjMatrix.getDistance(i, k) + adjMatrix.getDistance(k, j) - adjMatrix.getDistance(i, j);          
            insertionCost[l].insertedNode = k;
            insertionCost[l].removedEdge = a;
            l++;
            
        }
        count++;
    }
    return insertionCost;
}

bool compareCost(insertionInfo x, insertionInfo  y){
    
    return (x.cost) <  (y.cost);
}


Solution Construction(vector<int>& CL1, Data& adjMatrix){

    vector<int>CL = CL1;
    
    Solution s = {{1,1}, 0};
    int i, find, selected;
    double alpha;
    
    for(i = 0; i < 3; i++){

    int randomPos = rand() % CL.size();
    int randomVal = CL[randomPos];
    s.sequency.insert(s.sequency.end()-1, randomVal);
    CL.erase(CL.begin() + randomPos);
    
    }  
    while(!CL.empty()){  

        vector<insertionInfo> insertionCost = estimateInsertionCost(&s, CL, adjMatrix);
        sort(insertionCost.begin(), insertionCost.end(), compareCost);
        alpha = rand() / (RAND_MAX + 1.0);
        alpha += 0.0000000001; //avoiding alpha 0
        selected = rand() % ((int) ceil(alpha * insertionCost.size()));
        s.sequency.insert(s.sequency.begin() + insertionCost[selected].removedEdge + 1, insertionCost[selected].insertedNode);
            
        find = 0; //finding the inserted node and removing from CL
        
        while(1){
            if(CL[find] == insertionCost[selected].insertedNode){
                CL.erase(CL.begin() + find);
                break;
            }
            find++;
        }
    }
     
    
    updateSolutionCost(&s, adjMatrix);
    return s;
}

bool bestImprovementSwap(Solution* s, Data& adjMatrix){
    
    double delta1, totalDelta, bestDelta = 0;
    int best_i, best_j;
    int n = s->sequency.size();

    for(int i = 1; i < n - 1; i++){

        delta1 = -adjMatrix.getDistance(s->sequency[i - 1], s->sequency[i])-adjMatrix.getDistance(s->sequency[i], s->sequency[i+1]);
        
        for(int j = i + 1; j < n - 1; j++){
            
           if(j == i + 1){ //Swapping with the subsequent
                totalDelta = -adjMatrix.getDistance(s->sequency[i], s->sequency[i-1])-adjMatrix.getDistance(s->sequency[j], s->sequency[j+1])
                +adjMatrix.getDistance(s->sequency[j], s->sequency[i-1])+adjMatrix.getDistance(s->sequency[i], s->sequency[j+1]);
           }
           else{
           totalDelta = delta1;
           
           totalDelta += -adjMatrix.getDistance(s->sequency[j-1], s->sequency[j])-adjMatrix.getDistance(s->sequency[j], s->sequency[j+1])
           +adjMatrix.getDistance(s->sequency[i-1], s->sequency[j])+adjMatrix.getDistance(s->sequency[i+1], s->sequency[j]) +
           adjMatrix.getDistance(s->sequency[j-1], s->sequency[i])+adjMatrix.getDistance(s->sequency[j+1], s->sequency[i]);

           }
            if(totalDelta < bestDelta){
                bestDelta = totalDelta;
                best_i = i;
                best_j = j;
            }
        }
    }

    if(bestDelta < 0){
       
        swap(s->sequency[best_i], s->sequency[best_j]);

        return true;
    }
    
    return false;
}

bool bestImprovement2Opt(Solution *s, Data& adjMatrix){

    double bestDelta = 0;
    int best_i, best_j;
    int n = s->sequency.size();
    double edge1Delta, totalDelta;
    for(int i = 1; i < n; i++){
        
        edge1Delta = -adjMatrix.getDistance(s->sequency[i - 1], s->sequency[i]);

        for(int j = i  + 2; j < n - 1; j++){

            totalDelta = edge1Delta;
            totalDelta += -adjMatrix.getDistance(s->sequency[j - 1], s->sequency[j]) 
            +adjMatrix.getDistance(s->sequency[i - 1], s->sequency[j - 1])+adjMatrix.getDistance(s->sequency[i], s->sequency[j]);

            if(totalDelta < bestDelta){

                bestDelta = totalDelta;
                best_i = i;
                best_j = j;
            }
        }
    }

    if(bestDelta < 0){
        
        int qTrocas = (best_j - best_i)/2;
        

        for(int c = 0; c < qTrocas; c++){

            swap(s->sequency[best_j - 1 - c], s->sequency[best_i + c]); //swap the elements in between
        }

        return true;
    }
    return false;    
}

bool bestImprovementOrOpt(Solution *s, int type, Data& adjMatrix){

    double bestDelta = 0;
    int best_i, best_j;
    int n = s->sequency.size();
    double delta1, totalDelta;

    switch(type){

        case 1: //Reinsertion

        for(int i = 1; i < n - 1; i++){

            delta1 = -adjMatrix.getDistance(s->sequency[i - 1], s->sequency[i]) 
            -adjMatrix.getDistance(s->sequency[i], s->sequency[i+1]) +adjMatrix.getDistance(s->sequency[i-1], s->sequency[i+1]);

            for(int j = 1; j < n - 1; j++){
                
                if(j == i + 1 || j == i){ //Checking if the reinsertion is in the same place
                    totalDelta = 0;
                    continue;
                    
                }
                else{

                totalDelta = delta1;

                totalDelta += -adjMatrix.getDistance(s->sequency[j - 1], s->sequency[j])
                +adjMatrix.getDistance(s->sequency[j-1], s->sequency[i])+adjMatrix.getDistance(s->sequency[i], s->sequency[j]);

                if(totalDelta < bestDelta){

                    bestDelta = totalDelta;
                    best_i = i;
                    best_j = j;
                }
                }
            }
        }

        if(bestDelta < 0){
            
            if(best_i > best_j){

                s->sequency.insert(s->sequency.begin() + best_j, s->sequency[best_i]);
                s->sequency.erase(s->sequency.begin()+best_i + 1);
            }
            else{
                s->sequency.insert(s->sequency.begin() + best_j, s->sequency[best_i]);
                s->sequency.erase(s->sequency.begin()+best_i);
            }

            return true;
        }
        return false;


        case 2: //Or-opt-2 

        for(int i = 2; i < n -1; i++){

            delta1 = -adjMatrix.getDistance(s->sequency[i], s->sequency[i + 1])
            -adjMatrix.getDistance(s->sequency[i-1], s->sequency[i-2])+adjMatrix.getDistance(s->sequency[i-2], s->sequency[i + 1]);

            for(int j = 1; j < n -1; j++){

                if(abs(j - i) <= 1){ //Avoiding putting the block on the same place
                    
                    totalDelta = 0;
                    continue;

                }
                else{

                totalDelta = delta1;
                totalDelta += -adjMatrix.getDistance(s->sequency[j], s->sequency[j-1])
                +adjMatrix.getDistance(s->sequency[i], s->sequency[j-1])+adjMatrix.getDistance(s->sequency[i-1], s->sequency[j]);

                if(totalDelta < bestDelta){

                    bestDelta = totalDelta;
                    best_i = i;
                    best_j = j;

                }
                }
            }
        }

        if(bestDelta < 0){
            if(best_i < best_j){
            s->sequency.insert(s->sequency.begin()+best_j, s->sequency[best_i]);
            s->sequency.insert(s->sequency.begin()+best_j +1, s->sequency[best_i - 1]);
            s->sequency.erase(s->sequency.begin()+best_i-1);
            s->sequency.erase(s->sequency.begin()+best_i-1);
            
            }
            else{
            s->sequency.insert(s->sequency.begin()+best_j, s->sequency[best_i - 1]);
            s->sequency.insert(s->sequency.begin()+best_j, s->sequency[best_i+1]);
            s->sequency.erase(s->sequency.begin()+best_i+1);
            s->sequency.erase(s->sequency.begin()+best_i+1);
            
            }
    
            return true;
        }
        return false;

        case 3: //Or-opt-3

        for(int i = 3; i < n - 1; i++){

            delta1 = -adjMatrix.getDistance(s->sequency[i], s->sequency[i+1])
            -adjMatrix.getDistance(s->sequency[i-3], s->sequency[i-2])+adjMatrix.getDistance(s->sequency[i-3], s->sequency[i+1]);

            for(int j = 1; j < n -1; j++){

                if(abs(j - i) <= 2){ //Avoiding putting the block on the same place

                    totalDelta = 0;
                    continue;
                }

                totalDelta = delta1;
                totalDelta += -adjMatrix.getDistance(s->sequency[j], s->sequency[j-1])
                +adjMatrix.getDistance(s->sequency[j-1], s->sequency[i])+adjMatrix.getDistance(s->sequency[i-2], s->sequency[j]);

                if(totalDelta < bestDelta){

                    bestDelta = totalDelta;
                    best_i = i;
                    best_j = j;
                }
            }
        }

        if(bestDelta < 0){
          

            if(best_i < best_j){
                s->sequency.insert(s->sequency.begin()+best_j, s->sequency[best_i]);
                s->sequency.insert(s->sequency.begin()+best_j+1, s->sequency[best_i-1]);
                s->sequency.insert(s->sequency.begin()+best_j+2, s->sequency[best_i-2]);
                s->sequency.erase(s->sequency.begin()+best_i-1);
                s->sequency.erase(s->sequency.begin()+best_i-1);
                s->sequency.erase(s->sequency.begin()+best_i-2); 
            }
            else{
                s->sequency.insert(s->sequency.begin()+best_j, s->sequency[best_i]);
                s->sequency.insert(s->sequency.begin()+best_j+1, s->sequency[best_i]);
                s->sequency.insert(s->sequency.begin()+best_j+2, s->sequency[best_i]);
                s->sequency.erase(s->sequency.begin()+best_i+1);
                s->sequency.erase(s->sequency.begin()+best_i+1);
                s->sequency.erase(s->sequency.begin()+best_i+1);      
            }
            return true;

        }
        return false;

    }
    return false;
}

void localSearch(Solution& s, Data& adjMatrix){


    vector<int> NL = {1,2,3,4,5};
    bool improved = 0;
    
    while(!NL.empty()){

        int n = rand() % NL.size();

        switch(NL[n]){


            case 1:
                improved = bestImprovementSwap(&s, adjMatrix);
                
                break;
            case 2:
                improved = bestImprovement2Opt(&s, adjMatrix);
                
                break;
            case 3:
                improved = bestImprovementOrOpt(&s, 1, adjMatrix);
                
                break;
            case 4:
                improved = bestImprovementOrOpt(&s, 2, adjMatrix);
                
                break;
            case 5:
                improved = bestImprovementOrOpt(&s, 3, adjMatrix);
                break;
        }

        if(improved){
            NL = {1, 2, 3, 4, 5};
        }
        else{
            NL.erase(NL.begin() + n);
        }
        }
    
    }

Solution Pertubation(const Solution s){

    Solution sCopy = s;
    int n = sCopy.sequency.size();
    int tSub1, tSub2;
    int qV = (n - 1)/10; 


    if(qV > 2){ //chosing random subsets sizes 

        tSub1 = rand() % (qV - 1) + 2; //In case of vertices > 29, the size range changes
        tSub2 = rand() % (qV - 1) + 2;
    }
    else{

        tSub1 = tSub2 = 2;
    }


    cout << "TAMANHO1: " << tSub1 << endl;
    cout << "TAMANHO2: " << tSub2 << endl;  
    int i1, j1, i2, j2;

    j1 = rand() % (n - 1 - tSub1) + tSub1; //chosing a random subset1 end
    i1 = j1 - tSub1 + 1;
    
    do{ //subset2 cant overlap subset1
        
        j2 = rand() %( n - 1 - tSub2) + tSub2;
        i2 = j2 - tSub2 + 1;
        
        if((j2 < i1 && i2 < i1) || (i2 > j1 && j2 > j1)){
            break;
        }

    }while(1);

    if(tSub1 > tSub2){ // subset1 bigger

        int qTrocas = tSub2;
        int cT;
        if(i1 < i2){ //if subset1 before subset2

            for(cT = 0; cT < qTrocas; cT++){    

                swap(sCopy.sequency[i1 + cT], sCopy.sequency[i2 + cT]);
            }
            for(int i = 0; i < tSub1 - tSub2; i++){

                sCopy.sequency.insert(sCopy.sequency.begin() + i2 + cT, sCopy.sequency[i1 + cT]);
                sCopy.sequency.erase(sCopy.sequency.begin() + i1 + cT);
            }
            
        }
        else{ 

            for(cT = 0; cT < qTrocas; cT++){

                swap(sCopy.sequency[i1+cT], sCopy.sequency[i2+cT]);
            }
            for(int i = 0; i < tSub1 - tSub2; i++){

                sCopy.sequency.insert(sCopy.sequency.begin()+i2+cT + i, sCopy.sequency[i1+cT+i]);
                sCopy.sequency.erase(sCopy.sequency.begin() + i1 + cT + 1 + i);
            }
        }
    }
    else{ 

        int qTrocas = tSub1;
        int cT;
        if(i2 < i1){ 

            for(cT = 0; cT < qTrocas; cT++){    

                swap(sCopy.sequency[i2 + cT], sCopy.sequency[i1 + cT]);
            }
            for(int i = 0; i < tSub2 - tSub1; i++){

                sCopy.sequency.insert(sCopy.sequency.begin() + i1 + cT, sCopy.sequency[i2 + cT]);
                
                sCopy.sequency.erase(sCopy.sequency.begin() + i2 + cT);
            }
            
        }
        else{ 

            for(cT = 0; cT < qTrocas; cT++){

                swap(sCopy.sequency[i2+cT], sCopy.sequency[i1+cT]);
            }
            for(int i = 0; i < tSub2 - tSub1; i++){

                sCopy.sequency.insert(sCopy.sequency.begin()+i1+cT + i, sCopy.sequency[i2+cT + i]); // - i?
                
                sCopy.sequency.erase(sCopy.sequency.begin() + i2 + cT + 1 + i);
            }
        }
    }

    return sCopy;
}

Solution ILS(int maxIter, int maxIterILS, Data& adjMatrix){

    size_t n = adjMatrix.getDimension();
    Solution bestOfAll;
    bestOfAll.cost = INFINITY;
    vector<int> CL;
    for(int j = 2; j <= n; j++){ //generating CL
        CL.push_back(j);

    }
    
    for(int i = 0; i < maxIter; i++){

        Solution s = Construction(CL, adjMatrix);
        Solution best = s;
        
        int iterIls = 0;
            
        while(iterIls <= maxIterILS){

            localSearch(s, adjMatrix);
            updateSolutionCost(&s, adjMatrix);
            if(s.cost < best.cost){
                best = s;
                iterIls = 0;
            }

            s = Pertubation(best);
            iterIls++;
        }
        

        if(best.cost < bestOfAll.cost){
            bestOfAll = best;
        }

    }
    
    return bestOfAll;

}


int main(int argc, char** argv) {

    srand((unsigned)time(NULL));
    
    auto data = Data(argc, argv[1]);
    data.read();
    size_t n = data.getDimension();

    int maxIter, maxIterILS;
    Solution best;
    maxIter = 50;
    double sumCost = 0;
    double sumTime = 0;

    for(int i = 0; i < 10; i++){
        auto begin = chrono::high_resolution_clock::now();
        if(n >= 150){   
            maxIterILS = n/2;
        }
        else{
            maxIterILS = n;
        }
        
        best = ILS(maxIter, maxIterILS, data);
            
        auto end = chrono::high_resolution_clock::now();
        auto time = chrono::duration_cast<chrono::milliseconds>(end - begin);

        sumCost += best.cost;
        sumTime += (time.count()/1000.0);
    }
    sumCost /= 10.0;
    sumTime /= 10.0;

    cout << sumTime << " " << sumCost << endl;

    return 0;
}

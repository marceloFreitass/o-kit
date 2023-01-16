#include "Data.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <chrono>

using namespace std;

typedef struct{

    vector<int> sequency;
    double cost;
    
} Solution;

struct Subsequence{

    double T, C;
    int W;
    int first, last;

    inline static Subsequence Concatenate(Subsequence& sigma1, Subsequence& sigma2, Data& adjMatrix){

        Subsequence sigma;
        
        double temp = adjMatrix.getDistance(sigma1.last, sigma2.first);
        sigma.W = sigma1.W + sigma2.W;
        sigma.T = sigma1.T + temp + sigma2.T;
        sigma.C = sigma1.C + sigma2.W * (sigma1.T + temp) + sigma2.C;
        sigma.first = sigma1.first;
        sigma.last = sigma2.last;
        
        return sigma;
    }
};

void UpdateAllSubseq(Solution *s, vector<vector<Subsequence>>& subseq_matrix, Data& adjMatrix, int begin = -1, int end = -1){

    int n = s->sequency.size();

    for(int i = 0; i < n; i++){

        subseq_matrix[i][i].W = (i > 0);
        subseq_matrix[i][i].C = 0;
        subseq_matrix[i][i].T = 0;
        subseq_matrix[i][i].first = s->sequency[i];
        subseq_matrix[i][i].last = s->sequency[i];
    }

    if(begin != -1 && end != -1){ //Updating only the subsequences that were changed by the moviments
        for(int i = 0; i <= end; i++){ //Normal
            for(int j = begin; j < n; j++){
                if(i >= j){
                    j = i + 1;
                }
                subseq_matrix[i][j] = Subsequence::Concatenate(subseq_matrix[i][j-1], subseq_matrix[j][j], adjMatrix);
            }
        }
        for(int i = n - 1; i >= begin; i--){ //Reverse
            for(int j = end; j >= 0; j--){
                if(i <= j){
                    j = i - 1;
                }
                subseq_matrix[i][j] = Subsequence::Concatenate(subseq_matrix[i][j+1], subseq_matrix[j][j], adjMatrix);
            }
        }
        
    }
    else{ //From scratch

        for(int i = 0; i < n; i++){
            for(int j = i + 1; j < n; j++){ //Normal
                subseq_matrix[i][j] = Subsequence::Concatenate(subseq_matrix[i][j-1], subseq_matrix[j][j], adjMatrix);
            }
        }

        for(int i = n - 1; i >= 0; i--){ //Reverse
            for(int j = i - 1; j >= 0; j--){
                subseq_matrix[i][j] = Subsequence::Concatenate(subseq_matrix[i][j+1], subseq_matrix[j][j], adjMatrix);
            }
        }
    }
}

typedef struct{
    
    int insertedNode;
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

vector <insertionInfo> estimateInsertionCost(int r, vector<int>& CL, Data& adjMatrix){
    
    vector<insertionInfo> insertionCost(CL.size());
    int count = 0;

    for(auto k: CL){

        insertionCost[count].cost = adjMatrix.getDistance(r, k);
        insertionCost[count].insertedNode = k;
        count++;
    }

    return insertionCost;
}

bool compareCost(insertionInfo x, insertionInfo  y){
    
    return (x.cost) <  (y.cost);
}


int binarySearch(int x, int begin, int end, vector<int> CL){

    if(end >= begin){

        int mid = begin + (end - begin) / 2;

        if(CL[mid] == x){
            return mid;
        }

        if(CL[mid] > x){
            return binarySearch(x, begin, mid - 1, CL);
        }

        return binarySearch(x, mid + 1, end, CL);

    }

    return -1;

}

Solution Construction(vector<int>& CL1, Data& adjMatrix){

    vector<int>CL = CL1; //Getting a copy of original CL
    Solution s = {{1,1}, 0};
    int r = 1;
    int find, selected;
    double alpha;
      
    while(!CL.empty()){  

        vector<insertionInfo> insertionCost = estimateInsertionCost(r, CL, adjMatrix);
        sort(insertionCost.begin(), insertionCost.end(), compareCost);
        alpha = rand() / (RAND_MAX + 1.0);
        alpha += 0.0000000000000001; //avoiding alpha 0
        selected = rand() % ((int) ceil(alpha * insertionCost.size()));
        r = insertionCost[selected].insertedNode;
        s.sequency.insert(s.sequency.begin() + s.sequency.size() - 1, r);
            
        //finding the insert node on CL
        int indexR = binarySearch(insertionCost[selected].insertedNode, 0, CL.size() - 1, CL);

        CL.erase(CL.begin() + indexR);
    }
     
    
    updateSolutionCost(&s, adjMatrix);
    return s;
}

bool bestImprovementSwap(Solution* s, vector<vector<Subsequence>>& subseq_matrix, Data& adjMatrix){
    
    int best_i, best_j;
    int n = s->sequency.size();
    double totalCost = subseq_matrix[0][n - 1].C;
    double bestCost = totalCost;

    Subsequence sigma1, sigma2, sigma3, finalSigma;

    for(int i = 1; i < n - 1; i++){
        for(int j = i + 1; j < n - 1; j++){
            
           if(j == i + 1){ //Swapping with the subsequent
                
                sigma1 = Subsequence::Concatenate(subseq_matrix[0][i-1], subseq_matrix[j][j], adjMatrix);
                sigma2 = Subsequence::Concatenate(sigma1, subseq_matrix[i][i], adjMatrix);
                finalSigma = Subsequence::Concatenate(sigma2, subseq_matrix[j + 1][n - 1], adjMatrix);
                
           }
           else{
           sigma1 = Subsequence::Concatenate(subseq_matrix[0][i - 1], subseq_matrix[j][j], adjMatrix);
           sigma2 = Subsequence::Concatenate(sigma1, subseq_matrix[i+1][j-1], adjMatrix);
           sigma3 = Subsequence::Concatenate(sigma2, subseq_matrix[i][i], adjMatrix);
           finalSigma = Subsequence::Concatenate(sigma3, subseq_matrix[j +1][n-1], adjMatrix);

           }
            if(finalSigma.C < bestCost){
                bestCost = finalSigma.C;
                best_i = i;
                best_j = j;
            }
        }
    }

    if(bestCost < totalCost){
        swap(s->sequency[best_i], s->sequency[best_j]);
        UpdateAllSubseq(s, subseq_matrix, adjMatrix, best_i, best_j);
        return true;
    }
    
    return false;
}

bool bestImprovement2Opt(Solution *s, vector<vector<Subsequence>>& subseq_matrix, Data& adjMatrix){

    int best_i, best_j;
    int n = s->sequency.size();
    double totalCost = subseq_matrix[0][n-1].C;
    double bestCost = totalCost;
   
    Subsequence sigma1, finalSigma;

    for(int i = 1; i < n; i++){
        for(int j = i  + 2; j < n - 1; j++){

            sigma1 = Subsequence::Concatenate(subseq_matrix[0][i-1], subseq_matrix[j - 1][i], adjMatrix);
            finalSigma = Subsequence::Concatenate(sigma1, subseq_matrix[j][n-1], adjMatrix);

            if(finalSigma.C < bestCost){
                
                bestCost = finalSigma.C;
                best_i = i;
                best_j = j;
            }
        }
    }

    if(bestCost < totalCost){
        
        int nSwaps = (best_j - best_i)/2;
        
        for(int c = 0; c < nSwaps; c++){

            swap(s->sequency[best_j - 1 - c], s->sequency[best_i + c]); //swap the elements in between
        }

        UpdateAllSubseq(s, subseq_matrix, adjMatrix, best_i, best_j);
        
        return true;
    }
    return false;    
}

bool bestImprovementOrOpt(Solution *s, vector<vector<Subsequence>>& subseq_matrix, int type, Data& adjMatrix){

    int best_i, best_j;
    int n = s->sequency.size();
    Subsequence sigma1, sigma2, finalSigma;
    double totalCost = subseq_matrix[0][n - 1].C;
    double bestCost = totalCost;
    switch(type){

        case 1: //Reinsertion

        for(int i = 1; i < n - 1; i++){
            for(int j = 1; j < n - 1; j++){
                
                if(j == i + 1 || j == i){ //Checking if the reinsertion is in the same place
                    continue;
                    
                }
                else{

                if(i > j){

                    sigma1 = Subsequence::Concatenate(subseq_matrix[0][j - 1], subseq_matrix[i][i], adjMatrix);
                    sigma2 = Subsequence::Concatenate(sigma1, subseq_matrix[j][i - 1], adjMatrix);
                    finalSigma = Subsequence::Concatenate(sigma2, subseq_matrix[i +1][n - 1], adjMatrix);

                }
                else{

                    sigma1 = Subsequence::Concatenate(subseq_matrix[0][i - 1], subseq_matrix[i+1][j - 1], adjMatrix);
                    sigma2 = Subsequence::Concatenate(sigma1, subseq_matrix[i][i], adjMatrix);
                    finalSigma = Subsequence::Concatenate(sigma2, subseq_matrix[j][n - 1], adjMatrix);
                    
                }

                if(finalSigma.C < bestCost){

                    bestCost = finalSigma.C;
                    best_i = i;
                    best_j = j;
                }
                }
            }
        }

        if(bestCost < totalCost){

            
            if(best_i > best_j){

                s->sequency.insert(s->sequency.begin() + best_j, s->sequency[best_i]);
                s->sequency.erase(s->sequency.begin()+best_i + 1);
                UpdateAllSubseq(s, subseq_matrix, adjMatrix, best_j, best_i);
            }
            else{
                s->sequency.insert(s->sequency.begin() + best_j, s->sequency[best_i]);
                s->sequency.erase(s->sequency.begin()+best_i);
                 UpdateAllSubseq(s, subseq_matrix, adjMatrix, best_i, best_j);
            }

            return true;
        }
        return false;


        case 2: //Or-opt-2 

        for(int i = 2; i < n -1; i++){

            for(int j = 1; j < n -1; j++){

                if(abs(j - i) <= 1){ //Avoiding putting the block on the same place
                    
                    continue;

                }
                else{

                if(i > j){

                    sigma1 = Subsequence::Concatenate(subseq_matrix[0][j - 1], subseq_matrix[i][i - 1], adjMatrix);
                    sigma2 = Subsequence::Concatenate(sigma1, subseq_matrix[j][i-2], adjMatrix);
                    finalSigma = Subsequence::Concatenate(sigma2, subseq_matrix[i+1][n - 1], adjMatrix);
                    
                }
                else{

                    sigma1 = Subsequence::Concatenate(subseq_matrix[0][i - 2], subseq_matrix[i+1][j-1], adjMatrix);
                    sigma2 = Subsequence::Concatenate(sigma1, subseq_matrix[i][i - 1], adjMatrix);
                    finalSigma = Subsequence::Concatenate(sigma2, subseq_matrix[j][n-1], adjMatrix);
                    
                }

                if(finalSigma.C < bestCost){
                    
                    bestCost = finalSigma.C;
                    best_i = i;
                    best_j = j;

                }
                }
            }
        }

        if(bestCost < totalCost){
           
            if(best_i < best_j){
            s->sequency.insert(s->sequency.begin()+best_j, s->sequency[best_i]);
            s->sequency.insert(s->sequency.begin()+best_j +1, s->sequency[best_i - 1]);
            s->sequency.erase(s->sequency.begin()+best_i-1);
            s->sequency.erase(s->sequency.begin()+best_i-1);
            UpdateAllSubseq(s, subseq_matrix, adjMatrix, best_i-1, best_j-1);
            
            }
            else{
            s->sequency.insert(s->sequency.begin()+best_j, s->sequency[best_i - 1]);
            s->sequency.insert(s->sequency.begin()+best_j, s->sequency[best_i+1]);
            s->sequency.erase(s->sequency.begin()+best_i+1);
            s->sequency.erase(s->sequency.begin()+best_i+1);
            UpdateAllSubseq(s, subseq_matrix, adjMatrix, best_j, best_i);
            
            }
            
            return true;
        }
        return false;

        case 3: 

        for(int i = 3; i < n - 1; i++){
            for(int j = 1; j < n -1; j++){

                if(abs(j - i) <= 2){ //Avoiding putting the block on same place

                    continue;
                }
                
                if(i > j){

                    sigma1 = Subsequence::Concatenate(subseq_matrix[0][j - 1], subseq_matrix[i][i-2], adjMatrix);
                    sigma2 = Subsequence::Concatenate(sigma1, subseq_matrix[j][i - 3], adjMatrix);
                    finalSigma = Subsequence::Concatenate(sigma2, subseq_matrix[i+1][n - 1], adjMatrix);
                
                }
                else{

                    sigma1 = Subsequence::Concatenate(subseq_matrix[0][i - 3], subseq_matrix[i+1][j - 1], adjMatrix);
                    sigma2 = Subsequence::Concatenate(sigma1, subseq_matrix[i][i - 2], adjMatrix);
                    finalSigma = Subsequence::Concatenate(sigma2, subseq_matrix[j][n - 1], adjMatrix);
                }


                if(finalSigma.C < bestCost){
                   
                    bestCost = finalSigma.C;
                    best_i = i;
                    best_j = j;
                }
            }
        }

        if(bestCost < totalCost){
          
            
            if(best_i < best_j){
                s->sequency.insert(s->sequency.begin()+best_j, s->sequency[best_i]);
                s->sequency.insert(s->sequency.begin()+best_j+1, s->sequency[best_i-1]);
                s->sequency.insert(s->sequency.begin()+best_j+2, s->sequency[best_i-2]);
                s->sequency.erase(s->sequency.begin()+best_i-1);
                s->sequency.erase(s->sequency.begin()+best_i-1);
                s->sequency.erase(s->sequency.begin()+best_i-2);
                UpdateAllSubseq(s, subseq_matrix, adjMatrix, best_i-2, best_j-1); 
            }
            else{
                s->sequency.insert(s->sequency.begin()+best_j, s->sequency[best_i]);
                s->sequency.insert(s->sequency.begin()+best_j+1, s->sequency[best_i]);
                s->sequency.insert(s->sequency.begin()+best_j+2, s->sequency[best_i]);
                s->sequency.erase(s->sequency.begin()+best_i+1);
                s->sequency.erase(s->sequency.begin()+best_i+1);
                s->sequency.erase(s->sequency.begin()+best_i+1);
                UpdateAllSubseq(s, subseq_matrix, adjMatrix, best_j, best_i);   
            }
            
            return true;

        }
        return false;

    }
    return false;
}

void localSearch(Solution& s, vector<vector<Subsequence>>& subseq_matrix,  Data& adjMatrix){


    vector<int> NL = {1,2,3,4,5};
    bool improved = 0;
    
    while(!NL.empty()){

        int n = rand() % NL.size();

        switch(NL[n]){


            case 1:
                improved = bestImprovementSwap(&s, subseq_matrix, adjMatrix);
                
                break;
            case 2:
                improved = bestImprovement2Opt(&s, subseq_matrix, adjMatrix);
                
                break;
            case 3:
                improved = bestImprovementOrOpt(&s,subseq_matrix, 1, adjMatrix);
                
                break;
            case 4:
                improved = bestImprovementOrOpt(&s,subseq_matrix, 2, adjMatrix);
                
                break;
            case 5:
                improved = bestImprovementOrOpt(&s,subseq_matrix, 3, adjMatrix);
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

        int nSwaps = tSub2;
        int cT;
        if(i1 < i2){ //if subset1 before subset2

            for(cT = 0; cT < nSwaps; cT++){    

                swap(sCopy.sequency[i1 + cT], sCopy.sequency[i2 + cT]);
            }
            for(int i = 0; i < tSub1 - tSub2; i++){

                sCopy.sequency.insert(sCopy.sequency.begin() + i2 + cT, sCopy.sequency[i1 + cT]);
                sCopy.sequency.erase(sCopy.sequency.begin() + i1 + cT);
            }
            
        }
        else{

            for(cT = 0; cT < nSwaps; cT++){

                swap(sCopy.sequency[i1+cT], sCopy.sequency[i2+cT]);
            }
            for(int i = 0; i < tSub1 - tSub2; i++){

                sCopy.sequency.insert(sCopy.sequency.begin()+i2+cT + i, sCopy.sequency[i1+cT+i]);
                sCopy.sequency.erase(sCopy.sequency.begin() + i1 + cT + 1 + i);
            }
        }
    }
    else{ //subset2 bigger

        int nSwaps = tSub1;
        int cT;
        if(i2 < i1){

            for(cT = 0; cT < nSwaps; cT++){    

                swap(sCopy.sequency[i2 + cT], sCopy.sequency[i1 + cT]);
            }
            for(int i = 0; i < tSub2 - tSub1; i++){

                sCopy.sequency.insert(sCopy.sequency.begin() + i1 + cT, sCopy.sequency[i2 + cT]);
                
                sCopy.sequency.erase(sCopy.sequency.begin() + i2 + cT);
            }
            
        }
        else{

            for(cT = 0; cT < nSwaps; cT++){

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
    double bestcostAcum;
    double bestOfAllcostAcum = INFINITY;
    vector<vector<Subsequence>> subseq_matrix(n + 1, vector<Subsequence>(n + 1));

    for(int j = 2; j <= n; j++){ //generating CL
        CL.push_back(j);
    }
    
    for(int i = 0; i < maxIter; i++){
    
        Solution s = Construction(CL, adjMatrix);
        UpdateAllSubseq(&s, subseq_matrix, adjMatrix);
        Solution best = s;
        bestcostAcum = subseq_matrix[0][n].C; 
        
        int iterIls = 0;
            
        while(iterIls <= maxIterILS){

            localSearch(s, subseq_matrix, adjMatrix);
            updateSolutionCost(&s, adjMatrix);
            if(subseq_matrix[0][n].C < bestcostAcum){
                best = s;
                iterIls = 0;
                bestcostAcum = subseq_matrix[0][n].C;
            }

            s = Pertubation(best);
            UpdateAllSubseq(&s, subseq_matrix, adjMatrix);
            iterIls++;
        }
        
        if(bestcostAcum < bestOfAllcostAcum){
            bestOfAll = best;
            bestOfAllcostAcum = bestcostAcum;
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
    maxIter = 10;
    double sumCost = 0;
    double sumTime = 0;

    if(n < 100){
        maxIterILS = n;
    }
    else{
        maxIterILS = 100;
    }

    for(int i = 0; i < 10; i++){
        vector<vector<Subsequence>> subseq_matrix(n + 1, vector<Subsequence>(n + 1));   
        auto begin = chrono::high_resolution_clock::now(); 
        

        best = ILS(maxIter, maxIterILS, data);
            
        UpdateAllSubseq(&best, subseq_matrix, data);
        auto end = chrono::high_resolution_clock::now();
        auto time = chrono::duration_cast<chrono::milliseconds>(end - begin);

        sumTime += time.count()/1000.0;
        sumCost += subseq_matrix[0][n].C;

    }

    sumTime /= 10.0;
    sumCost /= 10.0;
    
    cout << fixed <<  sumTime << " " << sumCost << endl;
    
    return 0;
}

#pragma once
#include <vector>
#include <utility>
#include <iostream>

using namespace std;

class Node{

    private:

        vector<pair<int, int>> MST;
        vector<pair<int, int>> forbiddenEdges;
        double lowerBound; // 1-MST cost + 2 lambda
        int chosen; // high degree
        bool feasible; //all degree = 2
        vector<int> subgradient;
        vector<double> multipliers;

    public:

        Node();
        
        void setMST(vector<pair<int, int>> MST);
        void setForbiddenEdges(vector<pair<int, int>> forbiddenEdges);
        void setLB(double lowerBound);
        void setSubgradient(vector<int> subgradient);
        void setMultipliers(vector<double> multipliers);

        void setChosen();
        void setFeasible();
        
        void addForbiddenEdges(pair<int, int> arc);

        vector<pair<int, int>> getMST();
        vector<int> getSubgradient();
        double getLB(); 
        vector<double> getMultipliers();
        bool getFeasible();
        vector<pair<int, int>> getForbiddenEdges();

        vector<pair<int, int>> getChosenEdges();
        


};
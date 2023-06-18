#pragma once

#include "Node.h"
#include "Kruskal.h"
#include <vector>
#include <utility>
#include <iostream>
#include <limits>
class Lagrange{

    private:

        vector<vector<double>> originalCost; //qse n vai mudar
        vector<vector<double>> lagrangianCosts;
        vector<int> subgradient; //talvez seja inutil
        vector<double> multipliers; //talvez seja inutil
        double stepSize;
        double upperBound;
        double EPSILON;

    public:

        Lagrange(double upperBound, vector<vector<double>> originalCost);

        void setOriginalCost(vector<vector<double>> originalCost);
        void setLagrangianCosts(vector<vector<double>> lagrangianCosts);
        void setSubgradient(vector<int> subgradient);
        void setMultipliers(vector<double> multipliers);
        void setStepsize(double stepSize);
        void setUpperBound(double upperBound);

        vector<int> getDegree(Node* node); //Aux

        void updateLagrangianCosts();
        
        
        void calculateSubgradient(Node& node);
        void calculateStepSize(Node* node);
        void updateMultipliers(Node& node);

        bool checkMultipliers();
        bool checkSubgradient();
        void subgradientMethod(Node& node);

        



        




};
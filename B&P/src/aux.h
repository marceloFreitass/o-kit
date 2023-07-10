#pragma once;

#include "Data.h"
#include "MasterProblem.h"
#include "Subproblem.h"
#include "Node.h"
#include <iostream>
#include <chrono>
#include <utility>

extern void CG(MasterProblem& PM, Subproblem& SP, Data * data, Node& node);
extern pair<int, int> getMostFractional(vector<double> lambdaValues, vector<vector<bool>> A);
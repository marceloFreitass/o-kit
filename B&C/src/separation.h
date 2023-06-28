//---------------------------------------------------------------------------

/***************************************************/
/* Functions prototypes by Prof. Anand Subramanian */
/***************************************************/

#ifndef Separation_H
#define Separation_H

#include <ilcplex/ilocplex.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include <list>
#include <vector>
#include <algorithm>
#include <iterator>
#include <pthread.h>
#include <bits/stdc++.h>

#define EPSILON 0.00000001

using namespace std;

typedef struct{

    vector<int> nodes;
    int id;
    bool is_vertexA;
    
} vertex_type;

extern vector <vector <int>> MaxBack (double ** weight, int dimension);
extern vector <vector <int>> MinCut (double ** weight, int dimension);


#endif

//---------------------------------------------------------------------------

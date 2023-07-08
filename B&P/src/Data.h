#pragma once

#include <string>
#include <fstream>
#include <vector>

using namespace std;

class Data{

    private:

        int quantItems;
        int binCapacity;
        vector<int> itemWeight;

    public:

    Data(char* instance);

    int getQuantItems();
    int getBinCapacity();
    int getItemWeight(int i);

};
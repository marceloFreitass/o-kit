#include "Data.h"

Data::Data(char* instance){

    fstream fp;
    string linha;
    int weight;
    fp.open(instance, fstream::in);


    getline(fp, linha);
    quantItems = stoi(linha);
    getline(fp, linha);
    binCapacity = stoi(linha);

    for(int i = 0; i < quantItems; i++){
        getline(fp, linha);
        weight = stoi(linha);
        itemWeight.push_back(weight);
    }
}

int Data::getQuantItems(){return quantItems;}
int Data::getBinCapacity(){return binCapacity;}
int Data::getItemWeight(int i){return itemWeight[i];}


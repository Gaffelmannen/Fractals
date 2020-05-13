#ifndef SIERPINSKI_CARPET_H
#define SIERPINSKI_CARPET_H

#include <iostream>
#include <fstream>
#include <complex>
#include <vector>
#include <map>
#include <cmath>
#include <sstream>

#include "FileManager.h"

using namespace std;

class SierpinskiCarpet
{
    private:
    static const int DIMENSION;
    int limit;
    int** array;
    void Rectangle(int, int, int, int);
    void GenerateSierpinskiCarpet(int, int, int);
    void PrintInASCII();
    vector<vector<int>> ConvertToVector();
    
    public:
    SierpinskiCarpet(void);
    ~SierpinskiCarpet(void);
    int getDimension(void);
    void DrawSierpinskiCarpet(int depth, string filename);
};

#endif

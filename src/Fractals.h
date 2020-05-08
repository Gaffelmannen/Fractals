#ifndef FRACTALS_H
#define FRACTALS_H

#include <iostream>
#include <fstream>
#include <complex>
#include <vector>
#include <map>
#include <cmath>
#include <sstream>

#include "FileManager.h"

using namespace std;

class Fractals
{
    private:
    static const double EULER_CONSTANT;
    static const double WIDTH;
    static const double HEIGHT;
    static const int CUTOFF_VALUE;
    double positionReal = 0;
    double positionImg = 0;
    double zoomReal = 0.0;
    double zoomImg = 0.0;
    int selectedAlgorithm = 0;
    map<int, vector<int>> colorGradientMap;
    void setStartPosition(int);
    void setStartZoom(int);
    vector<int> CalculateValue(int, int);
    vector<int> CalculateValueSquaring(int, int);
    vector<int> MapColor(int);
    
    public:
    Fractals(void);
    double getWidth(void);
    double getHeight(void);
    void GenerateMandelbrotSet(std::string);
};

#endif

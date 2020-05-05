#ifndef FRACTALS_H
#define FRACTALS_H

#include <iostream>
#include <fstream>
#include <complex>
#include <vector>
#include <map>
#include <cmath>

using namespace std;

class Fractals
{
    private:
    static const double EulerConstant;
    static const string BASE_DIR;
    static const double WIDTH;
    static const double HEIGHT;
    static const int CUTOFF_VALUE;
    double positionReal = 0;//001643721971153;
    double positionImg = 0;//0.822467633298876;
    double zoomReal = - 0.75;
    double zoomImg = 0.1;
    map<int, vector<int>> colorGradientMap;
    void setStartPosition(void);
    vector<int> CalculateValue(int, int);
    vector<int> MapColor(int);
    
    public:
    Fractals(void);
    double getWidth(void);
    double getHeight(void);
    void DrawMandelbrotSet(std::string);
};

#endif

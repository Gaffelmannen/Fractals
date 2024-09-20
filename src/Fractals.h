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
    public:
    //typedef struct Point point;
    typedef struct Frame frame;
    typedef struct Rectangle rect;
    typedef struct DoubleFrame doubleFrame;

    typedef struct Point 
    {
        double x, y;
    } point;
    Fractals(void);
    double getWidth(void);
    double getHeight(void);
    void GenerateMandelbrotSet(std::string);
    void GenerateMandelbrotAnimation(
        std::string filename, 
        double t, 
        int maxIteration, 
        Point *points, 
        int pointsCount
    );

    private:
    static const double EULER_CONSTANT;

    int debug;
    int cutoff_value;
    int treshold_R;
    int treshold_G;
    int treshold_B;
    int write_to_ppm;
    int write_to_jpg;
    int number_of_threads;
    int max_count_x;
    int max_count_y;
    double width;
    double height;
    double startX;
    double startY;
    double sizeX;
    double sizeY;

    complex<double> positionStart;
    complex<double> zoomStart;
    int selectedAlgorithm = 0;
    map<int, vector<int>> colorGradientMap;
    void setStartPosition(int);
    void setStartZoom(int);
    vector<int> CalculateValue(int, int);
    vector<int> CalculateValueSquaring(int, int);
    vector<int> MapColor(int);
    void mandelbrotPartial(Frame*, double, Point, int, Rectangle);
    int* transformation(Frame*);
    void ShowPercent(int, int);
};

#endif

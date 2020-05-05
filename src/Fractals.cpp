#include "Fractals.h"

const double Fractals::EulerConstant = std::exp(1.0);
const double Fractals::WIDTH = 800;
const double Fractals::HEIGHT = 800;
const int Fractals::CUTOFF_VALUE = 1000;

Fractals::Fractals()
{
    Fractals::colorGradientMap.insert(make_pair(0, vector<int>{0, 0, 0}));
    Fractals::colorGradientMap.insert(make_pair(1, vector<int>{66, 30, 15}));
    Fractals::colorGradientMap.insert(make_pair(2, vector<int>{25, 7, 26}));
    Fractals::colorGradientMap.insert(make_pair(3, vector<int>{9, 1, 47}));
    Fractals::colorGradientMap.insert(make_pair(4, vector<int>{4, 4, 73}));
    Fractals::colorGradientMap.insert(make_pair(5, vector<int>{0, 7, 100}));
    Fractals::colorGradientMap.insert(make_pair(6, vector<int>{12, 44, 138}));
    Fractals::colorGradientMap.insert(make_pair(7, vector<int>{24, 82, 177}));
    Fractals::colorGradientMap.insert(make_pair(8, vector<int>{57, 125, 209}));
    Fractals::colorGradientMap.insert(make_pair(9, vector<int>{134, 181, 229}));
    Fractals::colorGradientMap.insert(make_pair(10, vector<int>{211, 236, 248}));
    Fractals::colorGradientMap.insert(make_pair(11, vector<int>{241, 233, 191}));
    Fractals::colorGradientMap.insert(make_pair(12, vector<int>{248, 201, 95}));
    Fractals::colorGradientMap.insert(make_pair(13, vector<int>{255, 170, 0}));
    Fractals::colorGradientMap.insert(make_pair(14, vector<int>{204, 128, 0}));
    Fractals::colorGradientMap.insert(make_pair(15, vector<int>{153, 87, 0}));
    Fractals::colorGradientMap.insert(make_pair(16, vector<int>{106, 52, 3}));
    
    //setStartPosition();
}

double Fractals::getWidth(void)
{
    return Fractals::WIDTH;
}

double Fractals::getHeight(void)
{
    return Fractals::HEIGHT;
}

void Fractals::setStartPosition(void)
{
    positionReal = - EulerConstant / 7;
    positionImg = - EulerConstant / 20;
}

vector<int> Fractals::MapColor(int numberOfIterations)
{
    if (numberOfIterations < CUTOFF_VALUE && numberOfIterations > 0)
    {
        int i = numberOfIterations % 16;
        return colorGradientMap[i];
    }
    
    return colorGradientMap[0];
}

vector<int> Fractals::CalculateValue(int a, int b)
{
    complex<double> point(
        (double) a / Fractals::getWidth() + zoomReal,
        (double) b / Fractals::getHeight() + zoomImg
    );
    complex<double> z(positionReal, positionImg);
    
    unsigned int numberOfIterations = 0;
    
    while (abs(z) < 4.0 && numberOfIterations <= Fractals::CUTOFF_VALUE)
    {
           z = z * z + point;
           numberOfIterations++;
    }
    
    return Fractals::MapColor(numberOfIterations);
}

void Fractals::DrawMandelbrotSet(std::string filename)
{
    vector<string> rows;
    
    for (int i = 0; i < Fractals::WIDTH; i++)
    {
         for (int j = 0; j < Fractals::HEIGHT; j++)
         {
             vector<int> pos = Fractals::CalculateValue(i, j);
             char row[100];
             sprintf(row, "%i %i %i \n", pos[0], pos[1], pos[2]);
             rows.push_back(row);
         }
    }
    
    FileManager fm;
    fm.WriteToPPMFile(filename, Fractals::WIDTH, Fractals::HEIGHT, rows);
    
    return;
}

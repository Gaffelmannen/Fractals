#include "Fractals.h"

const double Fractals::WIDTH = 800;
const double Fractals::HEIGHT = 800;
const int Fractals::CUTOFF_VALUE = 2000;

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
}

double Fractals::getWidth(void)
{
    return Fractals::WIDTH;
}

double Fractals::getHeight(void)
{
    return Fractals::HEIGHT;
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
    
    while (abs(z) < 8.0 && numberOfIterations <= Fractals::CUTOFF_VALUE)
    {
           z = z * z + point;
           numberOfIterations++;
    }
    
    return Fractals::MapColor(numberOfIterations);
}

void Fractals::Draw(std::string filename)
{
    string path = "data/" + filename + ".ppm";
    
    ofstream theImage(path);
    
    if (theImage.is_open ())
    {
        theImage
            << "P3\n" << Fractals::WIDTH
            << " " << Fractals::HEIGHT
            << " 255\n";
        
        cout << "Working" << endl;
        
        for (int i = 0; i < Fractals::WIDTH; i++)
        {
             for (int j = 0; j < Fractals::HEIGHT; j++)
             {
                 vector<int> positionValue = Fractals::CalculateValue(i, j);
                 theImage
                    << positionValue[0] << ' '
                    << positionValue[1] << ' '
                    << positionValue[2] << endl;
             }
        }
        theImage.close();
    }
    else
    {
        cout << "Could not open the file";
    }
    
    return;
}

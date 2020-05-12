#include "Fractals.h"

#define START_POS_SEA_HORSE 0
#define START_POS_VALLEY 1
#define START_POS_DEFAULT 2

#define START_ZOOM_ONE 1
#define START_ZOOM_TWO 2

#define ALGORITHM_SQUARING 1
#define ALGORITHM_DEFAULT 2

#define WRITE_TO_PPM 0
#define WRITE_TO_JPEG 1

const double Fractals::EULER_CONSTANT = std::exp(1.0);
const double Fractals::WIDTH = 2000;
const double Fractals::HEIGHT = 2000;
const int Fractals::CUTOFF_VALUE = 100;

Fractals::Fractals()
{
    FileManager fm;
    map<int, vector<int>> gradient_map =
        fm.ReadFromGradientFile("gradient-style-four");
    
    for (int i=0; i < gradient_map.size(); i++)
    {
        Fractals::colorGradientMap.insert(make_pair(i, gradient_map[i]));
    }
    
    setStartPosition(START_POS_DEFAULT);
    setStartZoom(START_ZOOM_ONE);
    selectedAlgorithm = ALGORITHM_DEFAULT;
}

double Fractals::getWidth(void)
{
    return Fractals::WIDTH;
}

double Fractals::getHeight(void)
{
    return Fractals::HEIGHT;
}

void Fractals::setStartPosition(int selectedStartPos)
{
    switch (selectedStartPos)
    {
        case START_POS_SEA_HORSE:
            positionStart = complex<double>
                (- EULER_CONSTANT / 7, - EULER_CONSTANT / 20);
            break;
        case START_POS_VALLEY:
            positionStart = complex<double>
                (0.1643721971153, 0.822467633298876);
            break;
        case START_POS_DEFAULT:
        default:
            positionStart = complex<double>
                (0.0, 0.0);
            break;
    }
}

void Fractals::setStartZoom(int selectedStartZoom)
{
    switch (selectedStartZoom)
    {
        case START_ZOOM_TWO:
            zoomStart = complex<double>
                (-0.75, 0.1);
            break;
        case START_ZOOM_ONE:
        default:
            zoomStart = complex<double>
                (-1.5,- 0.5);
            break;
    }
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
    complex<double> c
    (
        (double) a / Fractals::getWidth() + zoomStart.real(),
        (double) b / Fractals::getHeight() + zoomStart.imag()
    );
    
    complex<double> z
    (
        positionStart.real(),
        positionStart.imag()
    );
    
    unsigned int numberOfIterations = 0;
    
    while (abs(z) < 4.0 && numberOfIterations <= Fractals::CUTOFF_VALUE)
    {
           z = z * z + c;
           numberOfIterations++;
    }
    
    return Fractals::MapColor(numberOfIterations);
}

vector<int> Fractals::CalculateValueSquaring(int a, int b)
{
    complex<double> c
    (
        (double) a / Fractals::getWidth() + zoomStart.real(),
        (double) b / Fractals::getHeight() + zoomStart.imag()
    );
    
    complex<double> z
    (
        positionStart.real(),
        positionStart.imag()
    );
    
    double zrsqr = z.real() * z.real();
    double zisqr = z.imag() * z.imag();
    
    unsigned int numberOfIterations = 0;
    
    while (zrsqr + zisqr <= 4.0 && numberOfIterations <= Fractals::CUTOFF_VALUE)
    {
        complex<double> z0
        (
            (double)( zrsqr - zisqr + c.real() ),
            (double)( (norm(z.real() + z.imag()) - zrsqr - zisqr) + c.imag() )
        );
        z = z0;
        
        zrsqr = z.real() * z.real();
        zisqr = z.imag() * z.imag();
        
        numberOfIterations++;
    }
    
    return Fractals::MapColor(numberOfIterations);
}

void Fractals::GenerateMandelbrotSet(std::string filename)
{
    vector<vector<int>> rows;
    
    cout << "Generating Mandelbrot - Begin" << endl;
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < Fractals::WIDTH; i++)
    {
        for (int j = 0; j < Fractals::HEIGHT; j++)
        {
            vector<int> pos;
            switch (selectedAlgorithm)
            {
                case ALGORITHM_SQUARING:
                    pos = Fractals::CalculateValueSquaring(i, j);
                    break;
                case ALGORITHM_DEFAULT:
                default:
                    pos = Fractals::CalculateValue(i, j);
                    break;
            }

            rows.push_back(pos);
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    cout << "Generating Mandelbrot - Done" << endl;
    
    auto elapsed = end - start;
    long long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();

    cout << "Time taken by mandelbrot generation is: " << fixed;
    cout << setprecision(3) << double(milliseconds / 1000.0);
    cout << " seconds " << endl;
    
    FileManager* fm = new FileManager();
    
    if(WRITE_TO_PPM)
    {
        fm->WriteToPPMFile(filename, Fractals::WIDTH, Fractals::HEIGHT, rows);
    }
    
    if(WRITE_TO_JPEG)
    {
        fm->WriteToJpegFile(filename, Fractals::WIDTH, Fractals::HEIGHT, rows);
    }
    
    delete fm;
    
    return;
}

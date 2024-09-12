#include "Fractals.h"

#include "hamilton.h"

#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <atomic>

using namespace std;

#define START_POS_SEA_HORSE 0
#define START_POS_VALLEY 1
#define START_POS_DEFAULT 2

#define START_ZOOM_ONE 1
#define START_ZOOM_TWO 2

#define ALGORITHM_SQUARING 1
#define ALGORITHM_DEFAULT 2

#define WRITE_TO_PPM 0
#define WRITE_TO_JPEG 1

#define NUMBER_OF_THREADS 5
#define MAX_COUNT_X 20000
#define MAX_COUNT_Y 20000


const double Fractals::EULER_CONSTANT = std::exp(1.0);
const double Fractals::WIDTH = 2000;
const double Fractals::HEIGHT = 2000;
const int Fractals::CUTOFF_VALUE = 100;
const double Fractals::START_X = -2.5;
const int Fractals::SIZE_X = 4;
const double Fractals::SIZE_Y = ((double) SIZE_X / WIDTH * HEIGHT);
const double Fractals::START_Y = (-SIZE_Y / 2.0);
const int Fractals::TRESHOLD_R = 256;
const int Fractals::TRESHOLD_G = 64;
const int Fractals::TRESHOLD_B = 16;



struct Rectangle 
{
    double x, y, w, h;
};

struct Frame 
{
    atomic<int> red, green, blue;
};

struct DoubleFrame
{
	double r, g, b;
	DoubleFrame(const Frame& rgb) : r(rgb.red), g(rgb.green), b(rgb.blue) {}
};

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
        cerr << "\rRows remaining: " << i-Fractals::WIDTH << ' ' << flush;
        
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

int* Fractals::transformation(Frame* frames)
{
	int *pixels = new int[WIDTH * HEIGHT * 3];

	DoubleFrame maxValues = frames[0];
	for(int i = 0; i < WIDTH * HEIGHT; i++)
    {
		if(maxValues.r < frames[i].red) maxValues.r = frames[i].red;
		if(maxValues.g < frames[i].green) maxValues.g = frames[i].green;
		if(maxValues.b < frames[i].blue) maxValues.b = frames[i].blue;
	}

	int maxValue = max(max(maxValues.r, maxValues.g), maxValues.b);

	for(int y = 0; y < HEIGHT; y++)
    {
		for(int x = 0; x < WIDTH; x++)
        {
            int frameval = y * WIDTH + x;
			DoubleFrame color = frames[frameval];
			color.r = sqrt(color.r);
			color.g = sqrt(color.g);
			color.b = sqrt(color.b);

			color.r /= sqrt(maxValue);
			color.g /= sqrt(maxValue);
			color.b /= sqrt(maxValue);

			color.r = 1 - color.r;
			color.g = 1 - color.g;
			color.b = 1 - color.b;

			color.r *= color.r*color.r*color.r*color.r*color.r;
			color.g *= color.g*color.g*color.g*color.g*color.g*color.g;
			color.b *= color.b*color.b*color.b*color.b*color.b;

			color.r = 1 - color.r;
			color.g = 1 - color.g;
			color.b = 1 - color.b;

			color.r *= color.r;

			int cr = max(0.0, min(255.0, color.r * 256));
			int cg = max(0.0, min(255.0, color.g * 256));
			int cb = max(0.0, min(255.0, color.b * 256));
			
            int pix1 = (y * WIDTH + x) * 3 + 0;
            int pix2 = (y * WIDTH + x) * 3 + 1;
            int pix3 = (y * WIDTH + x) * 3 + 2;

			pixels[pix1] = cr;
			pixels[pix2] = cg;
			pixels[pix3] = cb;
		}
	}

	return pixels;
}

void Fractals::mandelbrotPartial(Frame* counters, double t, Point p, int maxIteration, Rectangle render)
{
	double tValue = t * t * 1.5;

	Hamilton c = Hamilton(p.x, p.y, (t < 0 ? tValue : 0), (t > 0 ? tValue : 0));
	Hamilton z = c;

	int iteration = 0;

	while(z.magnitude() < 5 && iteration < maxIteration)
    {
		iteration++;
		z = z.multiply(z).add(c);

		int zw = (z.w - render.x) / render.w * WIDTH;
		int zi = (z.i - render.y) / render.h * HEIGHT;

		if(zw >= 0 && zw < WIDTH && zi >= 0 && zi < HEIGHT)
        {
            int r = zi * WIDTH + zw;
            int g = zi * WIDTH + zw;
            int b = zi * WIDTH + zw;

			if(iteration <= TRESHOLD_R) counters[r].red++;
			if(iteration <= TRESHOLD_G) counters[g].green++;
			if(iteration <= TRESHOLD_B) counters[b].blue++;
		}
	}
}

void Fractals::GenerateMandelbrotAnimation(string filename, double t, int maxIteration, Point *points, int pointsCount)
{
	Rectangle render = {START_X, START_Y, SIZE_X, SIZE_Y};
	double sampleDistX = (double) SIZE_X / WIDTH;
	double sampleDistY = (double) SIZE_Y / HEIGHT;

	Frame *counters = new Frame[WIDTH * HEIGHT]();

	thread *threads = new thread[NUMBER_OF_THREADS];

	for(int j = 0; j < NUMBER_OF_THREADS; j++)
    {
		threads[j] = thread([&](int start)
        {
			for(int i = start; i < MAX_COUNT_X * MAX_COUNT_Y; i += NUMBER_OF_THREADS)
            {
				int x = i / MAX_COUNT_X;
				int y = i % MAX_COUNT_Y;

				if(x % 100 == 0 && y == 0)
                {   
                    cout 
                        << "column " 
                        << x 
                        << " / " 
                        << MAX_COUNT_X 
                        << endl;
                }

				for(int k = 0; k < pointsCount; k++)
                {
					Point p = 
                    {
						START_X + (x + points[k].x) * sampleDistX,
						START_Y + (y + points[k].y) * sampleDistY
					};
					mandelbrotPartial(counters, t, p, maxIteration, render);
				}
			}
		}, j);

	}

	for(int j = 0; j < NUMBER_OF_THREADS; j++)
    {

		threads[j].join();
	}

	int *pixels = transformation(counters);

    FileManager* fm = new FileManager();
    fm->WriteToPPMFile(filename, pixels, WIDTH, HEIGHT);
    delete fm;

	delete[] threads;
	delete[] pixels;
	delete[] counters;
}
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
#include <map>

using namespace std;

#define START_POS_SEA_HORSE 0
#define START_POS_VALLEY 1
#define START_POS_DEFAULT 2

#define START_ZOOM_ONE 1
#define START_ZOOM_TWO 2

#define ALGORITHM_SQUARING 1
#define ALGORITHM_DEFAULT 2

#define PI 3.14159265359

const double Fractals::EULER_CONSTANT = std::exp(1.0);

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
    FileManager* fm = new FileManager();

    // Config
    auto configMap = fm->ReadFromConfigFile("rules");
    
    this->debug = stod(configMap["DEBUG"]);
    this->write_to_ppm = stod(configMap["WRITE_TO_PPM"]);
    this->write_to_jpg = stoi(configMap["WRITE_TO_JPEG"]);
    this->number_of_threads = stoi(configMap["NUMBER_OF_THREADS"]);
    this->max_count_x = stoi(configMap["MAX_COUNT_X"]);
    this->max_count_y = stoi(configMap["MAX_COUNT_Y"]);
    
    this->cutoff_value = stod(configMap["CUTOFF_VALUE"]);
    this->treshold_R = stoi(configMap["TRESHOLD_R"]);
    this->treshold_G = stoi(configMap["TRESHOLD_G"]);
    this->treshold_B = stoi(configMap["TRESHOLD_B"]);

    this->width = stod(configMap["WIDTH"]);
    this->height = stoi(configMap["HEIGHT"]);
    this->startX = stod(configMap["START_X"]);
    this->sizeX = stod(configMap["SIZE_X"]);
    this->sizeY = ((double) this->sizeX / this->width * this->height);
    this->startY = (-this->sizeY / PI);

    // Gradients
    map<int, vector<int>> gradient_map =
        fm->ReadFromGradientFile("gradient-style-four");
    
    for (int i=0; i < gradient_map.size(); i++)
    {
        Fractals::colorGradientMap.insert(
            make_pair(i, gradient_map[i])
        );
    }

    delete fm;
    
    setStartPosition(START_POS_DEFAULT);
    setStartZoom(START_ZOOM_ONE);
    selectedAlgorithm = ALGORITHM_DEFAULT;
}

double Fractals::getWidth(void)
{
    return this->width;
}

double Fractals::getHeight(void)
{
    return this->height;
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
    if (numberOfIterations < this->cutoff_value && numberOfIterations > 0)
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
    
    while (abs(z) < 4.0 && numberOfIterations <= this->cutoff_value)
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
    
    while (zrsqr + zisqr <= 4.0 && numberOfIterations <= this->cutoff_value)
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
    
    for (int i = 0; i < this->width; i++)
    {
        cerr << "\rRows remaining: " << i-this->width << ' ' << flush;
        
        for (int j = 0; j < this->height; j++)
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
    
    if(this->write_to_ppm)
    {
        fm->WriteToPPMFile(filename, this->width, this->height, rows);
    }
    
    if(this->write_to_jpg)
    {
        fm->WriteToJpegFile(filename, this->width, this->height, rows);
    }
    
    delete fm;
    
    return;
}

int* Fractals::transformation(Frame* frames)
{
	int *pixels = new int[this->width * this->height * 3];

	DoubleFrame maxValues = frames[0];
	for(int i = 0; i < this->width * this->height; i++)
    {
		if(maxValues.r < frames[i].red) maxValues.r = frames[i].red;
		if(maxValues.g < frames[i].green) maxValues.g = frames[i].green;
		if(maxValues.b < frames[i].blue) maxValues.b = frames[i].blue;
	}

	int maxValue = max(max(maxValues.r, maxValues.g), maxValues.b);

	for(int y = 0; y < this->height; y++)
    {
		for(int x = 0; x < this->width; x++)
        {
            int frameval = y * this->width + x;
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
			
            int pix1 = (y * this->width + x) * 3 + 0;
            int pix2 = (y * this->width + x) * 3 + 1;
            int pix3 = (y * this->width + x) * 3 + 2;

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

		int zw = (z.w - render.x) / render.w * this->width;
		int zi = (z.i - render.y) / render.h * this->height;

		if(zw >= 0 && zw < this->width && zi >= 0 && zi < this->height)
        {
            int r = zi * this->width + zw;
            int g = zi * this->width + zw;
            int b = zi * this->width + zw;

			if(iteration <= this->treshold_R) counters[r].red++;
			if(iteration <= this->treshold_G) counters[g].green++;
			if(iteration <= this->treshold_B) counters[b].blue++;
		}
	}
}

void Fractals::GenerateMandelbrotAnimation(
    string filename, 
    double t, 
    int maxIteration, 
    Point *points, 
    int pointsCount)
{
    Rectangle render = {this->startX, this->startY, this->sizeX, this->sizeY};
	double sampleDistX = (double) this->sizeX / this->width;
	double sampleDistY = (double) this->sizeY / this->height;

	Frame *counters = new Frame[this->width * this->height]();

    // Thread compatible
    int maxThreads = this->number_of_threads;
    int maxCountX = this->max_count_x;
    int maxCountY = this->max_count_y;
    int startForX = this->startX;
    int startForY = this->startY;

	thread *threads = new thread[maxThreads];

	for(int j = 0; j < maxThreads; j++)
    {
		threads[j] = thread([&](int start)
        {
			for(int i = start; i < maxCountX * maxCountY; i += maxThreads)
            {
				int x = i / maxCountX;
				int y = i % maxCountY;

				if(this->debug && ( x % 100 == 0 && y == 0))
                {   
                    cout 
                        << "column " 
                        << x 
                        << " / " 
                        << maxCountX 
                        << endl;
                }

				for(int k = 0; k < pointsCount; k++)
                {
					Point p = 
                    {
						startForX + (x + points[k].x) * sampleDistX,
						startForY + (y + points[k].y) * sampleDistY
					};
					mandelbrotPartial(counters, t, p, maxIteration, render);
				}
			}
		}, j);
	}

	for(int j = 0; j < maxThreads; j++)
    {
		threads[j].join();
	}

	int *pixels = transformation(counters);

    FileManager* fm = new FileManager();
    if(this->write_to_ppm)
    {
        fm->WriteToPPMFile(filename, pixels, this->width, this->height);
    }
    if(this->write_to_jpg)
    {
        fm->WriteToJpegFile(filename, pixels, this->width, this->height);
    }

    delete fm;
	delete[] threads;
	delete[] pixels;
	delete[] counters;
}
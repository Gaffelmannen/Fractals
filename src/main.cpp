#include <iostream>
#include <string>
#include "Fractals.h"
#include "Sierpinskicarpet.h"

using namespace std;

#define NUMBER_OF_SAMPLES 8
#define NUMBER_OF_FRAMES 361
#define NUMBER_OF_MAX_ITERATIONS 256

double randomizer(){
	return ((double) rand() / (RAND_MAX));
}

bool isNumeric(const string& s)
{
    return !s.empty() &&
            find_if
            (
                s.begin(),
                s.end(), []( unsigned char c )
                {
                    return !isdigit(c);
                }
            ) == s.end();
}

void GenerateMandelbrot()
{
    cout << "Generate Mandelbrot image" << endl;
    
    cout << "Enter filename: ";
    std::string filename;
    cin>>filename;
    
    Fractals fractals;
    fractals.Fractals::GenerateMandelbrotSet(filename);
}

void GenerateSierpinskiCarpet()
{
    cout << "Generate Sierpinski Carpet image" << endl;

    cout << "Enter filename: ";
    std::string filename;
    cin>>filename;
    
    int depth = 0;
    while(depth < 1 || depth > 6)
    {
        cout << "Enter depth 1 to 6: ";
        cin>>depth;
    }
    
    SierpinskiCarpet sc;
    sc.DrawSierpinskiCarpet(depth, filename);
}

void GenerateAnimatedMandelbrot()
{
    cout << "Generate animated mandelbrot." << endl;

    cout << "Enter filename: ";
    std::string mandelbrotfilename;
    cin >> mandelbrotfilename;

    Fractals::point points[NUMBER_OF_SAMPLES];

	srand(time(0));

	for(int i = 0; i < NUMBER_OF_SAMPLES; i++)
    {
		points[i].x = randomizer();
		points[i].y = randomizer();
	}


	for(int i = 0; i < NUMBER_OF_FRAMES; i++)
	{
		cout << "Rendering frame " << i << endl;
		auto start = chrono::steady_clock::now();

		double t = 2.0 * i / NUMBER_OF_FRAMES - 1;

		ostringstream filename;
		filename << mandelbrotfilename 
            << setfill('0') 
            << setw(ceil(log10(NUMBER_OF_FRAMES))) 
            << i 
            << ".ppm";
        
        Fractals* f = new Fractals();
	    f->GenerateMandelbrotAnimation(
            filename.str(), 
            t, 
            NUMBER_OF_MAX_ITERATIONS,
            points, 
            NUMBER_OF_SAMPLES
        );
        delete f;


		auto end = chrono::steady_clock::now();
		cout    << "Frame " 
                << i 
                << " generated in " 
                << chrono::duration_cast<chrono::milliseconds>(end - start).count() / 1000.0 
                << "seconds" 
                << endl;
	}
}

int main()
{
    int choice = 0;
    
    cout << "Begin" << endl;
    
    while (1)
    {
        cout << endl;
        cout << "Menu: " << endl;
        cout << "1. Generate Mandelbrot image." << endl;
        cout << "2. Generate Sierpinski Carpet image." << endl;
        cout << "3. Generate animated Mandelbrot video." << endl;
        cout << "Press q to quit." << endl;
        cout << " > ";
        
        string input;
        cin>>input;
        
        if (input.compare("q") == 0)
        {
            return 0;
        }
        
        if (isNumeric(input))
        {
            choice = stoi(input);
        }
        else
        {
            cout << "Invalid input." << endl;
            continue;
        }
        
        switch (choice)
        {
            case 1:
                GenerateMandelbrot();
                break;
            case 2:
                GenerateSierpinskiCarpet();
                break;
            case 3:
                GenerateAnimatedMandelbrot();
                break;
            default:
                cout << "Invalid selection.";
                break;
        }
        
        cout << endl;
    }
    
    cout << "End" << endl;
}

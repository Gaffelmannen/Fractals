#include <iostream>
#include <string>
#include "Fractals.h"
#include "Sierpinskicarpet.h"

using namespace std;

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
            default:
                cout << "Invalid selection.";
                break;
        }
        
        cout << endl;
    }
    
    cout << "End" << endl;
}

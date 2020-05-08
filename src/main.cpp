#include <iostream>
#include "Fractals.h"

using namespace std;

int main()
{
    cout << "Begin" << endl;
    
    std::string filename;
    cout << "Enter filename: ";
    cin>>filename;
    
    Fractals fractals;
    fractals.Fractals::GenerateMandelbrotSet(filename);
    
    cout << "End" << endl;
}

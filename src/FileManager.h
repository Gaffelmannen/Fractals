#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <iostream>
#include <fstream>
#include <complex>
#include <vector>
#include <map>
#include <cmath>

using namespace std;

class FileManager
{
    private:
    static const string BASE_DIR;
    static const string FILE_ABBREVATION_PPM;
    
    public:
    int WriteToPPMFile(string, int, int, vector<string>);
};

#endif

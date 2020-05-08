#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <iostream>
#include <fstream>
#include <complex>
#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <regex>

using namespace std;

class FileManager
{
    private:
    static const string BASE_DIR;
    static const string SETTINGS_DIR;
    static const string FILE_ABBREVATION_PPM;
    static const string FILE_ABBREVATION_TXT;
    
    public:
    int WriteToPPMFile(string, int, int, vector<string>);
    map<int, vector<int>> ReadFromGradientFile(string);
};

#endif

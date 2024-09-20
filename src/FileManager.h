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
#include <cctype>

#include "../lib/toojpeg.h"

using namespace std;

class FileManager
{
    private:
    int debug;
    static const string BASE_DIR;
    static const string SETTINGS_DIR;
    static const string FILE_ABBREVATION_PPM;
    static const string FILE_ABBREVATION_JPEG;
    static const string FILE_ABBREVATION_TXT;
    static const string FILE_ABBREVATION_INI;
    void WriteOutput(unsigned char);
    
    public:
    FileManager(void);
    int WriteToPPMFile(string, int, int, vector<vector<int>>);
    int WriteToPPMFile(string, int*, int, int);
    int WriteToJpegFile(string, int, int, vector<vector<int>>);
    int WriteToJpegFile(string, int, int, int**);
    int WriteToJpegFile(string, int*, int, int);
    map<int, vector<int>> ReadFromGradientFile(string);
    map<string, string> ReadFromConfigFile(string);
};

#endif

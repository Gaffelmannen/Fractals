#include "FileManager.h"

const string FileManager::BASE_DIR = "data/";
const string FileManager::FILE_ABBREVATION_PPM = ".ppm";

int FileManager::WriteToPPMFile(
    string filename,
    int width,
    int height,
    vector<string> rows)
{
    string path = BASE_DIR + filename + FILE_ABBREVATION_PPM;
    
    ofstream theImage(path);
    
    if (theImage.is_open())
    {
        theImage << "P3";
        theImage << endl;
        theImage << width;
        theImage << " ";
        theImage << height;
        theImage << " 255";
        theImage << endl;
        
        cout << "Working" << endl;
        
        for (int i = 0; i < rows.size(); i++)
        {
            theImage << rows[i];
        }
         
        theImage.close();
    }
    else
    {
        return 1;
    }
    
    return 0;
}

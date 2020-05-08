#include "FileManager.h"

const string FileManager::SETTINGS_DIR = "settings/";
const string FileManager::BASE_DIR = "data/";
const string FileManager::FILE_ABBREVATION_PPM = ".ppm";
const string FileManager::FILE_ABBREVATION_TXT = ".txt";

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
        cout << "Writing to file - Begin" << endl;
        
        theImage << "P3";
        theImage << endl;
        theImage << width;
        theImage << " ";
        theImage << height;
        theImage << " 255";
        theImage << endl;
        
        for (int i = 0; i < rows.size(); i++)
        {
            theImage << rows[i];
        }
         
        theImage.close();
        
        cout << "Writing to file - Done" << endl;
    }
    else
    {
        return 1;
    }
    
    return 0;
}

map<int, vector<int>> FileManager::ReadFromGradientFile(string filename)
{
    map<int, vector<int>> map;
    
    int numberOfReadLines = 0;
    string line;
    string segment;
    
    string path = SETTINGS_DIR + filename + FILE_ABBREVATION_TXT;
    ifstream gradientFile (path);
    
    if (gradientFile.is_open())
    {
        while (std::getline(gradientFile,line))
        {
            vector<string> tokens;
            
            std::regex re("[[:digit:]]+");
            
            std::sregex_token_iterator
                begin(line.begin(), line.end(), re),
                end;

            std::copy(
                begin,
                end,
                std::back_inserter(tokens)
            );
            
            if (tokens.size() == 3)
            {
                vector<int> values;
                for (int i = 0; i < tokens.size(); i++)
                {
                    values.push_back(std::stoi(tokens[i]));
                }
                map.insert(make_pair(numberOfReadLines++, values));
            }
        }
        gradientFile.close();
    }
    else
    {
        cout << "Unable to open file";
    }
    
    return map;
}

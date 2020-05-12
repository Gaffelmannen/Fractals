#include "FileManager.h"

const string FileManager::SETTINGS_DIR = "settings/";
const string FileManager::BASE_DIR = "data/";
const string FileManager::FILE_ABBREVATION_PPM = ".ppm";
const string FileManager::FILE_ABBREVATION_JPEG = ".jpg";
const string FileManager::FILE_ABBREVATION_TXT = ".txt";
std::ofstream myFile;

FileManager::FileManager()
{
    
}

int FileManager::WriteToPPMFile(
    string filename,
    int width,
    int height,
    vector<vector<int>> rows)
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
            vector<int> pos = rows[i];
            char fields[100];
            sprintf(fields, "%i %i %i \n", pos[0], pos[1], pos[2]);
            theImage << fields;
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

void myOutput(unsigned char byte)
{
    myFile << byte;
}

int FileManager::WriteToJpegFile(
    string filename,
    int width,
    int height,
    vector<vector<int>> rows)
{
    const auto bytesPerPixel = 3;
    auto image = new unsigned char[width * height * bytesPerPixel];
    int count = 0;
    
    string path = BASE_DIR + filename + FILE_ABBREVATION_JPEG;
    myFile = std::ofstream(path, std::ios_base::out | std::ios_base::binary);
    
    for (auto y = 0; y < height; y++)
    {
        for (auto x = 0; x < width; x++)
        {
            auto offset = (y * width + x) * bytesPerPixel;
            image[offset] = rows[count][0];
            image[offset+1] = rows[count][1];
            image[offset+2] = rows[count][2];
            count++;
        }
    }
    
    const bool isRGB = true;
    const auto quality = 90;
    const bool downsample = false;
    const char* comment = "Fractal: Mandelbrot Set Image.";
    
    auto ok = TooJpeg::writeJpeg(myOutput, image, width, height, isRGB, quality, downsample, comment);
    
    delete[] image;
    
    return ok ? 0 : 1;
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

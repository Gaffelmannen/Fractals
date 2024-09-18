#include "FileManager.h"

const string FileManager::SETTINGS_DIR = "settings/";
const string FileManager::BASE_DIR = "data/";
const string FileManager::FILE_ABBREVATION_PPM = ".ppm";
const string FileManager::FILE_ABBREVATION_JPEG = ".jpg";
const string FileManager::FILE_ABBREVATION_TXT = ".txt";
const string FileManager::FILE_ABBREVATION_INI = ".ini";
std::ofstream theFile;

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
            snprintf(fields, sizeof(fields), "%i %i %i \n", pos[0], pos[1], pos[2]);
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

int FileManager::WriteToPPMFile(string filename, int* pixels, int width, int height)
{
	cout << "writing file " << filename << endl;

	ofstream file;
	file.open(filename);

	if(!file.is_open())
    {
		return -1;
	}

	file << "P3" << endl;
	file << width << " " << height << endl;
	file << 255 << endl;

	for(int i = 0; i < width * height; i++)
    {

		file    << pixels[i * 3] 
                << " " 
                << pixels[i * 3 + 1] 
                << " " 
                << pixels[i * 3 + 2] 
                << endl;
	}

	file.close();

    return 0;
}

void writeOutput(unsigned char byte)
{
    theFile << byte;
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
    theFile = std::ofstream(path, std::ios_base::out | std::ios_base::binary);
    
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
    
    auto ok = TooJpeg::writeJpeg(writeOutput, image, width, height, isRGB, quality, downsample, comment);
    
    delete[] image;
    
    return ok ? 0 : 1;
}

int FileManager::WriteToJpegFile(
    string filename,
    int width,
    int height,
    int** canvas)
{
    const auto bytesPerPixel = 3;
    auto image = new unsigned char[width * height * bytesPerPixel];
    
    string path = BASE_DIR + filename + FILE_ABBREVATION_JPEG;
    theFile = std::ofstream(path, std::ios_base::out | std::ios_base::binary);
    
    for (int x = 0; x < width; x++)
    {
        int* columns = canvas[x];
        for (int y = 0; y < height; y++)
        {
            auto offset = (y * width + x) * bytesPerPixel;
            if(columns[y] == 1)
            {
                image[offset] = 255;
                image[offset+1] = 255;
                image[offset+2] = 255;
            }
            else
            {
                image[offset] = 0;
                image[offset+1] = 0;
                image[offset+2] = 0;
            }
        }
    }
    
    const bool isRGB = true;
    const auto quality = 90;
    const bool downsample = false;
    const char* comment = "Fractal: Mandelbrot Set Image.";
    
    auto ok = TooJpeg::writeJpeg(writeOutput, image, width, height, isRGB, quality, downsample, comment);
    
    delete[] image;
    
    return ok ? 0 : 1;
}

int FileManager::WriteToJpegFile(string filename, int* pixels, int width, int height)
{
	cout << "writing file " << filename << endl;

	const auto bytesPerPixel = 3;
    auto image = new unsigned char[width * height * bytesPerPixel];
    
    string path = BASE_DIR + filename + FILE_ABBREVATION_JPEG;

    theFile = std::ofstream(path, std::ios_base::out | std::ios_base::binary);

    for (auto y = 0; y < height; y++)
    {
        for (auto x = 0; x < width; x++)
        {
            auto offset = (y * width + x) * bytesPerPixel;
            image[offset] = pixels[offset];
            image[offset+1] = pixels[offset+1];
            image[offset+2] = pixels[offset+2];
        }
    }
    
    cout << "Writing to file - Done" << endl;
    
    const bool isRGB = true;
    const auto quality = 90;
    const bool downsample = false;
    const char* comment = "Fractal: Mandelbrot Set Image.";
    
    auto ok = TooJpeg::writeJpeg(writeOutput, image, width, height, isRGB, quality, downsample, comment);
    
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

map<string, string> FileManager::ReadFromConfigFile(string filename)
{
    map<string, string> configMap;

    string line;
    string path = filename + FILE_ABBREVATION_INI;

    ifstream configFile (path);
    if (configFile.is_open())
    {
        while (std::getline(configFile, line))
        {
            line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());

            std::istringstream is_line(line);
            std::string key;

            if( std::getline(is_line, key, '=') )
            {
                std::string value;
                if(std::getline(is_line, value) ) 
                {
                    configMap.insert(make_pair(key, value));
                }
            }
        }
    }

    //cout << ".." << configMap.at("CUTOFF_VALUE") << "..";

    return configMap;
}
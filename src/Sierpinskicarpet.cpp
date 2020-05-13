#include "Sierpinskicarpet.h"

#define DRAW_ASCII 0

const int SierpinskiCarpet::DIMENSION = 1000;

SierpinskiCarpet::SierpinskiCarpet()
{
    array = new int*[DIMENSION];
    for (int i = 0; i < DIMENSION; ++i)
    {
        array[i] = new int[DIMENSION];
    }
    
    limit = DIMENSION;
}

SierpinskiCarpet::~SierpinskiCarpet()
{
    for(int i = 0; i < DIMENSION; ++i)
    {
        delete array[i];
    }
    delete array;
}

int SierpinskiCarpet::getDimension(void)
{
    return SierpinskiCarpet::DIMENSION;
}

void SierpinskiCarpet::GenerateSierpinskiCarpet(int x, int y, int size)
{
    if (size < limit)
    {
        return;
    }
    
    size = size / 3;
    
    for (int i = 0; i < 9; i++)
    {
        if (i == 4)
        {
            Rectangle(x+size, y+size, size, size);
        }
        else
        {
            GenerateSierpinskiCarpet(
                x + (i%3) * size,
                y + (i/3) * size,
                size
            );
        }
    }
                                     
    return;
}

void SierpinskiCarpet::Rectangle(int x0, int y0, int x1, int y1)
{
    if(x0 >= DIMENSION || y0 >= DIMENSION)
    {
        cout << "x0 is " << x0;
        cout << " and ";
        cout << "y0 is " << y0;
        cout << " which is bigger than the canvas dimensions ";
        cout << DIMENSION << endl;
        return;
    }
    
    if(x0 + x1 >= DIMENSION || y0 + y1 >= DIMENSION)
    {
        cout << "x0 is " << x0;
        cout << "x1 is " << x1;
        cout << "y0 is " << x0;
        cout << "y1 is " << y1;
        cout << "x0 + x1 = " << x0 + x1;
        cout << "y0 + y1 = " << y0 + y1;
        cout << " which is outside the canvas dimensions ";
        cout << DIMENSION << endl;
        return;
    }
    
    for (int x = x0; x < x0 + x1; x++)
    {
        int* columns = array[x];
        
        for (int y = y0; y < y0 + y1; y++)
        {
            columns[y] = 1;
        }
    }
    
    return;
}

void SierpinskiCarpet::DrawSierpinskiCarpet(int depth, string filename)
{
    for (int i = 1; i <= depth; i = i+1)
    {
        GenerateSierpinskiCarpet(0, 0, DIMENSION);
        limit = limit / 3;
    }
    
    FileManager* fm = new FileManager();
    fm->WriteToJpegFile(filename, DIMENSION, DIMENSION, array);
    delete fm;
    
    if (DRAW_ASCII)
    {
        PrintInASCII();
    }
    
    return;
}

vector<vector<int>> SierpinskiCarpet::ConvertToVector()
{
    vector<vector<int>> rows;
    
    for (int x = 0; x < DIMENSION; x++)
    {
        vector<int> columns;
        int* arr = array[x];
        
        for (int y = 0; y < DIMENSION; y++)
        {
            columns.push_back(arr[y]);
        }
        
        rows.push_back(columns);
    }
    
    return rows;
 }

void SierpinskiCarpet::PrintInASCII()
{
    for (int x = 0; x < DIMENSION; x++)
    {
        int* columns = array[x];
        
        for (int y = 0; y < DIMENSION; y++)
        {
            cout << columns[y];
        }
        
        cout << endl;
    }
 }

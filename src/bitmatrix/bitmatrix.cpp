#include "bitmatrix.h"

bitmatrix::bitmatrix(uint32 rows, uint32 cols)
{
    m_rows = (rows == 0)?1:rows;
    m_cols = (cols == 0)?1:cols;
    m_cell = int(cols/8) + (int(cols%8) ? 1 : 0);
    matrix = new uint8*[rows];
    for (int i = 0; i < rows; i++)	
       matrix[i] = new uint8[m_cell];
}

bitmatrix::~bitmatrix()
{
    for (int i = 0; i < m_rows; i++)	
        delete matrix[i];
    delete matrix;
}

void bitmatrix::Randomize(uint32 rows, uint32 cols)
{
    if (rows > m_rows || cols > m_cols)
        return;
    
    if (rand()%2)
        Unset(rows, cols);
    else
        Set(rows, cols);    
}

bool bitmatrix::Get(uint32 rows, uint32 cols)
{
    if (rows > m_rows || cols > m_cols)
        return false;
          
    return matrix[rows][int(cols/8)] & uint8(1 << int(cols%8));
}

void bitmatrix::Set(uint32 rows, uint32 cols)
{
    if (rows > m_rows || cols > m_cols)
        return;
        
    matrix[rows][int(cols/8)] |= uint8(1 << int(cols%8));
}

void bitmatrix::Unset(uint32 rows, uint32 cols)
{
    if (rows > m_rows || cols > m_cols)
        return;
        
    matrix[rows][int(cols/8)] &= ~uint8(1 << int(cols%8));
}

void bitmatrix::Flip(uint32 rows, uint32 cols)
{
    if (rows > m_rows || cols > m_cols)
        return;
        
    if (matrix[rows][int(cols/8)] & uint8(1 << int(cols%8)))
        Unset(rows, cols);
    else
        Set(rows, cols);
}

void bitmatrix::RandomizeAll()
{
    for (int i = 0; i < m_rows; i++)
        for (int j = 0; j < m_cell; j++)
        {
             matrix[i][j] = uint8(rand()%256);
        }
}

void bitmatrix::SetAll()
{
    for (int i = 0; i < m_rows; i++)
        for (int j = 0; j < m_cell; j++)
        {
             matrix[i][j] = uint8(255); // 11111111
        }
}

void bitmatrix::UnsetAll()
{
    for (int i = 0; i < m_rows; i++)
        for (int j = 0; j < m_cell; j++)
        {
             matrix[i][j] = uint8(0); // 00000000
        }
}

void bitmatrix::FlipAll()
{
    for (int i = 0; i < m_rows; i++)
        for (int j = 0; j < m_cell; j++)
        {
             matrix[i][j] = ~uint8(matrix[i][j]);
        }
}

void bitmatrix::SetCol(bitmatrix& bin_mat, uint32 cols)
{
    if (bin_mat.GetCols() < m_cols || cols > m_cols)
        return;

    for (int i = 0; i < m_rows; i++) 
    {    
        uint8 mask = bin_mat.matrix[i][int(cols/8)] & uint8(1 << int(cols%8));
        if (mask)
            matrix[i][int(cols/8)] |= uint8(1 << int(cols%8));
        else 
            matrix[i][int(cols/8)] &= ~uint8(1 << int(cols%8));
    }
}

void bitmatrix::SetRow(bitmatrix& bin_mat, uint32 rows)
{
    if (bin_mat.GetRows() < m_rows || rows > m_rows)
        return;

    for (int j = 0; j < m_cell; j++) 
    {    
        matrix[rows][j] = bin_mat.matrix[rows][j];
    }
}

void bitmatrix::Print()
{
    for (int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < m_cols; j++)
        {
             printf("%d ", (matrix[i][int(j/8)] & uint8(1 << int(j%8)) ? 1 : 0 ));
        }
        printf("\n");
    }
}

string bitmatrix::ToString()
{
    string s;
    for (int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < m_cols; j++)
        {
            s += (matrix[i][int(j/8)] & uint8(1 << int(j%8)))?"1":"0";
            if(j != (m_cols-1))
                s +=",";
        }
        s+="\n";
    }
    return s;
}

#include "bitmatrix.h"

bitmatrix::bitmatrix(uint32 rows, uint32 cols)
{
    m_rows = rows;
    m_cols = cols;
    m_cell = int(cols/8) + (int(cols%8) ? 1 : 0);
    Matrix = new uint8*[rows];
    for (int i = 0; i < rows; i++)	
       Matrix[i] = new uint8[m_cell];
}

bitmatrix::~bitmatrix()
{
    for (int i = 0; i < m_rows; i++)	
        delete Matrix[i];
    delete Matrix;
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
          
    return Matrix[rows][int(cols/8)] & uint8(1 << int(cols%8));
}

void bitmatrix::Set(uint32 rows, uint32 cols)
{
    if (rows > m_rows || cols > m_cols)
        return;
        
    Matrix[rows][int(cols/8)] |= uint8(1 << int(cols%8));
}

void bitmatrix::Unset(uint32 rows, uint32 cols)
{
    if (rows > m_rows || cols > m_cols)
        return;
        
    Matrix[rows][int(cols/8)] &= ~uint8(1 << int(cols%8));
}

void bitmatrix::Flip(uint32 rows, uint32 cols)
{
    if (rows > m_rows || cols > m_cols)
        return;
        
    if (Matrix[rows][int(cols/8)] & uint8(1 << int(cols%8)))
        Unset(rows, cols);
    else
        Set(rows, cols);
}

void bitmatrix::RandomizeAll()
{
    for (int i = 0; i < m_rows; i++)
        for (int j = 0; j < m_cell; j++)
        {
             Matrix[i][j] = uint8(rand()%256);
        }
}

void bitmatrix::SetAll()
{
    for (int i = 0; i < m_rows; i++)
        for (int j = 0; j < m_cell; j++)
        {
             Matrix[i][j] = uint8(255); // 11111111
        }
}

void bitmatrix::UnsetAll()
{
    for (int i = 0; i < m_rows; i++)
        for (int j = 0; j < m_cell; j++)
        {
             Matrix[i][j] = uint8(0); // 00000000
        }
}

void bitmatrix::FlipAll()
{
    for (int i = 0; i < m_rows; i++)
        for (int j = 0; j < m_cell; j++)
        {
             Matrix[i][j] = ~uint8(Matrix[i][j]);
        }
}

void bitmatrix::SetCol(bitmatrix& bin_mat, uint32 cols)
{
    if (bin_mat.GetCols() < m_cols || cols > m_cols)
        return;

    for (int i = 0; i < m_rows; i++) 
    {    
        uint8 mask = bin_mat.Matrix[i][int(cols/8)] & uint8(1 << int(cols%8));
        if (mask)
            Matrix[i][int(cols/8)] |= uint8(1 << int(cols%8));
        else 
            Matrix[i][int(cols/8)] &= ~uint8(1 << int(cols%8));
    }
}

void bitmatrix::SetRow(bitmatrix& bin_mat, uint32 rows)
{
    if (bin_mat.GetRows() < m_rows || rows > m_rows)
        return;

    for (int j = 0; j < m_cell; j++) 
    {    
        Matrix[rows][j] = bin_mat.Matrix[rows][j];
    }
}

void bitmatrix::Print()
{
    for (int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < m_cols; j++)
        {
             printf("%d ", (Matrix[i][int(j/8)] & uint8(1 << int(j%8)) ? 1 : 0 ));
        }
        printf("\n");
    }
}

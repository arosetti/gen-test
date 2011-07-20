#include "bitmatrix.h"

bitmatrix::bitmatrix(uint32 rows, uint32 cols)
{        
    m_rows = (rows ? rows : 1);
    m_cols = (cols ? cols : 1);
    m_cells = int(m_cols/8) + (int(m_cols%8) ? 1 : 0);
    matrix = new uint8*[m_rows];
    for (int i = 0; i < m_rows; i++)	
       matrix[i] = new uint8[m_cells];
}

bitmatrix::~bitmatrix()
{
    for (int i = 0; i < m_rows; i++)	
        delete matrix[i];
    delete matrix;
}

void bitmatrix::Randomize(uint32 rows, uint32 cols)
{
    if ((rows + 1) > m_rows || (cols + 1) > m_cols)
        return;
    
    if (rand()%2)
        Unset(rows, cols);
    else
        Set(rows, cols);    
}

bool bitmatrix::Get(uint32 rows, uint32 cols)
{
    if (rows >= m_rows || cols >= m_cols)
        return false;
          
    return matrix[rows][int(cols/8)] & uint8(1 << int(cols%8));
}

void bitmatrix::Set(uint32 rows, uint32 cols)
{
    if (rows >= m_rows || cols >= m_cols)
        return;
        
    matrix[rows][int(cols/8)] |= uint8(1 << int(cols%8));
}

void bitmatrix::Unset(uint32 rows, uint32 cols)
{
    if (rows >= m_rows || cols >= m_cols)
        return;
        
    matrix[rows][int(cols/8)] &= ~uint8(1 << int(cols%8));
}

void bitmatrix::Flip(uint32 rows, uint32 cols)
{
    if (rows >= m_rows || cols >= m_cols)
        return;
        
    if (matrix[rows][int(cols/8)] & uint8(1 << int(cols%8)))
        Unset(rows, cols);
    else
        Set(rows, cols);
}

void bitmatrix::RandomizeAll()
{
    for (int i = 0; i < m_rows; i++)
        for (int j = 0; j < m_cells; j++)
        {
             matrix[i][j] = uint8(rand()%256);
        }
}

void bitmatrix::SetAll()
{
    for (int i = 0; i < m_rows; i++)
        for (int j = 0; j < m_cells; j++)
        {
             matrix[i][j] = uint8(255); // 11111111
        }
}

void bitmatrix::UnsetAll()
{
    for (int i = 0; i < m_rows; i++)
        for (int j = 0; j < m_cells; j++)
        {
             matrix[i][j] = uint8(0); // 00000000
        }
}

void bitmatrix::FlipAll()
{
    for (int i = 0; i < m_rows; i++)
        for (int j = 0; j < m_cells; j++)
        {
             matrix[i][j] = ~uint8(matrix[i][j]);
        }
}

void bitmatrix::SetCol(bitmatrix& bin_mat, uint32 cols)
{
    if (bin_mat.GetRows() < m_rows || cols >= m_cols)
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
    if (bin_mat.GetCols() < m_cols || rows >= m_rows)
        return;

    for (int j = 0; j < m_cells; j++) 
    {    
        matrix[rows][j] = bin_mat.matrix[rows][j];
    }
}

void bitmatrix::Import(bitmatrix& bin_mat)
{
    uint32 max_rows = (m_rows > bin_mat.GetRows() ? bin_mat.GetRows() : m_rows);
    uint32 max_cells  = (m_cells > bin_mat.GetCells() ? bin_mat.GetCells() : m_cells);
    uint8 mask = 0;
    
    if (bin_mat.GetCols() < m_cols)
    {
        for (int i = 0; i < (bin_mat.GetCols()%8); i++) 
            mask |= uint8(1 << i);
    }    
    
    for (int i = 0; i < max_rows; i++) 
    {
        for (int j = 0; j < max_cells; j++)
        {
            if (mask && (j == max_cells - 1))
            {   
                matrix[i][j] &= ~mask;
                matrix[i][j] |= bin_mat.matrix[i][j] & mask;                
            }           
            else
                matrix[i][j] = bin_mat.matrix[i][j];
        }        
    }
}

void bitmatrix::StringToRow(string& str, uint32 rows)
{
    if (rows >= m_rows || str.length() < m_cols)
        return;
        
    char* c_str = code.c_str();
    if (!c_str)
        return;
    
    for (int i = 0; c_str[i] != '\0'; i++)
    {
        if (i >= m_cols)
            break;
            
        if (c_str[i] == '0')
            Unset(rows, i);
        else // qualsiasi cifra che non sia 0 viene considerata 1
            Set(rows, i);
    }
}

void bitmatrix::StringToCol(string& str, uint32 cols)
{
    if (cols >= m_cols || str.length() < m_rows)
        return;
        
    char* c_str = code.c_str();
    if (!c_str)
        return;
    
    for (int i = 0; c_str[i] != '\0'; i++)
    {
        if (i >= m_rows)
            break;
            
        if (c_str[i] == '0')
            Unset(i, cols);
        else // qualsiasi cifra che non sia 0 viene considerata 1
            Set(i, cols);
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
            if (j != (m_cols-1))
                s +=",";
        }
        s+="\n";
    }
    return s;
}

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

bitmatrix::bitmatrix(const bitmatrix &bit_mat)
{
    for (int i = 0; i < m_rows; i++)	
        delete matrix[i];
    delete matrix;

    m_rows = bit_mat.m_rows;
    m_cols = bit_mat.m_cols;
    m_cells = bit_mat.m_cells;
    matrix = new uint8*[m_rows];
    for (int i = 0; i < m_rows; i++)	
       matrix[i] = new uint8[m_cells];

    Import(bit_mat);
}

bitmatrix::~bitmatrix()
{
    for (int i = 0; i < m_rows; i++)	
        delete matrix[i];
    delete matrix;
}

void bitmatrix::Randomize(uint32 rows, uint32 cols)
{
    if (rows >= m_rows || cols >= m_cols)
        return;
    
    if (rand()%2)
        Unset(rows, cols);
    else
        Set(rows, cols);    
}

bool bitmatrix::Get(uint32 rows, uint32 cols) const
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

void bitmatrix::RandomizeCol(uint32 cols)
{
    if (cols >= m_cols)
        return;

    for (int i = 0; i < m_rows; i++)
        Randomize(i, cols);
}

void bitmatrix::RandomizeRow(uint32 rows)
{
    if (rows >= m_rows)
        return;

    for (int i = 0; i < m_cells; i++)
        matrix[rows][i] = uint8(rand()%256);
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

void bitmatrix::Resize(uint32 rows, uint32 cols)
{

}

void bitmatrix::UnsetCol(uint32 cols)
{
    if (cols >= m_cols)
        return;

    for (int i = 0; i < m_rows; i++)
        Unset(i, cols);
}

void bitmatrix::SetCol(uint32 cols)
{
    if (cols >= m_cols)
        return;

    for (int i = 0; i < m_rows; i++)
        Set(i, cols);
}

void bitmatrix::SetCol(const bitmatrix& bin_mat, uint32 cols)
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

void bitmatrix::SetCol(const string& str, uint32 cols)
{
    if (cols >= m_cols)
        return;
        
    const char* c_str = str.c_str();
    if (!c_str)
        return;

    uint32 rows = 0;
    
    for (int i = 0; c_str[i] != '\0'; i++)
    {    
        if (c_str[i] == ',')
            continue;        

        if (rows >= m_rows)
            break;
            
        if (c_str[i] == '0')
            Unset(rows, cols);
        else // qualsiasi cifra che non sia 0 viene considerata 1
            Set(rows, cols);

        rows++;
    }
}

void bitmatrix::UnsetRow(uint32 rows)
{
    if (rows >= m_rows)
        return;

    for (int i = 0; i < m_cells; i++)
        matrix[rows][i] = uint8(0); // 00000000
}

void bitmatrix::SetRow(uint32 rows)
{
    if (rows >= m_rows)
        return;

    for (int i = 0; i < m_cells; i++)
        matrix[rows][i] = uint8(255); // 11111111
}

void bitmatrix::SetRow(const bitmatrix& bin_mat, uint32 rows)
{
    if (bin_mat.GetCols() < m_cols || rows >= m_rows)
        return;

    for (int j = 0; j < m_cells; j++) 
    {    
        matrix[rows][j] = bin_mat.matrix[rows][j];
    }
}

void bitmatrix::SetRow(const string& str, uint32 rows)
{
    if (rows >= m_rows)
        return;
        
    const char* c_str = str.c_str();
    if (!c_str)
        return;

    uint32 cols = 0;
    
    for (int i = 0; c_str[i] != '\0'; i++)
    {
        if (c_str[i] == ',')
            continue;        

        if (cols >= m_cols)
            break;        
            
        if (c_str[i] == '0')
            Unset(rows, cols);
        else // qualsiasi cifra che non sia 0 viene considerata 1
            Set(rows, cols);

        cols++;
    }
}

void bitmatrix::Import(const string& str)
{        
    const char* c_str = str.c_str();
    if (!c_str)
        return;

    int rows = 0;
    int cols = 0;
    
    for (int i = 0; c_str[i] != '\0'; i++)
    {
        if (c_str[i] == ',')
            continue;
     
        if (c_str[i] == '\n')
        {
            rows++;

            if (rows >= m_rows)
                break;

            cols = 0;
            continue;
        }
        
        if (cols >= m_cols)
            continue;       
            
        if (c_str[i] == '0')
            Unset(rows, cols);
        else // qualsiasi cifra che non sia 0 viene considerata 1
            Set(rows, cols);

        cols++;
    }
}

void bitmatrix::Import(const bitmatrix& bin_mat)
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

void bitmatrix::Print() const
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

string bitmatrix::ToString() const
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

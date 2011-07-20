#include "bitmatrix.h"

bitmatrix::bitmatrix(uint32 rows, uint32 cols)
{        
    m_rows = (rows ? rows : 1);
    m_cols = (cols ? cols : 1);
    m_cells = int(m_cols/8) + (int(m_cols%8) ? 1 : 0);
    matrix = new uint8*[m_rows];
    for (uint32 i = 0; i < m_rows; i++)	
       matrix[i] = new uint8[m_cells];
}

bitmatrix::bitmatrix(const bitmatrix &bit_mat)
{
    m_rows = bit_mat.m_rows;
    m_cols = bit_mat.m_cols;
    m_cells = bit_mat.m_cells;
    matrix = new uint8*[m_rows];
    for (uint32 i = 0; i < m_rows; i++)	
       matrix[i] = new uint8[m_cells];

    Import(bit_mat);
}

bitmatrix::~bitmatrix()
{
    for (uint32 i = 0; i < m_rows; i++)	
        delete matrix[i];
    delete matrix;
}

void bitmatrix::Randomize(uint32 row, uint32 col)
{
    if (row >= m_rows || col >= m_cols)
        return;
    
    if (rand()%2)
        Unset(row, col);
    else
        Set(row, col);    
}

bool bitmatrix::Get(uint32 row, uint32 col) const
{
    if (row >= m_rows || col >= m_cols)
        return false;
          
    return matrix[row][int(col/8)] & uint8(1 << int(col%8));
}

void bitmatrix::Set(uint32 row, uint32 col)
{
    if (row >= m_rows || col >= m_cols)
        return;
        
    matrix[row][int(col/8)] |= uint8(1 << int(col%8));
}

void bitmatrix::Unset(uint32 row, uint32 col)
{
    if (row >= m_rows || col >= m_cols)
        return;
        
    matrix[row][int(col/8)] &= ~uint8(1 << int(col%8));
}

void bitmatrix::Flip(uint32 row, uint32 col)
{
    if (row >= m_rows || col >= m_cols)
        return;
        
    matrix[row][int(col/8)] = ~matrix[row][int(col/8)];
}

void bitmatrix::RandomizeCol(uint32 col)
{
    if (col >= m_cols)
        return;

    for (uint32 i = 0; i < m_rows; i++)
        Randomize(i, col);
}

void bitmatrix::RandomizeRow(uint32 row)
{
    if (row >= m_rows)
        return;

    for (uint32 i = 0; i < m_cells; i++)
        matrix[row][i] = uint8(rand()%256);
}

void bitmatrix::RandomizeAll()
{
    for (uint32 i = 0; i < m_rows; i++)
        for (uint32 j = 0; j < m_cells; j++)
        {
             matrix[i][j] = uint8(rand()%256);
        }
}

void bitmatrix::SetAll()
{
    for (uint32 i = 0; i < m_rows; i++)
        for (uint32 j = 0; j < m_cells; j++)
        {
             matrix[i][j] = uint8(255); // 11111111
        }
}

void bitmatrix::UnsetAll()
{
    for (uint32 i = 0; i < m_rows; i++)
        for (uint32 j = 0; j < m_cells; j++)
        {
             matrix[i][j] = uint8(0); // 00000000
        }
}

void bitmatrix::FlipAll()
{
    for (uint32 i = 0; i < m_rows; i++)
        for (uint32 j = 0; j < m_cells; j++)
        {
             matrix[i][j] = ~uint8(matrix[i][j]);
        }
}

void bitmatrix::Resize(uint32 rows, uint32 cols)
{
    bitmatrix temp(*this);
    
     for (uint32 i = 0; i < m_rows; i++)	
        delete matrix[i];
    delete matrix;

    m_rows = (rows ? rows : 1);
    m_cols = (cols ? cols : 1);
    m_cells = int(m_cols/8) + (int(m_cols%8) ? 1 : 0);
    matrix = new uint8*[m_rows];
    for (uint32 i = 0; i < m_rows; i++)	
       matrix[i] = new uint8[m_cells];

   Import(temp);   
}

string bitmatrix::GetCol(uint32 col)
{
    string str;

    if (col >= m_cols)
        return str;
    
    for (uint32 i = 0; i < m_rows; i++)
    {
        str += (matrix[i][int(col/8)] & uint8(1 << int(col%8))) ? "1" : "0";
        if (i < (m_rows - 1))
            str +=",";
    }
    return str;
}

void bitmatrix::UnsetCol(uint32 col)
{
    if (col >= m_cols)
        return;

    for (uint32 i = 0; i < m_rows; i++)
        Unset(i, col);
}

void bitmatrix::SetCol(uint32 col)
{
    if (col >= m_cols)
        return;

    for (uint32 i = 0; i < m_rows; i++)
        Set(i, col);
}

void bitmatrix::SetCol(const bitmatrix& bin_mat, uint32 col)
{
    if (bin_mat.GetRows() < m_rows || col >= m_cols)
        return;

    for (uint32 i = 0; i < m_rows; i++) 
    {    
        uint8 mask = bin_mat.matrix[i][int(col/8)] & uint8(1 << int(col%8));
        if (mask)
            matrix[i][int(col/8)] |= uint8(1 << int(col%8));
        else 
            matrix[i][int(col/8)] &= ~uint8(1 << int(col%8));
    }
}

void bitmatrix::SetCol(const string& str, uint32 col)
{
    if (col >= m_cols)
        return;
        
    const char* c_str = str.c_str();
    if (!c_str)
        return;

    uint32 row = 0;
    
    for (uint32 i = 0; c_str[i] != '\0'; i++)
    {    
        if (c_str[i] == ',')
            continue;        

        if (row >= m_rows)
            break;
            
        if (c_str[i] == '0')
            Unset(row, col);
        else // qualsiasi cifra che non sia 0 viene considerata 1
            Set(row, col);

        row++;
    }
}

string bitmatrix::GetRow(uint32 row)
{
    string str;

    if (row >= m_cols)
        return str;

    for (uint32 i = 0; i < m_cols; i++)
    {
        str += (matrix[row][int(i/8)] & uint8(1 << int(i%8))) ? "1" : "0";
        if (i < (m_cols - 1))
            str +=",";
    }
    return str;
}

void bitmatrix::UnsetRow(uint32 row)
{
    if (row >= m_rows)
        return;

    for (uint32 i = 0; i < m_cells; i++)
        matrix[row][i] = uint8(0); // 00000000
}

void bitmatrix::SetRow(uint32 row)
{
    if (row >= m_rows)
        return;

    for (uint32 i = 0; i < m_cells; i++)
        matrix[row][i] = uint8(255); // 11111111
}

void bitmatrix::SetRow(const bitmatrix& bin_mat, uint32 row)
{
    if (bin_mat.GetCols() < m_cols || row >= m_rows)
        return;

    for (uint32 j = 0; j < m_cells; j++) 
    {    
        matrix[row][j] = bin_mat.matrix[row][j];
    }
}

void bitmatrix::SetRow(const string& str, uint32 row)
{
    if (row >= m_rows)
        return;
        
    const char* c_str = str.c_str();
    if (!c_str)
        return;

    uint32 col = 0;
    
    for (uint32 i = 0; c_str[i] != '\0'; i++)
    {
        if (c_str[i] == ',')
            continue;        

        if (col >= m_cols)
            break;        
            
        if (c_str[i] == '0')
            Unset(row, col);
        else // qualsiasi cifra che non sia 0 viene considerata 1
            Set(row, col);

        col++;
    }
}

void bitmatrix::Import(const string& str)
{        
    const char* c_str = str.c_str();
    if (!c_str)
        return;

    uint32 row = 0;
    uint32 col = 0;
    
    for (uint32 i = 0; c_str[i] != '\0'; i++)
    {
        if (c_str[i] == ',')
            continue;
     
        if (c_str[i] == '\n')
        {
            row++;

            if (row >= m_rows)
                break;

            col = 0;
            continue;
        }
        
        if (col >= m_cols)
            continue;       
            
        if (c_str[i] == '0')
            Unset(row, col);
        else // qualsiasi cifra che non sia 0 viene considerata 1
            Set(row, col);

        col++;
    }
}

void bitmatrix::Import(const bitmatrix& bin_mat)
{
    uint32 max_rows = (m_rows > bin_mat.GetRows() ? bin_mat.GetRows() : m_rows);
    uint32 max_cells  = (m_cells > bin_mat.GetCells() ? bin_mat.GetCells() : m_cells);
    uint8 mask = 0;
    
    if (bin_mat.GetCols() < m_cols)
    {
        for (uint32 i = 0; i < (bin_mat.GetCols()%8); i++) 
            mask |= uint8(1 << i);
    }    
    
    for (uint32 i = 0; i < max_rows; i++) 
    {
        for (uint32 j = 0; j < max_cells; j++)
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
    for (uint32 i = 0; i < m_rows; i++)
    {
        for (uint32 j = 0; j < m_cols; j++)
        {
             printf("%d ", (matrix[i][int(j/8)] & uint8(1 << int(j%8)) ? 1 : 0 ));
        }
        printf("\n");
    }
}

string bitmatrix::ToString() const
{
    string s;
    for (uint32 i = 0; i < m_rows; i++)
    {
        for (uint32 j = 0; j < m_cols; j++)
        {
            s += (matrix[i][int(j/8)] & uint8(1 << int(j%8))) ? "1" : "0";
            if (j != (m_cols-1))
                s +=",";
        }
        s+="\n";
    }
    return s;
}

const bitmatrix& bitmatrix::operator=(const bitmatrix& bit_mat) 
{
    for (uint32 i = 0; i < m_rows; i++)
        delete matrix[i];
    delete matrix;

    m_rows = bit_mat.m_rows;
    m_cols = bit_mat.m_cols;
    m_cells = bit_mat.m_cells;
    matrix = new uint8*[m_rows];
    for (uint32 i = 0; i < m_rows; i++)	
       matrix[i] = new uint8[m_cells];

    Import(bit_mat);

    return bit_mat;
}

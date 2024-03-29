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
    delete_matrix();
}

void bitmatrix::delete_matrix()
{
    for (uint32 i = 0; i < m_rows; i++)
        if (matrix[i])
            delete[] matrix[i];
    if (matrix)
        delete[] matrix;
}

void bitmatrix::Randomize(uint32 row, uint32 col)
{
    if (row >= m_rows || col >= m_cols)
        return;

    if (randb())
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

    if (matrix[row][int(col/8)] & uint8(1 << int(col%8)))
        Unset(row, col);
    else
        Set(row, col);
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
        matrix[row][i] = uint8(randmm(0,255));
}

void bitmatrix::RandomizeAll()
{
    for (uint32 i = 0; i < m_rows; i++)
        for (uint32 j = 0; j < m_cells; j++)
             matrix[i][j] = uint8(randmm(0,255));
}

void bitmatrix::SetAll()
{
    for (uint32 i = 0; i < m_rows; i++)
        for (uint32 j = 0; j < m_cells; j++)
             matrix[i][j] = uint8(255); // 11111111
}

void bitmatrix::UnsetAll()
{
    for (uint32 i = 0; i < m_rows; i++)
        for (uint32 j = 0; j < m_cells; j++)
             matrix[i][j] = uint8(0); // 00000000
}

void bitmatrix::FlipAll()
{
    for (uint32 i = 0; i < m_rows; i++)
        for (uint32 j = 0; j < m_cells; j++)
             matrix[i][j] = ~uint8(matrix[i][j]);
}

void bitmatrix::Resize(uint32 rows, uint32 cols)
{
    bitmatrix temp(*this);
    delete_matrix();

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
    string str = "";

    if (col >= m_cols)
        return str;

    for (uint32 i = 0; i < m_rows; i++)
        str += (matrix[i][int(col/8)] & uint8(1 << int(col%8))) ? "1" : "0";
    
    StrTranspose(str);
    return str;
}

string bitmatrix::GetCols(uint32 col_a, uint32 col_b)
{   
    string str = "";

    if (col_a > m_cols || col_b > m_cols || col_a > col_b)
        return str;

    for (uint32 j = col_a; j <= col_b; j++)
    {
        for (uint32 i = 0; i < m_rows; i++)
            str += (matrix[i][int(j/8)] & uint8(1 << int(j%8))) ? "1" : "0";
        
        str+="\n";
    }

    StrTranspose(str);
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
    if (bin_mat.GetRowNum() < m_rows || col >= m_cols)
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
        if (row >= m_rows)
            break;

        if(c_str[i] == '\n')
            continue;

        if (c_str[i] == '0')
            Unset(row, col);
        else 
            Set(row, col);

        row++;
    }
}

void bitmatrix::SetCols(const string& str, uint32 start_col)
{
    int row_size = GetStrRowSize(str);
    int col_size = GetStrColSize(str);

    if (start_col > m_cols)
        return;

    const char* c_str = str.c_str();
    if (!c_str)
        return;

    for (uint32 i = 0; i < col_size; i++)
    {
        for (uint32 j = 0; j < (row_size + 1); j++)
        {
            if (c_str[j + i * (row_size + 1)] == '0')
                Unset(i, start_col + j);
            else
                Set(i, start_col + j);
        }
    }
}

string bitmatrix::GetRow(uint32 row)
{
    string str = "";

    if (row >= m_cols)
        return str;

    for (uint32 i = 0; i < m_cols; i++)
        str += (matrix[row][int(i/8)] & uint8(1 << int(i%8))) ? "1" : "0";
    
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
    if (bin_mat.GetColNum() < m_cols || row >= m_rows)
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
        else
            Set(row, col);

        col++;
    }
}

void bitmatrix::Import(const bitmatrix& bin_mat)
{
    uint32 max_rows = (m_rows > bin_mat.GetRowNum() ? bin_mat.GetRowNum() : m_rows);
    uint32 max_cells  = (m_cells > bin_mat.GetCellSize() ? bin_mat.GetCellSize() : m_cells);
    uint8 mask = 0;

    if (bin_mat.GetColNum() < m_cols)
    {
        for (uint32 i = 0; i < (bin_mat.GetColNum()%8); i++)
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

void bitmatrix::AttachCols(const string& str)
{
    uint32 n_add_cols = GetStrRowSize(str);
    uint32 old_col_size = m_cols;
    const char *c_str = str.c_str();
    
    if (!c_str)
        return;

    //cout << "resize " << m_cols << "-" << m_cols + n_add_cols << endl;

    /* se non stiamo usando lo stesso numero di righe o colonne nulle termina */
    if(n_add_cols == 0 || GetStrColSize(str) != GetRowNum())
        return;

    /* ridimensiona la matrice aggiungendo lo spazio per le nuove colonne */
    Resize(m_rows, m_cols + n_add_cols);
    
    /* aggiunge le colonne alla matrice */
    SetCols(str, old_col_size);
}

void bitmatrix::DeleteCol(uint32 col)
{
    if (col >= m_cols)
        return;

    string chunk_a, chunk_b;
    
    if (col)
        chunk_a = GetCols(0, col - 1);
    if (col < m_cols)
    chunk_b = GetCols(col + 1, m_cols);

    Resize(m_rows, m_cols -1);

    SetCols(chunk_a, 0);
    SetCols(chunk_b, col);
}


uint32 GetStrColSize(const string& str)
{
    /* conta i \n */
    uint32 cols = 0;
    uint32 row_size = GetStrRowSize(str);

    for (uint32 i = row_size + 1 ; str[i] != '\0'; i += (row_size + 1))
        cols++;

    return cols + 1;
}

uint32 GetStrRowSize(const string& str)
{
    /* controlla solo quanto è alta la prima colonna */
    uint32 rows ;

    for (rows = 0; str[rows] != '\n'; rows++);

    return rows;
}

void StrTranspose(string& str)
{   
    uint32 rows = GetStrRowSize(str), cols = GetStrColSize(str);
    string temp_string;

    for (uint32 i = 0; i < rows ; i++)
    {
        for (uint32 j = 0; j < cols; j++)
            temp_string += str.c_str()[j * (rows + 1) + i];
        temp_string += "\n";
    }

    str = temp_string;
}

void bitmatrix::Print() const
{
    for (uint32 i = 0; i < m_rows; i++)
    {
        for (uint32 j = 0; j < m_cols; j++)
             printf("%d", (matrix[i][int(j/8)] & uint8(1 << int(j%8)) ? 1 : 0 ));
        printf("\n");
    }
}

string bitmatrix::ToString() const
{
    string s = "";
    for (uint32 i = 0; i < m_rows; i++)
    {
        for (uint32 j = 0; j < m_cols; j++)
            s += (matrix[i][int(j/8)] & uint8(1 << int(j%8))) ? "1" : "0";
        s+="\n";
    }
    return s;
}

const bitmatrix& bitmatrix::operator=(const bitmatrix& bit_mat)
{
    delete_matrix();

    m_rows = bit_mat.m_rows;
    m_cols = bit_mat.m_cols;
    m_cells = bit_mat.m_cells;
    matrix = new uint8*[m_rows];
    for (uint32 i = 0; i < m_rows; i++)
       matrix[i] = new uint8[m_cells];

    Import(bit_mat);

    return bit_mat;
}

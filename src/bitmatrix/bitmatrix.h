#ifndef BITMATRIX_H
#define BITMATRIX_H

#include <cstdio>
#include <cstdlib>
#include <string>

typedef unsigned char uint8;
typedef unsigned short int uint16;
typedef unsigned long int uint32;

using namespace std;

class bitmatrix
{    
    public:
        bitmatrix(uint32 rows, uint32 cols);
        bitmatrix(const bitmatrix &bit_mat);   
        ~bitmatrix();
        
        uint32 GetRows() const
        {
            return m_rows;        
        }  
        
        uint32 GetCols() const
        {
            return m_cols;        
        }
        
        uint32 GetCells() const
        {
            return m_cells;        
        }
   
        void    Randomize(uint32 rows, uint32 cols);        
        bool    Get(uint32 rows, uint32 cols) const;
        void    Set(uint32 rows, uint32 cols);        
        void    Unset(uint32 rows, uint32 cols);
        void    Flip(uint32 rows, uint32 cols);

        void    RandomizeCol(uint32 cols);
        void    RandomizeRow(uint32 rows);
        
        void    RandomizeAll();
        void    SetAll();        
        void    UnsetAll();
        void    FlipAll();
        void    Resize(uint32 rows, uint32 cols);
        
        void    SetCol(const bitmatrix& bin_mat, uint32 cols);
        void    SetCol(string& str, uint32 rows);
        void    SetRow(const bitmatrix& bin_mat, uint32 rows);        
        void    SetRow(string& str, uint32 cols);
        void    Import(const bitmatrix& bin_mat);        
        
        void    Print() const;
        string  ToString() const;
        
    private:
        uint8** matrix; 
        uint32  m_rows;
        uint32  m_cols;
        
        uint32  m_cells;
};

#endif

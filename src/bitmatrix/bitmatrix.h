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
        ~bitmatrix();
        
        uint32 GetRows()
        {
            return m_rows;        
        }  
        
        uint32 GetCols()
        {
            return m_cols;        
        }
        
        uint32 GetCells()
        {
            return m_cells;        
        }
   
        void    Randomize(uint32 rows, uint32 cols);        
        bool    Get(uint32 rows, uint32 cols);
        void    Set(uint32 rows, uint32 cols);        
        void    Unset(uint32 rows, uint32 cols);
        void    Flip(uint32 rows, uint32 cols);
        
        void    RandomizeAll();
        void    SetAll();        
        void    UnsetAll();
        void    FlipAll();
        
        void    SetCol(bitmatrix& bin_mat, uint32 cols);
        void    SetRow(bitmatrix& bin_mat, uint32 rows);
        void    Import(bitmatrix& bin_mat);
        
        void    Print();
        string  ToString();
        
    private:
        uint8** matrix; 
        uint32  m_rows;
        uint32  m_cols;
        
        uint32  m_cells;
};

#endif

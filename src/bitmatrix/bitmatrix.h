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
   
        void    Randomize(uint32 row, uint32 col);        
        bool    Get(uint32 row, uint32 col) const;
        void    Set(uint32 row, uint32 col);        
        void    Unset(uint32 row, uint32 col);
        void    Flip(uint32 row, uint32 col);          
       
        string  GetCol(uint32 col);
        void    UnsetCol(uint32 col);
        void    SetCol(uint32 col);
        void    SetCol(const bitmatrix& bin_mat, uint32 col);
        // Formato Stringa 0,1,0,1,0,0
        void    SetCol(const string& str, uint32 row);
        void    RandomizeCol(uint32 col);

        string  GetRow(uint32 row);
        void    UnsetRow(uint32 row);      
        void    SetRow(uint32 row);        
        void    SetRow(const bitmatrix& bin_mat, uint32 row);
        // Formato Stringa 0,1,0,1,0,0      
        void    SetRow(const string& str, uint32 col);
        void    RandomizeRow(uint32 row);

        void    RandomizeAll();
        void    SetAll();        
        void    UnsetAll();
        void    FlipAll();
        void    Resize(uint32 rows, uint32 cols);
        void    Import(const string& str);  
        void    Import(const bitmatrix& bin_mat);        
        void    Print() const;
        string  ToString() const;

        const bitmatrix& operator=(const bitmatrix& bit_mat);
        
    private:

        uint8** matrix; 
        uint32  m_rows;
        uint32  m_cols;        
        uint32  m_cells;
};

#endif

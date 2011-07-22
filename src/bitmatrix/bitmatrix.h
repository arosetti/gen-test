#ifndef BITMATRIX_H
#define BITMATRIX_H

#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <string>

typedef unsigned char uint8;
typedef unsigned short int uint16;
typedef unsigned long int uint32;

using namespace std;


//! La classe bitmatrix è una struttura di memorizzazione di una matrice di bit
/**
 *
 */

class bitmatrix
{    
    public:

        //! Construttore - Setta il numero di righe e colonne iniziali della bitmatrix        
        /**
        * @param uint32 rows Numero di righe
        * @param uint32 cols Numero di colonne
        */
        bitmatrix(uint32 rows, uint32 cols);
        //! Construttore di copia     
        /**
        * @param const bitmatrix &bit_mat Bitmatrix da cui copiare la matrice
        */
        bitmatrix(const bitmatrix &bit_mat);
        //! Distruttore - dealloca la matrice   
        /**
        */   
        ~bitmatrix();
        
        //! Richiedi il numero di righe 
        /**
        * @return uint32 Numero di righe della bitmatrix
        */  
        uint32 GetRows() const
        {
            return m_rows;        
        }  
        
        //! Richiedi il numero di colonne
        /**
        * @return uint32 Numero di colonne della bitmatrix
        */  
        uint32 GetCols() const
        {
            return m_cols;        
        }
        
        //! Richiedi la dimensione
        /**
        * @return uint32 Numero di righe * Numero di colonne
        */  
        uint32 GetSize() const
        {
            return m_cols*m_rows;
        }

        uint32 GetCells() const
        {
            return m_cells;        
        }
   
        //! Randomizza un bit
        /**
        * @param uint32 row Numero della riga (partendo da 0)
        * @param uint32 col Numero della colonna (partendo da 0)
        */ 
        void    Randomize(uint32 row, uint32 col);

        //! Ottieni un bit
        /**
        * @param uint32 row Numero della riga (partendo da 0)
        * @param uint32 col Numero della colonna (partendo da 0)
        * @return bool Il bit ritorna come TRUE se è settato a 1 o FALSE se è a 0
        */         
        bool    Get(uint32 row, uint32 col) const;

        //! Setta un bit cioè lo mette ad 1
        /**
        * @param uint32 row Numero della riga (partendo da 0)
        * @param uint32 col Numero della colonna (partendo da 0)
        */  
        void    Set(uint32 row, uint32 col);

        //! Resetta un bit cioè lo mette ad 0
        /**
        * @param uint32 row Numero della riga (partendo da 0)
        * @param uint32 col Numero della colonna (partendo da 0)
        */       
        void    Unset(uint32 row, uint32 col);

        //! Flip un bit cioè lo inverte di valore
        /**
        * @param uint32 row Numero della riga (partendo da 0)
        * @param uint32 col Numero della colonna (partendo da 0)
        */   
        void    Flip(uint32 row, uint32 col);    
      
       
        //! Ritorna una colonna come stringa
        /**
        * @param uint32 col Numero della colonna (partendo da 0)
        * @return string La colonna convertita in stringa
        */  
        string  GetCol(uint32 col);

        //! Resetta una colonna cioè la mette tutta a 0
        /**
        * @param uint32 col Numero della colonna (partendo da 0)
        */  
        void    UnsetCol(uint32 col);

        //! Setta una colonna cioè la mette tutta a 1
        /**
        * @param uint32 col Numero della colonna (partendo da 0)
        */  
        void    SetCol(uint32 col);

        //! Copia una colonna da una bitmatrix alla bitmatrix locale (non serve che siano delle stesse dimensioni)
        /**
        * @param uint32 col Numero della colonna (partendo da 0)
        */ 
        void    SetCol(const bitmatrix& bin_mat, uint32 col);

        //! Copia una colonna da una stringa alla bitmatrix
        /**
        * @param uint32 str Stringa da copiare nel formato 0,0,1,0,1
        * @param uint32 col Numero della colonna (partendo da 0)
        */ 
        void    SetCol(const string& str, uint32 col);
        //! Randomizza una colonna della bitmatrix
        /**
        * @param uint32 col Numero della colonna (partendo da 0)
        */ 
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

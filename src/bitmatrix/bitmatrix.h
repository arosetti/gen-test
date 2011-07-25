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


//! Ritorna il numero di colonne di una stringa
/**
* @param const string& str stringa da cui leggere il numero di colonne
* @return uint32 il numero di colonne della stringa
*/
uint32  GetStrColSize(const string& str);

//! Ritorna il numero di righe di una stringa
/**
* @param const string& str stringa da cui leggere il numero di righe
* @return uint32 il numero di righe della stringa
*/
uint32  GetStrRowSize(const string& str);

//! Struttura di memorizzazione di una matrice di bit
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
        uint32 GetRowSize() const
        {
            return m_rows;
        }

        //! Richiedi il numero di colonne
        /**
        * @return uint32 Numero di colonne della bitmatrix
        */
        uint32 GetColSize() const
        {
            return m_cols;
        }

        //! Richiedi il numero di celle
        /**
        * @return uint32 Numero di celle
        */
        uint32 GetCellSize() const
        {
            return m_cells;
        }

        //! Richiedi la dimensione
        /**
        * @return uint32 Numero di righe * Numero di colonne
        */
        uint32 GetSize() const
        {
            return m_cols*m_rows;
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
    
        //! Ritorna una sottomatrice ridotta alle colonne scelte
        /**
        * @param uint32 col_a Numero della colonna a (partendo da 0)
        * @param uint32 col_b Numero della colonna b (partendo da 0)
        * @return string La colonna convertita in stringa

        */
        string  GetCols(uint32 col_a, uint32 col_b);

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
        * @param const bitmatrix& bin_mat Bitmatrix da cui copiare la colonna
        * @param uint32 col Numero della colonna (partendo da 0)
        */
        void    SetCol(const bitmatrix& bin_mat, uint32 col);

        //! Copia una stringa in una colonna della bitmatrix
        /**
        * @param uint32 str Stringa da copiare nel formato 0,0,1,0,1
        * @param uint32 col Numero della colonna (partendo da 0)
        */
        void    SetCol(const string& str, uint32 col);

        //! Copia una stringa in una serie di colonne della bitmatrix
        /**
        * @param uint32 str Stringa da copiare nel formato 0,0,1,0,1
        * @param uint32 col_a Numero della colonna a (partendo da 0)
        * @param uint32 col_b Numero della colonna b (partendo da 0)
        */
        void    SetCols(const string& str, uint32 col_a, uint32 col_b);

        //! Randomizza una colonna della bitmatrix
        /**
        * @param uint32 col Numero della colonna (partendo da 0)
        */
        void    RandomizeCol(uint32 col);

        //! Ritorna una riga come stringa
        /**
        * @param uint32 row Numero della riga (partendo da 0)
        * @return string La riga convertita in stringa
        */
        string  GetRow(uint32 row);

        //! Resetta una riga cioè la mette tutta a 0
        /**
        * @param uint32 row Numero della riga (partendo da 0)
        */
        void    UnsetRow(uint32 row);

        //! Setta una riga cioè la mette tutta a 1
        /**
        * @param uint32 row Numero della riga (partendo da 0)
        */
        void    SetRow(uint32 row);

        //! Copia una riga da una bitmatrix alla bitmatrix locale (non serve che siano delle stesse dimensioni)
        /**
        * @param const bitmatrix& bin_mat Bitmatrix da cui copiare la riga
        * @param uint32 row Numero della riga (partendo da 0)
        */
        void    SetRow(const bitmatrix& bin_mat, uint32 row);

        //! Copia una stringa in una riga della bitmatrix
        /**
        * @param uint32 str Stringa da copiare nel formato 0,0,1,0,1
        * @param uint32 row Numero della riga (partendo da 0)
        */ 
        void    SetRow(const string& str, uint32 row);

        //! Randomizza una riga della bitmatrix
        /**
        * @param uint32 col Numero della riga (partendo da 0)
        */
        void    RandomizeRow(uint32 row);

        //! Randomizza l'intera bitmatrix
        void    RandomizeAll();
        //! Setta l'intera bitmatrix (la mette a 1)
        void    SetAll();
        //! Resetta l'intera bitmatrix (la mette a 0)
        void    UnsetAll();
        //! Flip l'intera bitmatrix (inverte il valore dei bit)
        void    FlipAll();

        //! Ridimensiona la bitmatrix
        /**
        * @param uint32 rows Numero delle righe
        * @param uint32 cols Numero delle colonne
        */
        void    Resize(uint32 rows, uint32 cols);

        //! Importa i valori da una stringa nella bitmatrix
        /**
        * @param const string& str Stringa da cui importare, formato 0,0,0\n1,0,0,1\n0
        */
        void    Import(const string& str);

        //! Importa una bitmatrix dentro la bitmatrix locale manenendo la sua dimensione attuale
        /**
        * @param const bitmatrix& bin_mat Bitmatrix da cui importare
        */
        void    Import(const bitmatrix& bin_mat);

        //! Unisce le colonne specificate nella bitmatrix locale aumentandone la dimensione
        /**
        * @param const string& str stringa da cui importare le colonne
        */
        void    AttachCols(const string& str);

        //! Stampa la bitmatrix a video
        void    Print() const;
        //! Converte la bitmatrix in una stringa uguale a quella del formato di importazione
        string  ToString() const;

        //! Copia una bitmatrix dentro la bitmatrix locale modificandoli le dimensioni
        /**
        * @param const bitmatrix& bin_mat Bitmatrix da cui copiare
        */
        const bitmatrix& operator=(const bitmatrix& bit_mat);

    private:

        /*! matrice di uint8 */
        uint8** matrix;
        /*! numero di righe della matrice */
        uint32  m_rows;
        /*! il numero di colonne della matrice */
        uint32  m_cols;
        /*! il numero di celle della matrice */
        uint32  m_cells;
};

#endif

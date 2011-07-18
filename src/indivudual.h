#include <bitset>
#include <vector>
#include <boost>

#include "defines.h"

using namespace std;
using namespace boost;

#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

class individual
{
    int gene_len;
    int chrom_len;

    vector<bool> chromosome;  // in attesa della struttura di dati adatta
    float fitness;

    public:
    individual(int,int);
    ~individual();
    individual(const individual&);

    void  set_gene(int,int,bool*);
    bool* get_gene(int,int);
    int   get_gene_len(int,int);
    
    float get_fitness();
    
    void  mutate();         // muta gene/cromosoma??
    void  random_gene();    // gene random 
}

#endif

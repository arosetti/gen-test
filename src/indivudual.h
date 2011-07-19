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
    unsigned float fitness;

    public:
    individual(int,int);
    ~individual();
    individual(const individual&);

    void  set_gene(int,int,bool*);
    bool* get_gene(int,int);
    
    int   get_gene_len(int,int);
    int   set_gene_len(int);

    void  set_random_chromosome();
    string  get_chromosome();
    void  set_chromosome();
    
    int   get_chromosome_len();
    void  set_chromosome_len();

    float get_fitness();
    void  update_fitness();
    void  new_results();    // nuovi dati per il calcolo delle fitness
    
    void  mutate();         // muta gene/cromosoma??
    
    bool operator < (const population&);  // dovrebbe far funzionare il sort() della std su liste di classi
}

#endif

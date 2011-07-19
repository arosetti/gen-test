#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include "utility.h"
#include "bitmatrix/bitmatrix.h"

using namespace std;

class individual
{
    uint32    gene_l;
    uint32    chromosome_l;

    bitmatrix chromosome;
    ufloat    fitness;

    public:
    individual(uint32,uint32);
    ~individual();
    individual(const individual&);
    
    string  get_chromosome();      
    void    set_chromosome(string);
    uint32  get_chromosome_length();
    void    set_chromosome_length(uint32);    
    void    chromosome_random();
    void    chromosome_mutate();
    
    ufloat  get_fitness();
    void    set_fitness(ufloat);
              
    string  get_gene(uint32, uint32);        // si potrebbe usare uint16 
    void    set_gene(uint32, string);
    uint32  get_gene_length();
    uint32  set_gene_length(uint32);
    void    gene_mutate();
    void    gene_random();
    
    bool operator < (const population&);    // necessario per std::sort() std::list<individual>
};

#endif

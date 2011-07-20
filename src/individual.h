#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include "common/utility.h"
#include "bitmatrix/bitmatrix.h"

using namespace std;

class individual
{
    uint32    gene_l;
    uint32    chromosome_l;

    bitmatrix *chromosome;
    float    fitness;

    public:
    individual(uint32,uint32);
    ~individual();
    individual(const individual&);

    string  get_chromosome() const;
    void    set_chromosome(string);
    uint32  get_chromosome_length() const;
    void    set_chromosome_length(uint32);
    void    chromosome_random();
    void    chromosome_mutate(uint32);

    float   get_fitness() const;
    void    set_fitness(float);

    string  get_gene(uint32) const;
    void    set_gene(uint32, string);
    uint32  get_gene_length() const;
    void    set_gene_length(uint32);
    void    gene_mutate(uint32,uint32);
    void    gene_random(uint32);

    bool operator < (const individual&);    // necessario per std::sort() std::list<individual>
};

#endif

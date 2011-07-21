#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include "../common/utility.h"
#include "../bitmatrix/bitmatrix.h"

using namespace std;

class individual
{
    uint32    chromosome_length;
    uint32    dna_length;

    bitmatrix *dna;
    float    fitness;

    public:
    individual(uint32,uint32);
    ~individual();
    individual(const individual&);

    string  get_dna() const;
    void    set_dna(string);
    uint32  get_dna_length() const;
    void    set_dna_length(uint32);
    void    dna_random();
    void    dna_mutate(uint32);

    float   get_fitness() const;
    void    set_fitness(float);

    string  get_chromosome(uint32) const;
    void    set_chromosome(uint32, string);
    uint32  get_chromosome_length() const;
    void    set_chromosome_length(uint32);
    void    chromosome_mutate(uint32,uint32);
    void    chromosome_random(uint32);

    bool operator < (const individual&);    // necessario per std::sort() std::list<individual>
};

#endif

#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include "../common/utility.h"
#include "../bitmatrix/bitmatrix.h"
#include "../simulation/tests.h"
#include "../config.h"

using namespace std;

extern config *conf;

class individual
{
    uint32    chromosome_length;
    uint32    chromosome_number;

    bitmatrix *dna;
    float    fitness;

    tests test;

    public:
    individual(uint32,uint32);
    ~individual();
    individual(const individual&);

    string  get_dna() const;
    void    set_dna(string);
    uint32  get_dna_length() const;
    void    dna_random();
    void    dna_mutate();

    float   get_fitness() const;
    void    set_fitness(float);

    string  get_chromosome(uint32) const;
    void    set_chromosome(uint32, string);

    uint32  get_chromosome_length() const;
    void    set_chromosome_length(uint32);
    uint32  get_chromosome_number() const;
    void    set_chromosome_number(uint32);
    void    chromosome_mutate(uint32,uint32);
    void    chromosome_random(uint32);

    bool operator < (const individual&);    // necessario per std::sort() std::list<individual>
};

#endif

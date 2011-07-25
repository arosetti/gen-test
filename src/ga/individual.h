#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <sstream>

#include "../common/utility.h"
#include "../bitmatrix/bitmatrix.h"
#include "../simulation/tests.h"
#include "../config.h"

using namespace std;

extern config *conf;

//! Il generico individuo della popolazione
/**
 *
 */

class individual
{
    uint32    chromosome_length;
    uint32    chromosome_number;

    bitmatrix *dna;
    float    fitness;

    tests test;

    public:
    individual(uint32,uint32);
    individual(const individual&);
    ~individual();

    string  get_dna() const;
    void    set_dna(string);
    uint32  get_dna_length() const;
    void    dna_random();
    void    dna_mutate();
    void    dna_split(uint32, string&, string&);
    void    dna_merge(string&, string&);

    float   get_fitness() const;
    void    set_fitness(float);
    void    calc_fitness();

    string  get_chromosome(uint32) const;
    void    set_chromosome(uint32, string);
    uint32  get_chromosome_length() const;
    void    set_chromosome_length(uint32);
    uint32  get_chromosome_number() const;
    void    set_chromosome_number(uint32);   // not used
    void    chromosome_mutate(uint32,uint32);
    void    chromosome_random(uint32);

    string  info();

    bool operator < (const individual&);
};

#endif

#include <bitset>
#include <vector>
#include <boost>

using namespace std;
using namespace boost;

#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

class individual
{
    int gene_len;
    int chrom_len;

    vector<bitset> chromosome;
    float fitness;

    public:
    individual();
    individual(const individual&);
    ~individual();


    float calc_fitness();
    void mutate();      // muta gene
    void init_gene();   // gene random 
}

#endif

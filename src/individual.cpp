#include "individual.h"

individual::individual(uint32 g_len, uint32 c_len)
{
    
    gene_l = c_len;
    chromosome_l = g_len;
    fitness = 0;
    
    chromosome.RandomizeAll();
}

individual::~individual()
{

}

/***chromosome***/

string individual::get_chromosome()
{
    return chromosome.to_string();
}

void individual::set_chromosome(string c)
{
    chromosome.set(c);
}

uint32 individual::get_chromosome_length()
{
    return chromosome_l;
}

void individual::set_chromosome_length(uint32 l)
{
    chromosome_l = l; /* extend-reduce matrix*/
}

void individual::chromosome_random()
{
    chromosome.RandomizeAll();
}

void individual::chromosome_mutate()
{
    
}
  
ufloat individual::get_fitness()
{
    return fitness;
}
  
void individual::set_fitness(ufloat f)
{
    fitness = f;
}

/***gene***/

bool* individual::get_gene(uint32 i)
{
    return chromosome[i];
}

void individual::set_gene(uint32 i, string s)
{
    chromosome[i] = s;
}

uint32  individual::get_gene_length()
{
    return gene_l;
}

void  individual::set_gene_length(uint32 l)
{
    gene_len = l;  /* extend-reduce matrix */
}


void individual::gene_mutate(uint32 g)
{
    if ( g<0 || g>= gene_l )
        return;
        
    chromosome[g].flip(rand()%gene_l + 1);
}

void individual::gene_random(uint32 g)
{
    if ( g<0 || g>= gene_l )
        return;
    
    chromosome[g].random();
}

/***internal_stuff***/

bool individual::operator < (const individual& p)
{
    return fitness < p.get_fitness();
}

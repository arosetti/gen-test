#include "individual.h"

individual::individual(int g_len, int c_len)
{
    
    gene_len = c_len;
    chrom_len = g_len;
    fitness = 0;
    
    random_init();
}

individual::~individual()
{

}

void individual::set_gene(int i,int j,bool* b)
{

}

bool* individual::get_gene(int i,int j)
{
    return chromosomes?chromosomes[i][j]:0;
}

int  individual::get_gene_len(int,int)
{
    return gene_len;
}

void individual::mutate()
{

}

void individual::random_gene()
{

}

void  individual::set_random_chromosome()
{

}

string  individual::get_chromosome()
{

}

void  individual::set_chromosome()
{

}


int   individual::get_chromosome_len()
{

}

void  individual::set_chromosome_len()
{

}

unsigned float get_fitness()
{

}

bool individual::operator < (const population& p)
{
    return fitness < p.get_fitness();
}

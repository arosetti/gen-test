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

float get_fitness()
{

}

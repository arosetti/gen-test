#include "individual.h"

individual::individual(uint32 g_len, uint32 c_len)
{
    gene_l = c_len;
    chromosome_l = g_len;
    fitness = 0;
    
    chromosome = new bitmatrix(chromosome_l, gene_l);
}

individual::~individual()
{
    delete chromosome;
}

string individual::get_chromosome() const
{
    return chromosome->ToString();
}

void individual::set_chromosome(string str)
{
    chromosome->Import(str); 
}

uint32 individual::get_chromosome_length()  const
{
    return chromosome_l;
}

void individual::set_chromosome_length(uint32 l)
{
    chromosome_l = l;
    chromosome->Resize(chromosome->GetRows(), chromosome_l);
}

void individual::chromosome_random()
{
    chromosome->RandomizeAll();
}

void individual::chromosome_mutate(uint32 mutation_strength)
{
    uint8 count = mutation_strength>gene_l?gene_l:mutation_strength;
    uint32 row_r,col_r;

    row_r = rand()%chromosome_l + 1;
    col_r = rand()%gene_l + 1;

    while (count-- != 0)
        chromosome->Flip(row_r,col_r);
}
  
float individual::get_fitness()  const
{
    return fitness;
}
  
void individual::set_fitness(float f)
{
    fitness = f;
}

string individual::get_gene(uint32 g)  const
{
    if (g>= chromosome_l)
        return "";
        
    return chromosome->GetRow(g);
}

void individual::set_gene(uint32 g, string s)
{
    if (g>= chromosome_l)
        return;
        
    chromosome->SetRow(s,g);
}

uint32  individual::get_gene_length()  const
{
    return gene_l;
}

void  individual::set_gene_length(uint32 l)
{
    gene_l = l;
    chromosome->Resize(gene_l, chromosome->GetCols());
}

void individual::gene_mutate(uint32 g, uint32 mutation_strength)
{
    uint8 count = mutation_strength>gene_l?gene_l:mutation_strength;
    uint32 col_r;

    if (g<0 || g>= gene_l)
        return;

    col_r = rand()%gene_l + 1;

    while (count-- != 0)
        chromosome->Flip(g,col_r);
}

void individual::gene_random(uint32 g)
{
    if (g>= gene_l)
        return;

    chromosome->RandomizeRow(g);
}

bool individual::operator < (const individual& i)
{
    return fitness < i.get_fitness();
}

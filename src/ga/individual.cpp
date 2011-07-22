#include "individual.h"

individual::individual(uint32 crom_len, uint32 dna_len)
{
    chromosome_length = crom_len;
    chromosome_number = dna_len;
    fitness = 0;
    
    dna = new bitmatrix(chromosome_number, chromosome_length);
}

individual::~individual()
{
    delete dna;
}

string individual::get_dna() const
{
    return dna->ToString();
}

void individual::set_dna(string str)
{
    dna->Import(str); 
}

uint32 individual::get_dna_length()  const
{
    return chromosome_number * chromosome_length;
}

uint32 individual::get_chromosome_number()  const
{
    return chromosome_number;
}

void individual::set_chromosome_number(uint32 l)
{
    chromosome_number = l;
    dna->Resize(dna->GetRows(), chromosome_number);
}

void individual::dna_random()
{
    dna->RandomizeAll();
}

void individual::dna_mutate()
{
    uint8 count = conf->mutation_strength>chromosome_length? \
                  chromosome_length:conf->mutation_strength;
    uint32 row_r,col_r;

    while (count-- != 0)
    {
        row_r = rand()%chromosome_number + 1;
        col_r = rand()%chromosome_length + 1;
        
        //cout "mutation at "<< row_r << "-" <<col_r << endl;
        
        dna->Flip(row_r,col_r);
    }
}
  
float individual::get_fitness()  const
{
    return fitness;
}
  
void individual::set_fitness(float f)
{
    fitness = f;
}

string individual::get_chromosome(uint32 crom)  const
{
    if (crom >= chromosome_number)
        return "";
        
    return dna->GetRow(crom);
}

void individual::set_chromosome(uint32 crom, string str)
{
    if (crom>= chromosome_number)
        return;
        
    dna->SetRow(str, crom);
}

uint32  individual::get_chromosome_length()  const
{
    return chromosome_length;
}

void  individual::set_chromosome_length(uint32 len)
{
    chromosome_length = len;
    dna->Resize(chromosome_length, dna->GetCols());
}

void individual::chromosome_mutate(uint32 crom, uint32 mutation_strength)
{
    uint8 count = mutation_strength>chromosome_length?chromosome_length:mutation_strength;
    uint32 col_r;

    if (crom == 0 || crom >= chromosome_length)
        return;

    col_r = rand()%chromosome_length + 1;

    while (count-- != 0)
        dna->Flip(crom, col_r);
}

void individual::chromosome_random(uint32 crom)
{
    if (crom >= chromosome_length)
        return;

    dna->RandomizeRow(crom);
}

bool individual::operator < (const individual& ind)
{
    return fitness < ind.get_fitness();
}

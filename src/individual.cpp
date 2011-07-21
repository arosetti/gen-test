#include "individual.h"

individual::individual(uint32 crom_len, uint32 dna_len)
{
    chromosome_length = crom_len;
    dna_length = dna_len;
    fitness = 0;
    
    dna = new bitmatrix(dna_length, chromosome_length);
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
    return dna_length;
}

void individual::set_dna_length(uint32 l)
{
    dna_length = l;
    dna->Resize(dna->GetRows(), dna_length);
}

void individual::dna_random()
{
    dna->RandomizeAll();
}

void individual::dna_mutate(uint32 mutation_strength)
{
    uint8 count = mutation_strength>chromosome_length?chromosome_length:mutation_strength;
    uint32 row_r,col_r;



    while (count-- != 0)
    {
        row_r = rand()%dna_length + 1;
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
    if (crom >= dna_length)
        return "";
        
    return dna->GetRow(crom);
}

void individual::set_chromosome(uint32 crom, string str)
{
    if (crom>= dna_length)
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

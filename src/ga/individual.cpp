#include "individual.h"

individual::individual(uint32 chrom_len, uint32 chrom_num)
{
    chromosome_length = chrom_len;
    chromosome_number = chrom_num;
    fitness = 10;

    dna = new bitmatrix(chromosome_number, chromosome_length);
}

individual::individual(const individual &ind)
{
    chromosome_length = ind.chromosome_length;
    chromosome_number = ind.chromosome_number;
    fitness = ind.fitness;
    dna = new bitmatrix(*ind.dna); // con dna = ind.dna crash ??
    test = ind.test;
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
    dna->Resize(dna->GetRowSize(), chromosome_number);
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

        dna->Flip(row_r,col_r);
    }
}

void individual::dna_split(uint32 pos, string &dna_1, string &dna_2)
{
    if (pos == 0 || pos == dna->GetColSize())
        return;

    dna_1 = dna->GetCols(0, pos);
    dna_2 = dna->GetCols(pos + 1, dna->GetColSize());

    if (conf->debug && conf->print_mating)
    {
        cout << "dna_1 (0, " << pos << ")" << endl << dna_1 << endl;
        cout << "dna_2 (" << pos+1 << ", " << 
             dna->GetColSize() << ")" << endl << dna_2 << endl;
    }
}

void individual::dna_merge(string& dna_1, string& dna_2)
{
    string new_dna;

    /* controlla che le righe siano le stesse in tutti i componenti non nulle */
    if (dna_1.length() == 0 || dna_2.length() == 0 ||
        GetStrRowSize(dna_1) != dna->GetRowSize() ||
        GetStrRowSize(dna_2) != dna->GetRowSize())
        return;

    /* ridimensiona il dna alla somma delle colonne di dna_1 */
    dna->Resize(dna->GetRowSize(), GetStrColSize(dna_1) + 1);

    /* copio dna_1 nel dna */
    dna->Import(dna_1);
    dna->SetCols(dna_1, 0, GetStrColSize(dna_1));

    /* attacco il secondo pezzo di dna */
    dna->AttachCols(dna_2);

    if (conf->debug && conf->print_mating)
    {
        dna->Print();
        cout << endl;
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

void individual::calc_fitness()
{

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
    dna->Resize(chromosome_length, dna->GetColSize());
}

void individual::chromosome_mutate(uint32 crom, uint32 mutation_strength)
{
    uint8 count = mutation_strength>chromosome_length? \
                  chromosome_length:mutation_strength;
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

string individual::info()
{
    stringstream out;

    out << "ptr : " << this << endl;
    out << "fitness:     " << fitness << endl;
    out << "dna size (row,col) : (" <<
           dna->GetRowSize() << "," << dna->GetColSize() << ")" << endl;
    out << "dna: " << endl << dna->ToString() << endl;

    return out.str();
}

bool individual::operator < (const individual& ind)
{
    return fitness < ind.get_fitness();
}

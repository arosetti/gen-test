#include "individual.h"

individual::individual(uint32 chrom_len, uint32 chrom_num)
{
    chromosome_length = chrom_len;
    chromosome_number = chrom_num;
    fitness = 0;
    dna = new bitmatrix(chromosome_number, chromosome_length);
    tested = false;
}

individual::individual(const individual &ind) : tests (ind)
{
    chromosome_length = ind.chromosome_length;
    chromosome_number = ind.chromosome_number;
    fitness = ind.fitness;
    dna = new bitmatrix(*ind.dna);
    tested = ind.tested;
}

individual::~individual()
{
    delete dna;
}

bool individual::is_tested() const
{
    return tested;
}

void individual::test()
{
    tested = true;
}

void individual::untest()
{
    tested = false;
}

string individual::get_dna() const
{
    return dna->ToString();
}

void individual::set_dna(string str)
{
    dna->Import(str);
    untest();
}

uint32 individual::get_dna_length()  const
{
    return chromosome_number * get_chromosome_length();
}

uint32 individual::get_chromosome_number()  const
{
    return chromosome_number;
}

void individual::set_chromosome_number(uint32 l)
{
    chromosome_number = l;
    dna->Resize(dna->GetRowNum(), chromosome_number);
    untest();
}

void individual::dna_random()
{
    dna->RandomizeAll();
    untest();
}

void individual::dna_mutate(float mutation_rate)
{
    uint32 row_r = randmm(0, chromosome_number),
           col_r = randmm(0, get_chromosome_length());
    float count = mutation_rate * get_dna_length();

    if (conf->get_bool_config(CONFIG_LOG) && conf->get_bool_config(CONFIG_LOG_MUTATIONS))
    {
        //LOG in generation{x}-mutations.log ... in individual non sappiamo la generazione attuale...
        cout << count << " mutation event(s)!"<<endl;
    }

    while (count >= 1.0f)
    {
        dna->Flip(row_r,col_r);
        row_r = randmm(0, chromosome_number);
        col_r = randmm(0, get_chromosome_length());
        count--;
    }
    if (uint32(count * 1000) > randmm(0,1000))
        dna->Flip(row_r,col_r);

    untest();
}

void individual::dna_shrink()
{
    dna->DeleteCol(randmm(0, get_chromosome_length()));
}

void individual::dna_split(uint32 pos, string &dna_1, string &dna_2)
{
    if (pos > dna->GetColNum())
        return;

    dna_1 = dna->GetCols(0, pos);
    dna_2 = dna->GetCols(pos + 1, dna->GetColNum() - 1);

    if (conf->get_bool_config(CONFIG_LOG) && conf->get_bool_config(CONFIG_LOG_MATING))
    {
        cout << "dna_1 (0, " << pos << ")" << endl << dna_1 << endl;
        cout << "dna_2 (" << pos+1 << ", " << 
             dna->GetColNum() << ")" << endl << dna_2 << endl;
    }
}

void individual::dna_merge(string& dna_1, string& dna_2)
{
    string new_dna;

    // controlla che le righe siano le stesse in tutti i componenti non nulle
    if (dna_1.length() == 0 || dna_2.length() == 0 ||
        GetStrColSize(dna_1) != dna->GetRowNum() ||
        GetStrColSize(dna_2) != dna->GetRowNum())
        return;
    if (conf->get_bool_config(CONFIG_LOG) && conf->get_bool_config(CONFIG_LOG_MATING))
        cout << "# merging DNA" << endl; 

    // ridimensiona il dna alla somma delle colonne di dna_1
    set_chromosome_length(GetStrRowSize(dna_1));

    // copio dna_1 nel dna */
    dna->SetCols(dna_1, 0);

    // attacco il secondo pezzo di dna
    dna->AttachCols(dna_2);

    if (conf->get_bool_config(CONFIG_LOG) && conf->get_bool_config(CONFIG_LOG_MATING))
    {
        dna->Print();
        cout << endl;
    }

    untest();
}

float individual::get_fitness()  const
{
    return fitness;
}

void individual::set_fitness(float f) // deprecated
{
    fitness = f;
}

void individual::calc_fitness()
{
    if (conf->get_int_config(CONFIG_FITNESS_TYPE) == 1)
        fitness = (float)(100 * detected) / (float)(n_tests);
    else if (conf->get_int_config(CONFIG_FITNESS_TYPE)  == 2)
        fitness = ((float)(detected) / (float)(n_tests)) -
                 ((float)get_chromosome_length() / (10.0f * (float)(n_tests))) + 1.0f;
    else
    {
        cout << "please, select a valid fitness type" << endl;
        exit(1);
    }
    
}

float individual::get_fault_coverage() const
{
    if (n_tests)
        return (float)(detected) / (float)(n_tests);
    else
        return 0;
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
    untest();
}

uint32  individual::get_chromosome_length()  const
{
    return dna->GetColNum();
}

void  individual::set_chromosome_length(uint32 len)
{
    chromosome_length = len;
    dna->Resize(chromosome_number, len);
    untest();
}

void individual::chromosome_random(uint32 crom)
{
    if (crom >= get_chromosome_length())
        return;

    dna->RandomizeRow(crom);
    untest();
}

string individual::info(bool format)
{
    stringstream out;

    out << "faults   : " << detected << "/" << n_tests 
        << " (" << ((n_tests > 0)?((float)(100*detected)/(float)n_tests):n_tests) ;
    if (format)
        out << "%%)" << endl;
    else
        out << "%)" << endl;
    out << "fitness : " << fitness << endl;
    out << "dna     : (" <<
           dna->GetRowNum() << "," << dna->GetColNum() << ")" << endl;
    out << dna->ToString();

    return out.str();
}

bool individual::operator < (const individual& ind)
{
    return fitness < ind.get_fitness();
}

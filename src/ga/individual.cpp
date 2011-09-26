#include "individual.h"

individual::individual(uint32 chromosome_len, uint32 chromosome_num)
{
    fitness = 0;
    dna = new bitmatrix(chromosome_num, chromosome_len);
    tested = false;
}

individual::individual(const individual &ind) : tests (ind)
{
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
    return get_chromosome_number() * get_chromosome_length();
}

void individual::dna_random()
{
    dna->RandomizeAll();
    untest();
}

void individual::dna_mutate(float mutation_rate)
{    
    uint32 row_r = randmm(0, get_chromosome_number()),
           col_r = randmm(0, get_chromosome_length());
    float count = mutation_rate * (float) get_dna_length() * std::abs((double)rand_gaussian(1,1)); // HACK TEST

    LOG("ga_events", "mutation", "# mutation event! count: %f\n", count);

    while (count >= 1.0f)
    {
        dna->Flip(row_r,col_r);
        row_r = randmm(0, get_chromosome_number());
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
    untest();
}

void individual::dna_split(int pos_1, string* dna_1, string* dna_2, int pos_2,  string* dna_3)
{
    if (pos_1 > dna->GetColNum() || pos_2 > dna->GetColNum())
        return;

    LOG_STATIC("ga_events", "mating", "# splitting DNA\n");

    if (pos_2 >= 0)
    {
        if (pos_1 > pos_2)
        {
            uint32 temp_pos = pos_1;
            pos_1 = pos_2;
            pos_2 = temp_pos;    
        }       
       
        *dna_1 = dna->GetCols(0, pos_1);
        *dna_2 = dna->GetCols(pos_1 + 1, pos_2); // if pos_1 == pos_2 ritorna ""
        *dna_3 = dna->GetCols(pos_2 + 1, dna->GetColNum()-1);

        LOG("ga_events", "mating", "dna_1 (0, %d)\n%s\n", pos_1, dna_1->c_str());
        LOG("ga_events", "mating", "dna_2 (%d,%d)\n%s\n", pos_1+1, pos_2, dna_2->c_str());
        LOG("ga_events", "mating", "dna_3 (%d,%d)\n%s\n", pos_2+1, dna->GetColNum()-1, dna_3->c_str());
    }
    else
    {
        *dna_1 = dna->GetCols(0, pos_1);
        *dna_2 = dna->GetCols(pos_1 + 1, dna->GetColNum()-1);

        LOG("ga_events", "mating", "dna_1 (0, %d)\n%s\n", pos_1, dna_1->c_str());
        LOG("ga_events", "mating", "dna_2 (%d,%d)\n%s\n", pos_1+1, dna->GetColNum()-1, dna_2->c_str());
    }
}

void individual::dna_merge(string* dna_1, string* dna_2, string* dna_3)
{
    LOG_STATIC("ga_events", "mating", "# merging DNA\n"); 

    if (dna_1 && dna_1->length() && GetStrColSize(*dna_1) == dna->GetRowNum())
    {
        set_chromosome_length(GetStrRowSize(*dna_1));
        dna->SetCols((*dna_1), 0);

        if (dna_2 && dna_2->length() && GetStrColSize(*dna_2) == dna->GetRowNum())
            dna->AttachCols(*dna_2);

        if (dna_3 && dna_3->length() && GetStrColSize(*dna_3) == dna->GetRowNum())
            dna->AttachCols(*dna_3);
    }
    else if (dna_2 && dna_2->length() && GetStrColSize(*dna_2) == dna->GetRowNum())
    {
        set_chromosome_length(GetStrRowSize(*dna_2));
        dna->SetCols(*dna_2, 0);

        if (dna_3 && dna_3->length() && GetStrColSize(*dna_3) == dna->GetRowNum())
            dna->AttachCols(*dna_3);
    }
    else if (dna_3 && dna_3->length() && GetStrColSize(*dna_3) == dna->GetRowNum())
    {
        set_chromosome_length(GetStrRowSize(*dna_3));
        dna->SetCols(*dna_3, 0);
    }

    LOG("ga_events", "mating", "%s\n", dna->ToString().c_str());  // TODO da inserire tutti gli if log.mating
    untest();
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
    if (!n_tests)
        return;

    switch (conf->get_int_config(CONFIG_FITNESS_TYPE))
    {
        case FAULT_RATE:
            fitness = (float)(100 * detected) / (float)(n_tests);
            break;
        case FAULT_RATE_MIN_LENGTH:
            fitness = ((float)(detected) / (float)(n_tests)) -
                 ((float)get_chromosome_length() / (10.0f * (float)(n_tests)));
            break;
        default:
            INFO("verbose", "please, select a valid fitness type\n");
            exit(1);
            break;
    }

    if (fitness < 0.0f)
        fitness = 0.01f;
}

string individual::get_chromosome(uint32 crom)  const
{
    if (crom >= get_chromosome_number())
        return "";

    return dna->GetRow(crom);
}

void individual::set_chromosome(uint32 crom, string str)
{
    if (crom >= get_chromosome_number())
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
    dna->Resize(get_chromosome_number(), len);
    untest();
}

uint32 individual::get_chromosome_number()  const
{
    return dna->GetRowNum();
}

void individual::set_chromosome_number(uint32 chromosome_num)
{
    dna->Resize(dna->GetRowNum(), chromosome_num);
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

bool individual::operator == (const individual& ind)
{
    return dna->ToString() == ind.dna->ToString();
}

bool individual::operator < (const individual& ind)
{
    if (fitness < ind.get_fitness())
        return true;
    if (fitness == ind.get_fitness())
    {
        if (GetFaultCoverage() < ind.GetFaultCoverage())
            return true;
        if (GetFaultCoverage() == ind.GetFaultCoverage() && get_chromosome_length() > ind.get_chromosome_length())
            return true;
    }
    return false;
}

bool individual::operator > (const individual& ind)
{
    if (fitness > ind.get_fitness())
        return true;
    if (fitness == ind.get_fitness())
    {
        if (GetFaultCoverage() > ind.GetFaultCoverage())
            return true;
        if (GetFaultCoverage() == ind.GetFaultCoverage() && get_chromosome_length() < ind.get_chromosome_length())
            return true;
    }
    return false;
}

#include <cstdlib>
#include <string>
#include <confuse.h>

#include "common/typedefs.h"

using namespace std;

//! Questa struct contiene tutti i parametri di configurazione del programma.
/**
 *
 */

typedef struct config
{
    bool debug, verbose, interactive;
    bool print_best_dna, print_best_fitness;
    bool print_population_size, print_mating;
    bool print_mutations;

    string simulator_dir, simulator_bin, simulator_patch;
    string simulator_args;
    string test_file_outpath, test_file_inpath;

    uint32 max_generations;
    uint32 max_stall;

    uint32 population_size;

    uint32 chromosome_num;
    uint32 chromosome_max_len;
    uint32 chromosome_start_len_min, chromosome_start_len_max;

    float  mating_fraction;
    float  mating_rate;
    float  mutation_rate, mutation_strength;
} config;

bool load_config(string , config *);
void help_config();

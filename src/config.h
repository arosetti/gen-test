#include <cstdlib>
#include <string>
#include <confuse.h>

#include "common/utility.h"

using namespace std;

//! Contiene i parametri di configurazione
/**
 *
 */
typedef struct config
{
    string conf_filename;
    bool debug, verbose, interactive;
    bool print_best, print_avg_fitness;
    bool print_population_size, print_mating;
    bool print_mutations;

    bool print_simulation;

    string simulator_dir, simulator_bin, simulator_patch;
    string simulator_args;
    string test_file_out, test_file_in;
    string thread_prefix;
    uint32 thread_slots;

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

extern config *conf;

void init_config();
bool load_config();
void help_config();

int  load_args(int argc, char **argv);
void help_args();

void interactive();


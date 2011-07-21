#include <cstdlib>
#include <string>
#include <confuse.h>

#include "common/typedefs.h"

using namespace std;

typedef struct config 
{
    bool debug, verbose, interactive;
    bool print_best_dna, print_best_fitness;
    bool print_population_size, print_kills;
    bool print_mutations;

    string simulator_dir, simulator_bin, simulator_patch;
    string test_file_outpath, test_file_inpath;

    uint32 max_iterations;
    
    uint32 dna_length;
    uint32 chromosome_length;
        
    uint32 avg_population_size;
    
    float mutation_rate, mutation_strength;
    float mate_rate, kill_rate;
    uint32 max_stall;
    
} config;

bool load_config(string , config *);
void help_config();

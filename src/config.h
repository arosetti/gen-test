#include <cstdlib>
#include <string>
#include <confuse.h>

#include "common/typedefs.h"

using namespace std;

typedef struct config 
{
    bool debug, verbose;
    bool print_best_chromosome, print_best_fitness;
    
    uint32 max_iterations;
    
    uint32 gene_length;
    uint32 chromosome_length;
        
    uint32 avg_population_size;
    
    float mutation_rate, mutation_strength;
    float mate_rate, kill_rate;
    uint32 max_stall;
    
} config;

bool load_config(string , config *);
void help_config();

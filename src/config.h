#include <string>
#include <confuse.h>

typedef struct config 
{
    bool debug;
    
    uint32 max_iterations;
    uint32 max_gene_length;
    
    uint32 avg_population_size;
    
    ufloat mutation_rate, mutation_strength;
    ufloat kill_rate;
    uint32 max_stall;
    
} config;

bool load_config(string , config *);

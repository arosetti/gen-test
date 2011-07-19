#include <string>
#include <confuse.h>

#include "typedefs.h"

using namespace std;

typedef struct config 
{
    bool debug,verbose;
    
    uint32 max_iterations;
    uint32 max_gene_length;
    
    uint32 avg_population_size;
    
    float mutation_rate, mutation_strength;
    float mate_rate, kill_rate;
    uint32 max_stall;
    
} config;

bool load_config(string , config *);
void help_config();

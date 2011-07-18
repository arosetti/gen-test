#include <string>
#include <confuse.h>

typedef struct config 
{
    bool debug;
    int max_iterations;
    int pool_size;
    float mutation_rate, mutation_strength;
    float kill_rate;
    int max_stall;
} config;

bool load_config(string , config *);

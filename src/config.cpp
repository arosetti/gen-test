#include "config.h"

bool load_config(string conf_filename, config *c)
{
    /*CFG_STR(), CFG_INT(), CFG_BOOL() CFG_FLOAT*/
    
    cfg_opt_t opts[] =
    {
        CFG_BOOL((char*)"debug", (cfg_bool_t)false, CFGF_NONE),
        CFG_BOOL((char*)"verbose",(cfg_bool_t)true, CFGF_NONE),
        CFG_BOOL((char*)"print_best",(cfg_bool_t)false, CFGF_NONE),    
        
        CFG_INT((char*)"avg_population_size", 4096, CFGF_NONE),
        CFG_INT((char*)"max_iterations",  8192, CFGF_NONE),
        CFG_INT((char*)"gene_length", 15 , CFGF_NONE),
        CFG_INT((char*)"chromosome_length",  10, CFGF_NONE),        
        CFG_INT((char*)"max_stall", 100, CFGF_NONE),
        CFG_FLOAT((char*)"mutation_rate", 0.06f, CFGF_NONE),
        CFG_FLOAT((char*)"mutation_strength", 2.0f, CFGF_NONE),
        CFG_FLOAT((char*)"mate_rate", 0.1f, CFGF_NONE),
        CFG_FLOAT((char*)"kill_rate", 0.1f, CFGF_NONE),
        CFG_END()
    };
    
    cfg_t *cfg = cfg_init(opts, CFGF_NONE);
    if(cfg_parse(cfg, conf_filename.c_str()) == CFG_PARSE_ERROR)
        return 1;
        
    c->debug = cfg_getbool(cfg, "debug");
    c->verbose = cfg_getbool(cfg, "verbose");
    c->print_best = cfg_getbool(cfg, "print_best");
    
    c->avg_population_size = cfg_getint(cfg, "avg_population_size");
    c->max_iterations = cfg_getint(cfg, "max_iterations");
    c->gene_length = cfg_getint(cfg, "gene_length");
    c->chromosome_length = cfg_getint(cfg, "chromosome_length");
    
    c->mutation_rate = cfg_getfloat(cfg, "mutation_rate");
    c->mutation_strength = cfg_getfloat(cfg, "mutation_strength");    
    c->mate_rate = cfg_getfloat(cfg, "mate_rate");
    c->kill_rate = cfg_getfloat(cfg, "kill_rate");
    
    cfg_free(cfg);
        
    return 0;    
}

void help_config()
{


}

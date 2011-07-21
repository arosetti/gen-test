#include "config.h"

bool load_config(string conf_filename, config *c)
{
    /*CFG_STR(), CFG_INT(), CFG_BOOL() CFG_FLOAT*/
    
    cfg_opt_t opts[] =
    {
        CFG_BOOL((char*)"debug", (cfg_bool_t)false, CFGF_NONE),
        CFG_BOOL((char*)"verbose",(cfg_bool_t)true, CFGF_NONE),
        CFG_BOOL((char*)"interactive",(cfg_bool_t)false, CFGF_NONE),
        CFG_BOOL((char*)"print_best_dna",(cfg_bool_t)false, CFGF_NONE),
        CFG_BOOL((char*)"print_best_fitness",(cfg_bool_t)true, CFGF_NONE),
        CFG_BOOL((char*)"print_population_size",(cfg_bool_t)false, CFGF_NONE),
        CFG_BOOL((char*)"print_kills",(cfg_bool_t)false, CFGF_NONE),
        CFG_BOOL((char*)"print_mutations",(cfg_bool_t)false, CFGF_NONE),

        CFG_STR((char*)"simulator_dir",(char*)"",CFGF_NONE),
        CFG_STR((char*)"simulator_bin",(char*)"",CFGF_NONE),
        CFG_STR((char*)"simulator_patch",(char*)"simulator.patch",CFGF_NONE),
        CFG_STR((char*)"test_file_outpath",(char*)"",CFGF_NONE),
        CFG_STR((char*)"test_file_inpath",(char*)"",CFGF_NONE),

        CFG_INT((char*)"avg_population_size", 4096, CFGF_NONE),
        CFG_INT((char*)"max_iterations",  8192, CFGF_NONE),
        CFG_INT((char*)"dna_length", 15 , CFGF_NONE),
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
    {
        exit(0);    
    }
        
    c->debug = cfg_getbool(cfg, "debug");
    c->verbose = cfg_getbool(cfg, "verbose");
    c->interactive = cfg_getbool(cfg, "interactive");

    c->print_best_dna= cfg_getbool(cfg, "print_best_dna");
    c->print_best_fitness = cfg_getbool(cfg, "print_best_fitness");
    c->print_population_size = cfg_getbool(cfg, "print_population_size");
    c->print_kills = cfg_getbool(cfg, "print_kills");
    c->print_mutations = cfg_getbool(cfg, "print_mutations");

    c->simulator_dir = cfg_getstr(cfg, "simulator_dir");
    c->simulator_bin = cfg_getstr(cfg, "simulator_bin");
    c->simulator_patch = cfg_getstr(cfg, "simulator_patch");
    c->test_file_outpath = cfg_getstr(cfg, "test_file_outpath");
    c->test_file_inpath = cfg_getstr(cfg, "test_file_inpath");


    c->avg_population_size = cfg_getint(cfg, "avg_population_size");
    c->max_iterations = cfg_getint(cfg, "max_iterations");
    c->dna_length = cfg_getint(cfg, "dna_length");
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

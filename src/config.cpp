#include "config.h"

bool load_config(string conf_filename, config *c)
{
    /*CFG_STR(), CFG_INT(), CFG_BOOL() CFG_FLOAT*/
    
    cfg_opt_t opts[] =
    {
        CFG_BOOL("debug", 0, CFGF_NONE),
        CFG_INT("avg_population_size", 4096, CFGF_NONE),
        CFG_INT("max_iterations",  8192, CFGF_NONE),
        CFG_INT("max_stall", 100, CFGF_NONE),
        CFG_FLOAT("mutation_rate", 0.01f, CFGF_NONE),
        CFG_FLOAT("mutation_strength", 0.01f, CFGF_NONE),
        CFG_END()
    };
    cfg_t *cfg;
    
    cfg = cfg_init(opts, CFGF_NONE);
	if(cfg_parse(cfg, conf_filename) == CFG_PARSE_ERROR)
	    return 1;
	    
	c->debug = cfg_getbool(cfg, "debug");
	c->population_size = cfg_getint(cfg, "avg_population_size");
	c->max_iterations = cfg_getint(cfg, "max_iterations");
	c->mutation_rate = cfg_getfloat(cfg, "mutation_rate");
	c->mutation_strength = cfg_getfloat(cfg, "mutation_strength");	
	
	cfg_free(cfg);
	    
    return 0;    
}

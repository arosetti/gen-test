#include "config.h"

void init_config()
{
    if(!conf)
    {
        cout << "config is null" << endl;
        abort();
    }

    conf->conf_filename = "config.conf";
}

bool load_config()
{
    if (!file_exists(conf->conf_filename.c_str()))
        return 1;

    cfg_opt_t opts[] =
    {
        CFG_BOOL((char*)"debug", (cfg_bool_t)false, CFGF_NONE),
        CFG_BOOL((char*)"verbose",(cfg_bool_t)true, CFGF_NONE),
        CFG_BOOL((char*)"interactive",(cfg_bool_t)false, CFGF_NONE),
        CFG_BOOL((char*)"print_best",(cfg_bool_t)false, CFGF_NONE),
        CFG_BOOL((char*)"print_avg_fitness",(cfg_bool_t)true, CFGF_NONE),
        CFG_BOOL((char*)"print_population_size",(cfg_bool_t)false, CFGF_NONE),
        CFG_BOOL((char*)"print_mating",(cfg_bool_t)false, CFGF_NONE),
        CFG_BOOL((char*)"print_mutations",(cfg_bool_t)false, CFGF_NONE),

        CFG_BOOL((char*)"print_simulation",(cfg_bool_t)false, CFGF_NONE),

        CFG_STR((char*)"simulator_dir",(char*)"",CFGF_NONE),
        CFG_STR((char*)"simulator_bin",(char*)"",CFGF_NONE),
        CFG_STR((char*)"simulator_args",(char*)"",CFGF_NONE),
        CFG_STR((char*)"simulator_patch",(char*)"simulator.patch",CFGF_NONE),
        CFG_STR((char*)"test_file_out",(char*)"",CFGF_NONE),
        CFG_STR((char*)"test_file_in",(char*)"",CFGF_NONE),

        CFG_INT((char*)"max_generations",  100, CFGF_NONE),
        CFG_INT((char*)"max_stall", 10, CFGF_NONE),

        CFG_INT((char*)"population_size", 50, CFGF_NONE),

        CFG_INT((char*)"chromosome_num", 5 , CFGF_NONE),
        CFG_INT((char*)"chromosome_max_len", 100 , CFGF_NONE),
        CFG_INT((char*)"chromosome_start_len_min",  10, CFGF_NONE),
        CFG_INT((char*)"chromosome_start_len_max",  20, CFGF_NONE),

        CFG_FLOAT((char*)"mating_fraction", 0.5f, CFGF_NONE),
        CFG_FLOAT((char*)"mating_rate", 0.1f, CFGF_NONE),

        CFG_FLOAT((char*)"mutation_rate", 0.05f, CFGF_NONE),
        CFG_FLOAT((char*)"mutation_strength", 2.0f, CFGF_NONE),
        CFG_END()
    };

    cfg_t *cfg = cfg_init(opts, CFGF_NONE);
    if(cfg_parse(cfg, conf->conf_filename.c_str()) == CFG_PARSE_ERROR)
    {
        cfg_free(cfg);
        exit(0);
    }

    conf->debug = cfg_getbool(cfg, "debug");
    conf->verbose = cfg_getbool(cfg, "verbose");
    conf->interactive = cfg_getbool(cfg, "interactive");

    conf->print_best = cfg_getbool(cfg, "print_best");
    conf->print_avg_fitness = cfg_getbool(cfg, "print_avg_fitness");
    conf->print_population_size = cfg_getbool(cfg, "print_population_size");
    conf->print_mating = cfg_getbool(cfg, "print_mating");
    conf->print_mutations = cfg_getbool(cfg, "print_mutations");

    conf->print_simulation = cfg_getbool(cfg, "print_simulation");

    conf->simulator_dir = cfg_getstr(cfg, "simulator_dir");
    conf->simulator_bin = cfg_getstr(cfg, "simulator_bin");
    conf->simulator_args = cfg_getstr(cfg, "simulator_args");
    conf->simulator_patch = cfg_getstr(cfg, "simulator_patch");
    conf->test_file_out = cfg_getstr(cfg, "test_file_out");
    conf->test_file_in = cfg_getstr(cfg, "test_file_in");


    conf->max_generations = cfg_getint(cfg, "max_generations");
    conf->max_stall = cfg_getint(cfg, "max_stall");

    conf->population_size = cfg_getint(cfg, "population_size");

    conf->chromosome_num = cfg_getint(cfg, "chromosome_num");
    conf->chromosome_max_len = cfg_getint(cfg, "chromosome_max_len");
    conf->chromosome_start_len_min = cfg_getint(cfg, "chromosome_start_len_min");
    conf->chromosome_start_len_max = cfg_getint(cfg, "chromosome_start_len_max");

    conf->mating_rate = cfg_getfloat(cfg, "mating_rate");
    conf->mating_fraction = cfg_getfloat(cfg, "mating_fraction");

    conf->mutation_rate = cfg_getfloat(cfg, "mutation_rate");
    conf->mutation_strength = cfg_getfloat(cfg, "mutation_strength");

    cfg_free(cfg);

    return 0;
}

void help_config()
{
    cout << "help config: " << endl;
    //TODO help config
}

int load_args(int argc, char **argv)
{
    char *c_value = NULL;
    int index;
    int opt;

    opterr = 0;

    while ((opt = getopt (argc, argv, "hdIc:")) != -1)
        switch (opt)
        {
            case 'h':
                help_args();
                exit(0);
            break;
            case 'd':
                conf->debug = 1;
            break;
            case 'I':
                conf->interactive = 1;
                interactive();
                return 0;
            break;
            case 'c':
                conf->conf_filename = optarg;
                if (!file_exists(optarg))
                {
                    cout << "config file " << optarg << " does not exists!" << endl;
                    exit(0);
                }
                cout << "* loading config from" << optarg << endl;
                load_config();
            break;
            case '?':
                if (optopt == 'c')
                fprintf (stderr, "option -%c requires an argument.\n", optopt);
                else if (isprint (optopt))
                fprintf (stderr, "unknown option `-%c'.\n", optopt);
                else
                fprintf (stderr,
                        "unknown option character `\\x%x'.\n",
                        optopt);
                return 1;
            default:
            exit(0);
        }

    for (index = optind; index < argc; index++)
        cout << "non-option argument " << argv[index] << endl;
     
    return 0;
}

void help_args()
{
    cout << "help: " << endl;
    cout << "-h             :  help" << endl;
    cout << "-d             :  debug mode on" << endl;
    cout << "-I             :  interactive mode on" << endl;
    cout << "-c <filename>  :  alternative config filename" << endl;
    cout << endl;
}

void interactive()
{
    uint32 value;

    if(!conf->interactive)
        return;

    cout << "generations: ";
    cin  >> value;
    conf->max_generations = value;

    cout << "population: ";
    cin >> value;
    conf->population_size = value;

    cout << "chromosome_max_len: ";
    cin  >> value;
    conf->chromosome_max_len = value;

    cout << "chromosome_start_len_min: ";
    cin  >> value;
    conf->chromosome_start_len_min = value;

    cout << "chromosome_start_len_max: ";
    cin  >> value;
    conf->chromosome_start_len_max = value;

    cout << endl; // TODO finire la modalità interactive
}

#include "config.h"
#include "simulation/simulation_utility.h"

cfg_opt_t opts[] =
{
    CFG_BOOL((char*)"debug", (cfg_bool_t)false, CFGF_NONE),
    CFG_BOOL((char*)"verbose",(cfg_bool_t)true, CFGF_NONE),
    CFG_BOOL((char*)"log",(cfg_bool_t)true, CFGF_NONE),

    CFG_BOOL((char*)"print_progress_bar",(cfg_bool_t)true, CFGF_NONE),
    CFG_BOOL((char*)"print_best",(cfg_bool_t)true, CFGF_NONE),
    CFG_BOOL((char*)"print_avg_fitness",(cfg_bool_t)true, CFGF_NONE),
    CFG_BOOL((char*)"print_avg_chromosome_length",(cfg_bool_t)true, CFGF_NONE),
    CFG_BOOL((char*)"print_population_size",(cfg_bool_t)true, CFGF_NONE),

    CFG_BOOL((char*)"log_mating",(cfg_bool_t)false, CFGF_NONE),     // trasformare in log
    CFG_BOOL((char*)"log_mutations",(cfg_bool_t)false, CFGF_NONE),  // trasformare in log
    CFG_BOOL((char*)"log_simulation",(cfg_bool_t)false, CFGF_NONE), // trasformare in log

    CFG_STR((char*)"simulator_path",(char*)"",CFGF_NONE),
    CFG_STR((char*)"simulator_bin",(char*)"",CFGF_NONE),
    CFG_STR((char*)"simulator_args",(char*)"",CFGF_NONE),
    CFG_STR((char*)"simulator_patch",(char*)"simulator.patch",CFGF_NONE),
    CFG_STR((char*)"test_file_out",(char*)"",CFGF_NONE),
    CFG_STR((char*)"test_file_in",(char*)"",CFGF_NONE),
    CFG_STR((char*)"log_path",(char*)"logs",CFGF_NONE),

    CFG_STR((char*)"thread_prefix",(char*)"sim_",CFGF_NONE),
    CFG_INT((char*)"thread_slots",  4, CFGF_NONE),
    CFG_INT((char*)"max_threads", 32, CFGF_NONE),
    
    CFG_INT((char*)"max_generations",  100, CFGF_NONE),
    CFG_INT((char*)"max_retest", 3, CFGF_NONE),
    CFG_INT((char*)"max_stall", 10, CFGF_NONE),
    CFG_BOOL((char*)"check_stall", (cfg_bool_t)false, CFGF_NONE),

    CFG_INT((char*)"population_size", 50, CFGF_NONE),

    CFG_STR((char*)"fitness_type",(char*)"fault_rate",CFGF_NONE),
    CFG_STR((char*)"cut_type",(char*)"random",CFGF_NONE),

    CFG_INT((char*)"chromosome_max_len", 100 , CFGF_NONE),
    CFG_INT((char*)"chromosome_start_len_min",  10, CFGF_NONE),
    CFG_INT((char*)"chromosome_start_len_max",  20, CFGF_NONE),

    CFG_FLOAT((char*)"mating_fraction", 0.5f, CFGF_NONE),
    CFG_FLOAT((char*)"mating_rate", 0.1f, CFGF_NONE),

    CFG_FLOAT((char*)"mutation_rate", 0.05f, CFGF_NONE),
    CFG_BOOL((char*)"mutation_length_gene", (cfg_bool_t)false, CFGF_NONE),
    CFG_END()
};

const char *fitness_types[] =
{
    "fault_rate",
    "fault_rate_linear_min_length",
    NULL
};

const char *cut_types[] =
{
    "half",
    "single_random",
    "double_random",
    NULL
};

cfg_t *open_cfg()
{
    cfg_t *cfg = cfg_init(opts, CFGF_NONE);

    if (cfg_parse(cfg, conf->conf_filename.c_str()) == CFG_PARSE_ERROR)
    {
        cfg_free(cfg);
        exit(0);
    }

    return cfg;
}
    
void init_config()
{
    if (!conf)
    {
        cout << "config is null" << endl;
        abort();
    }

    conf->conf_filename = "config.conf";

    conf->main_path = get_current_dir_name();
    
}

void post_init_config()
{
    int ret;

    conf->chromosome_num = read_n_inputs();

    if (!file_exists(conf->log_path))  // usare dir_exists
        mkdir(conf->log_path.c_str(),0777);

    stringstream str;
    str << "rm -f " << conf->log_path << "/generation*.log";
    ret = system(str.str().c_str());
}

bool load_config()
{
    if (!file_exists(conf->conf_filename.c_str()))
        return 1;

    cfg_t *cfg = open_cfg();

    conf->debug = cfg_getbool(cfg, "debug");
    conf->verbose = cfg_getbool(cfg, "verbose");
    conf->log = cfg_getbool(cfg, "log");

    conf->print_progress_bar = cfg_getbool(cfg, "print_progress_bar");
    conf->print_best = cfg_getbool(cfg, "print_best");
    conf->print_avg_chromosome_length = cfg_getbool(cfg, "print_avg_chromosome_length");
    conf->print_avg_fitness = cfg_getbool(cfg, "print_avg_fitness");
    conf->print_population_size = cfg_getbool(cfg, "print_population_size");
    conf->log_mating = cfg_getbool(cfg, "log_mating");
    conf->log_mutations = cfg_getbool(cfg, "log_mutations");

    conf->log_simulation = cfg_getbool(cfg, "log_simulation");

    conf->simulator_path = cfg_getstr(cfg, "simulator_path");
    conf->simulator_bin = cfg_getstr(cfg, "simulator_bin");
    conf->simulator_args = cfg_getstr(cfg, "simulator_args");
    conf->simulator_patch = cfg_getstr(cfg, "simulator_patch");
    conf->test_file_out = cfg_getstr(cfg, "test_file_out");
    conf->test_file_in = cfg_getstr(cfg, "test_file_in");
    conf->log_path = cfg_getstr(cfg, "log_path");

    conf->thread_prefix = cfg_getstr(cfg, "thread_prefix");
    conf->thread_slots = cfg_getint(cfg, "thread_slots");
    conf->max_threads = cfg_getint(cfg, "max_threads");

    conf->max_generations = cfg_getint(cfg, "max_generations");
    conf->max_retest = cfg_getint(cfg, "max_retest");
    conf->max_stall = cfg_getint(cfg, "max_stall");
    conf->check_stall = cfg_getbool(cfg, "check_stall");
    
    conf->population_size = cfg_getint(cfg, "population_size");
    conf->fitness_type = cfg_getstr(cfg, "fitness_type");
    conf->cut_type = cfg_getstr(cfg, "cut_type");

    conf->chromosome_max_len = cfg_getint(cfg, "chromosome_max_len");
    conf->chromosome_start_len_min = cfg_getint(cfg, "chromosome_start_len_min");
    conf->chromosome_start_len_max = cfg_getint(cfg, "chromosome_start_len_max");

    conf->mating_rate = cfg_getfloat(cfg, "mating_rate");
    conf->mating_fraction = cfg_getfloat(cfg, "mating_fraction");

    conf->mutation_rate = cfg_getfloat(cfg, "mutation_rate");
    conf->mutation_length_gene = cfg_getbool(cfg, "mutation_length_gene");

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
    int opt, tmp_i;

    opterr = 0;

    while ((opt = getopt (argc, argv, "hdIt:c:s:")) != -1)
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
                interactive();
                return 0;
            break;
            case 'c':
                conf->conf_filename = optarg;
                if (!file_exists(optarg))
                {
                    cout << "config file " << optarg << " does not exists!" << endl;
                    exit(1);
                }
                cout << "* loading config from" << optarg << endl;
                load_config();
            break;
            case 't':
                tmp_i = atoi(optarg);
                if (tmp_i > 0 && tmp_i <= conf->max_threads)
                    conf->thread_slots = tmp_i;
                else
                    cout << "wrong -t parameter. you must use a value between 1 and "
                         << conf->max_threads << endl << endl;
            break;
            case 's':
                if (file_exists(optarg))  //TODO controllare che sia una dir con dir_exists
                    conf->simulator_path = optarg;
                else
                    cout << "wrong -s parameter. you must use a valid simulator directory " 
                         << endl << endl;
            break;
            case '?':
                if (optopt == 'c')
                fprintf (stderr, "option -%c requires an argument.\n\n", optopt);
                else if (optopt == 't')
                fprintf (stderr, "option -%c requires an argument.\n\n", optopt);
                else if (optopt == 's')
                fprintf (stderr, "option -%c requires an argument.\n\n", optopt);
                else if (isprint (optopt))
                fprintf (stderr, "unknown option `-%c'.\n\n", optopt);
                else
                fprintf (stderr,
                        "unknown option character `\\x%x'.\n\n",
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
    cout << "-h                  :  help" << endl;
    cout << "-d                  :  debug mode on" << endl;
    cout << "-I                  :  interactive mode on" << endl;
    cout << "-c <filename>       :  alternative config filename" << endl;
    cout << "-t <threads>        :  set thread number" << endl;
    cout << "-s <simulator path> :  set simulator directory" << endl;
    cout << endl;
}

void interactive()
{
    uint32 value;
    char response;

    cfg_t *cfg = open_cfg();

    cout << "generations: ";
    cin  >> value;
    conf->max_generations = value;
    cfg_setint(cfg, "max_generations", value);

    cout << "population: ";
    cin >> value;
    conf->population_size = value;
    cfg_setint(cfg, "population_size", value);

    cout << "thread slots: ";
    cin >> value;
    conf->thread_slots = value;
    cfg_setint(cfg, "thread_slots", value);

    cout << "chromosome_max_len: ";
    cin  >> value;
    conf->chromosome_max_len = value;
    cfg_setint(cfg, "chromosome_max_len", value);

    cout << "chromosome_start_len_min: ";
    cin  >> value;
    conf->chromosome_start_len_min = value;
    cfg_setint(cfg, "chromosome_start_len_min", value);

    cout << "chromosome_start_len_max: ";
    cin  >> value;
    conf->chromosome_start_len_max = value;
    cfg_setint(cfg, "chromosome_start_len_max", value);

    cout << endl; // TODO finire la modalità interactive

    cout << "do you want to save new options to " << conf->conf_filename << "? (y/n) ";
    cin  >> response;
    if(response == 'y')
    {
        FILE *fp = fopen(conf->conf_filename.c_str(), "w");
        cfg_print(cfg, fp);
        fclose(fp);
    }
    cout << endl;

    cfg_free(cfg);
}

void check_config() // TODO inserire altri controlli
{
    if (conf->thread_slots <= 0)
    {
        cout << "use at least one thread in thread_slots" << endl;
        exit(1);
    }

    if (conf->thread_slots > conf->max_threads)
    {
        cout << "max_threads (" << conf->max_threads  << ") exceeded!" << endl;
        conf->thread_slots = conf->max_threads;
    }

    if (conf->max_retest < 0)
    {
        cout << "use max_retest >= 0" << endl;
        exit(1);
    }
    
    if (conf->population_size == 0) 
    {
        cout << "can't use zero population size" << endl;
        exit(1);
    }

    if (conf->chromosome_start_len_min == 0 || conf->chromosome_start_len_max == 0)
    {
        cout << "can't use zero chromosome length" << endl;
        exit(1);
    }

    if (conf->chromosome_start_len_min > conf->chromosome_start_len_max)
    {
        cout << "chromosome_start_len_min value must be lower than chromosome_start_len_max value" << endl;
        exit(1);
    }

    if (conf->chromosome_num == 0)
    {
        cout << "chromosome number is 0, errror reading simulator output.net" << endl;
        exit(1);
    }
}

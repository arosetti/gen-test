#include "config.h"
#include "simulation/simulation_utility.h"

cfg_opt_t opts[] =
{
    CFG_BOOL((char*)"debug", (cfg_bool_t)false, CFGF_NONE),
    CFG_BOOL((char*)"verbose",(cfg_bool_t)true, CFGF_NONE),
    CFG_BOOL((char*)"log",(cfg_bool_t)true, CFGF_NONE),
    CFG_BOOL((char*)"graphics",(cfg_bool_t)true, CFGF_NONE),

    CFG_BOOL((char*)"print_progress_bar",(cfg_bool_t)true, CFGF_NONE),
    CFG_BOOL((char*)"print_best",(cfg_bool_t)true, CFGF_NONE),
    CFG_BOOL((char*)"print_avg_fitness",(cfg_bool_t)true, CFGF_NONE),
    CFG_BOOL((char*)"print_avg_chromosome_length",(cfg_bool_t)true, CFGF_NONE),
    CFG_BOOL((char*)"print_population_size",(cfg_bool_t)true, CFGF_NONE),

    CFG_BOOL((char*)"log_mating",(cfg_bool_t)false, CFGF_NONE),
    CFG_BOOL((char*)"log_mutations",(cfg_bool_t)false, CFGF_NONE),
    CFG_BOOL((char*)"log_simulation",(cfg_bool_t)false, CFGF_NONE),

    CFG_STR((char*)"simulator_path",(char*)"",CFGF_NONE),
    CFG_STR((char*)"simulator_bin",(char*)"",CFGF_NONE),
    CFG_STR((char*)"simulator_args",(char*)"",CFGF_NONE),
    CFG_STR((char*)"simulator_patch",(char*)"simulator.patch",CFGF_NONE),
    CFG_STR((char*)"test_file_out",(char*)"",CFGF_NONE),
    CFG_STR((char*)"test_file_in",(char*)"",CFGF_NONE),
    CFG_STR((char*)"log_path",(char*)"logs",CFGF_NONE),

    CFG_BOOL((char*)"read_faults_file",(cfg_bool_t)false, CFGF_NONE),

    CFG_STR((char*)"thread_prefix",(char*)"sim_",CFGF_NONE),
    CFG_INT((char*)"thread_slots",  4, CFGF_NONE),   
    
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

    CFG_BOOL((char*)"normalized_fitness", (cfg_bool_t)false, CFGF_NONE),
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

cfg_t* config::open_cfg()
{
    cfg_t *cfg = cfg_init(opts, CFGF_NONE);

    if (cfg_parse(cfg, config_string[CONFIG_CONF_FILENAME].c_str()) == CFG_PARSE_ERROR)
    {
        cfg_free(cfg);
        exit(0);
    }

    return cfg;
}

config::config()
{
    config_string[CONFIG_CONF_FILENAME] = "config.conf";
    config_string[CONFIG_MAIN_PATH] = get_current_dir_name();
    load_config();
}

bool config::load_config()
{
    if (!file_exists(config_string[CONFIG_CONF_FILENAME].c_str()))
        return false;

    cfg_t *cfg = open_cfg();

    config_bool[CONFIG_DEBUG]    = cfg_getbool(cfg, "debug");
    config_bool[CONFIG_VERBOSE]  = cfg_getbool(cfg, "verbose");
    config_bool[CONFIG_LOG]      = cfg_getbool(cfg, "log");
    config_bool[CONFIG_GRAPHICS] = cfg_getbool(cfg, "graphics");

    config_bool[CONFIG_PRINT_PROGRESS_BAR]          = cfg_getbool(cfg, "print_progress_bar");
    config_bool[CONFIG_PRINT_BEST]                  = cfg_getbool(cfg, "print_best");
    config_bool[CONFIG_PRINT_AVG_CHROMOSOME_LENGTH] = cfg_getbool(cfg, "print_avg_chromosome_length");
    config_bool[CONFIG_PRINT_AVG_FITNESS]           = cfg_getbool(cfg, "print_avg_fitness");
    config_bool[CONFIG_PRINT_POPULATION_SIZE]       = cfg_getbool(cfg, "print_population_size");
    config_bool[CONFIG_LOG_MATING]                  = cfg_getbool(cfg, "log_mating");
    config_bool[CONFIG_LOG_MUTATIONS]               = cfg_getbool(cfg, "log_mutations");
    config_bool[CONFIG_LOG_SIMULATION]              = cfg_getbool(cfg, "log_simulation");
    config_bool[CONFIG_READ_FAULTS_FILE]            = cfg_getbool(cfg, "read_faults_file");

    config_string[CONFIG_SIMULATOR_PATH]  = cfg_getstr(cfg, "simulator_path");
    config_string[CONFIG_SIMULATOR_BIN]   = cfg_getstr(cfg, "simulator_bin");
    config_string[CONFIG_SIMULATOR_ARGS]  = cfg_getstr(cfg, "simulator_args");
    config_string[CONFIG_SIMULATOR_PATCH] = cfg_getstr(cfg, "simulator_patch");
    config_string[CONFIG_TEST_FILE_OUT]   = cfg_getstr(cfg, "test_file_out");
    config_string[CONFIG_TEST_FILE_IN]    = cfg_getstr(cfg, "test_file_in");
    config_string[CONFIG_LOG_PATH]        = cfg_getstr(cfg, "log_path");

    config_string[CONFIG_THREAD_PREFIX] = cfg_getstr(cfg, "thread_prefix");
    config_int[CONFIG_THREAD_SLOTS]     = cfg_getint(cfg, "thread_slots");

    config_int[CONFIG_MAX_GENERATIONS] = cfg_getint(cfg, "max_generations");
    config_int[CONFIG_MAX_RETEST]       = cfg_getint(cfg, "max_retest");
    config_int[CONFIG_MAX_STALL]       = cfg_getint(cfg, "max_stall");
    config_bool[CONFIG_CHECK_STALL]    = cfg_getbool(cfg, "check_stall");
    
    config_int[CONFIG_POPULATION_SIZE] = cfg_getint(cfg, "population_size");
    config_string[CONFIG_FITNESS_TYPE] = cfg_getstr(cfg, "fitness_type");
    config_string[CONFIG_CUT_TYPE]     = cfg_getstr(cfg, "cut_type");   

    config_int[CONFIG_CHROMOSOME_MAX_LENGTH]      = cfg_getint(cfg, "chromosome_max_len");
    config_int[CONFIG_CHROMOSOME_START_LEN_MIN] = cfg_getint(cfg, "chromosome_start_len_min");
    config_int[CONFIG_CHROMOSOME_START_LEN_MAX] = cfg_getint(cfg, "chromosome_start_len_max");

    config_bool[CONFIG_NORMALIZED_FITNESS] = cfg_getbool(cfg, "normalized_fitness");
    config_float[CONFIG_MATING_RATE]       = cfg_getfloat(cfg, "mating_rate");
    config_float[CONFIG_MATING_FRACTION]   = cfg_getfloat(cfg, "mating_fraction");

    config_float[CONFIG_MUTATION_RATE] = cfg_getfloat(cfg, "mutation_rate");
    config_bool[CONFIG_MUTATION_LENGTH_GENE] = cfg_getbool(cfg, "mutation_length_gene");

    cfg_free(cfg);

    return true;
}

void config::post_init_config()
{
    int ret;

    config_int[CONFIG_CHROMOSOME_NUM] = read_n_inputs();

    if (!file_exists(config_string[CONFIG_LOG_PATH]))  // usare dir_exists
        mkdir(config_string[CONFIG_LOG_PATH].c_str(),0777);

    stringstream str;
    str << "rm -f " << config_string[CONFIG_LOG_PATH] << "/*.log";
    ret = system(str.str().c_str());

    check_config();
    init_log_profiles();
}

void config::init_log_profiles()
{
    logger_profile *l_profile;

    l_profile = new logger_profile("info", "");
    l_profile->set_opt(L_VERBOSE | L_COLOR | L_PRINT);
    LOG->add_profile(l_profile);

    l_profile = new logger_profile("events", config_string[CONFIG_LOG_PATH]);
    l_profile->set_opt(L_APPEND | L_FILE_LOG);
    LOG->add_profile(l_profile);
    
    l_profile = new logger_profile("generations", config_string[CONFIG_LOG_PATH]);
    l_profile->set_opt(L_STATIC | L_FILE_LOG | L_INCREMENTAL | L_CLOSE);
    LOG->add_profile(l_profile);
}

void config::help_config()
{
    cout << "help config: " << endl;
    //TODO help config
}

int config::load_args(int argc, char **argv)
{
    char *c_value = NULL;
    int index;
    int opt, tmp_i;

    opterr = 0;

    while ((opt = getopt (argc, argv, "hdt:c:s:l:")) != -1)
        switch (opt)
        {
            case 'h':
                help_args();
                exit(0);
            break;
            case 'd':
                config_bool[CONFIG_DEBUG] = true;
            break;
            case 'c':
                config_string[CONFIG_CONF_FILENAME] = optarg;
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
                if (tmp_i > 0)
                    config_int[CONFIG_THREAD_SLOTS] = tmp_i;
                else
                    cout << "wrong -t parameter. you must use a value > 0 " << endl;
            break;
            case 's':
                if (file_exists(optarg))  //TODO controllare che sia una dir con dir_exists
                    config_string[CONFIG_SIMULATOR_PATH] = optarg;
                else
                    cout << "wrong -s parameter. you must use a valid simulator directory" 
                         << endl << endl;
            break;
            case 'l':
                if (file_exists(optarg))
                    config_string[CONFIG_LOAD_LOG_FILENAME] = optarg;
                else
                    cout << "wrong -s parameter. you must use a valid generation log file" 
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
                break;
            default:
                break;
        }    

    for (index = optind; index < argc; index++)
        cout << "non-option argument " << argv[index] << endl;

    post_init_config();
     
    return 0;
}

void config::help_args()
{
    cout << "help: " << endl;
    cout << "-h                  :  help" << endl;
    cout << "-d                  :  debug mode on" << endl;
    cout << "-c <filename>       :  alternative config filename" << endl;
    cout << "-t <threads>        :  set thread number" << endl;
    cout << "-s <simulator path> :  set simulator directory" << endl;
    cout << "-l <log file>       :  load execution from log" << endl;
    cout << endl;
}

void config::check_config() // TODO inserire altri controlli
{
    if (config_int[CONFIG_THREAD_SLOTS] <= 0)
    {
        config_int[CONFIG_THREAD_SLOTS] = 1;
        cout << "Use at least one thread in thread_slots, setting to " << config_int[CONFIG_THREAD_SLOTS] << endl;
    }

    if (config_int[CONFIG_MAX_RETEST] < 0)
    {
        config_int[CONFIG_MAX_RETEST] = 0;
        cout << "use max_retest >= 0, setting to " << config_int[CONFIG_MAX_RETEST] << endl;
    }
    
    if (config_int[CONFIG_POPULATION_SIZE] <= 1) 
    {
        config_int[CONFIG_POPULATION_SIZE] = 2;
        cout << "population_size must be >= 2, setting to " << config_int[CONFIG_POPULATION_SIZE] << endl;
    }

    if (config_int[CONFIG_CHROMOSOME_START_LEN_MIN] <= 0)
    {
        config_int[CONFIG_CHROMOSOME_START_LEN_MIN] = 1;
        cout << "can't use <= 0 chromosome length min, setting to" << config_int[CONFIG_CHROMOSOME_START_LEN_MIN] << endl;
    }

    if (config_int[CONFIG_CHROMOSOME_START_LEN_MAX] <= 0)
    {
        config_int[CONFIG_CHROMOSOME_START_LEN_MAX] = 1;
        cout << "can't use <= 0 chromosome length max, setting to" << config_int[CONFIG_CHROMOSOME_START_LEN_MAX] << endl;
    }

    if (config_int[CONFIG_CHROMOSOME_START_LEN_MIN] > config_int[CONFIG_CHROMOSOME_START_LEN_MAX])
    {
        config_int[CONFIG_CHROMOSOME_START_LEN_MIN] = config_int[CONFIG_CHROMOSOME_START_LEN_MAX];
        cout << "chromosome_start_len_min value must be lower than chromosome_start_len_max value, setting chromosome_start_len_min = chromosome_start_len_max" << endl;
    }

    if (config_int[CONFIG_CHROMOSOME_NUM] <= 0)
    {
        cout << "chromosome number is <=0, error reading simulator output.net" << endl;
        exit(1);
    }
}

bool config::get_bool_config(enum config_bool e_conf)
{
    if (e_conf >= CONFIG_MAX_BOOL)
        exit(1); // Errore
    
    return config_bool[e_conf];
}

int config::get_int_config(enum config_int e_conf)
{
    if (e_conf >= CONFIG_MAX_INT)
        exit(1); // Errore
    
    return config_int[e_conf];
}

std::string config::get_string_config(enum config_string e_conf)
{
    if (e_conf >= CONFIG_MAX_STRING)
        exit(1); // Errore
    
    return config_string[e_conf];
}

float config::get_float_config(enum config_float e_conf)
{
    if (e_conf >= CONFIG_MAX_FLOAT)
        exit(1); // Errore
    
    return config_float[e_conf];
}

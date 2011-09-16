#include "config.h"
#include "simulation/simulation_utility.h"

cfg_opt_t opts[] =
{
    CFG_BOOL((char*)"log.enable",(cfg_bool_t)true, CFGF_NONE),
    CFG_STR((char*)"log.path",(char*)"logs",CFGF_NONE),
    CFG_BOOL((char*)"log.debug", (cfg_bool_t)false, CFGF_NONE),
    CFG_BOOL((char*)"log.verbose",(cfg_bool_t)true, CFGF_NONE),    
    CFG_BOOL((char*)"log.graphics",(cfg_bool_t)true, CFGF_NONE),
    CFG_BOOL((char*)"log.mating",(cfg_bool_t)false, CFGF_NONE),
    CFG_BOOL((char*)"log.mutations",(cfg_bool_t)false, CFGF_NONE),
    CFG_BOOL((char*)"log.simulation",(cfg_bool_t)false, CFGF_NONE),

    CFG_BOOL((char*)"print.progress_bar",(cfg_bool_t)true, CFGF_NONE),
    CFG_BOOL((char*)"print.best",(cfg_bool_t)true, CFGF_NONE),
    CFG_BOOL((char*)"print.avg_fitness",(cfg_bool_t)true, CFGF_NONE),
    CFG_BOOL((char*)"print.avg_fault_coverage",(cfg_bool_t)true, CFGF_NONE),
    CFG_BOOL((char*)"print.avg_chromosome_length",(cfg_bool_t)true, CFGF_NONE),
    CFG_BOOL((char*)"print.population_size",(cfg_bool_t)true, CFGF_NONE),    

    CFG_STR((char*)"simulator.path",(char*)"",CFGF_NONE),
    CFG_STR((char*)"simulator.bin",(char*)"",CFGF_NONE),
    CFG_STR((char*)"simulator.args",(char*)"",CFGF_NONE),
    CFG_STR((char*)"simulator.test_file_out",(char*)"",CFGF_NONE),
    CFG_STR((char*)"simulator.test_file_in",(char*)"",CFGF_NONE), 
    CFG_BOOL((char*)"simulator.read_faults_file",(cfg_bool_t)false, CFGF_NONE),

    CFG_STR((char*)"thread.prefix",(char*)"sim_",CFGF_NONE),
    CFG_INT((char*)"thread.slots",  4, CFGF_NONE),   
    
    CFG_INT((char*)"ga.max_generations",  100, CFGF_NONE),
    CFG_INT((char*)"ga.population_size", 50, CFGF_NONE),    
    CFG_INT((char*)"ga.chromosome_max_len", 1000 , CFGF_NONE),
    CFG_INT((char*)"ga.chromosome_start_len_min",  10, CFGF_NONE),
    CFG_INT((char*)"ga.chromosome_start_len_max",  20, CFGF_NONE),
    CFG_BOOL((char*)"ga.normalized_fitness", (cfg_bool_t)false, CFGF_NONE),
    CFG_INT((char*)"ga.fitness_type", 1, CFGF_NONE),
    CFG_FLOAT((char*)"ga.mating_fraction", 0.5f, CFGF_NONE),
    CFG_FLOAT((char*)"ga.mating_rate", 0.3f, CFGF_NONE),
    CFG_FLOAT((char*)"ga.mutation_rate", 0.05f, CFGF_NONE),
    CFG_INT((char*)"ga.cut_type",1,CFGF_NONE),
    CFG_INT((char*)"ga.max_retest", 1, CFGF_NONE),
    CFG_INT((char*)"ga.max_stall", 10, CFGF_NONE),
    CFG_FLOAT((char*)"ga.mutation_stall_rate", 0.30f, CFGF_NONE),
    CFG_BOOL((char*)"ga.stop_at_100", (cfg_bool_t)true, CFGF_NONE),    
    
    CFG_BOOL((char*)"mutation_length_gene", (cfg_bool_t)false, CFGF_NONE),
    CFG_END()
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

    config_bool[CONFIG_LOG]        = cfg_getbool(cfg, "log.enable");
    config_string[CONFIG_LOG_PATH] = cfg_getstr(cfg, "log.path");
    config_bool[CONFIG_DEBUG]      = cfg_getbool(cfg, "log.debug");
    config_bool[CONFIG_VERBOSE]    = cfg_getbool(cfg, "log.verbose");    
    config_bool[CONFIG_GRAPHICS]   = cfg_getbool(cfg, "log.graphics");
    config_bool[CONFIG_LOG_MATING]     = cfg_getbool(cfg, "log.mating");
    config_bool[CONFIG_LOG_MUTATIONS]  = cfg_getbool(cfg, "log.mutations");
    config_bool[CONFIG_LOG_SIMULATION] = cfg_getbool(cfg, "log.simulation");

    config_bool[CONFIG_PRINT_PROGRESS_BAR]          = cfg_getbool(cfg, "print.progress_bar");
    config_bool[CONFIG_PRINT_BEST]                  = cfg_getbool(cfg, "print.best");
    config_bool[CONFIG_PRINT_AVG_CHROMOSOME_LENGTH] = cfg_getbool(cfg, "print.avg_chromosome_length");
    config_bool[CONFIG_PRINT_AVG_FAULT_COVERAGE]    = cfg_getbool(cfg, "print.avg_fault_coverage");
    config_bool[CONFIG_PRINT_AVG_FITNESS]           = cfg_getbool(cfg, "print.avg_fitness");
    config_bool[CONFIG_PRINT_POPULATION_SIZE]       = cfg_getbool(cfg, "print.population_size");

    config_string[CONFIG_SIMULATOR_PATH]  = cfg_getstr(cfg, "simulator.path");
    config_string[CONFIG_SIMULATOR_BIN]   = cfg_getstr(cfg, "simulator.bin");
    config_string[CONFIG_SIMULATOR_ARGS]  = cfg_getstr(cfg, "simulator.args");
    config_string[CONFIG_TEST_FILE_OUT]   = cfg_getstr(cfg, "simulator.test_file_out");
    config_string[CONFIG_TEST_FILE_IN]    = cfg_getstr(cfg, "simulator.test_file_in");
    config_bool[CONFIG_READ_FAULTS_FILE]  = cfg_getbool(cfg, "simulator.read_faults_file");

    config_string[CONFIG_THREAD_PREFIX] = cfg_getstr(cfg, "thread.prefix");
    config_int[CONFIG_THREAD_SLOTS]     = cfg_getint(cfg, "thread.slots");

    config_int[CONFIG_MAX_GENERATIONS]          = cfg_getint(cfg, "ga.max_generations");
    config_int[CONFIG_POPULATION_SIZE]          = cfg_getint(cfg, "ga.population_size");
    config_int[CONFIG_CHROMOSOME_MAX_LENGTH]    = cfg_getint(cfg, "ga.chromosome_max_len");
    config_int[CONFIG_CHROMOSOME_START_LEN_MIN] = cfg_getint(cfg, "ga.chromosome_start_len_min");
    config_int[CONFIG_CHROMOSOME_START_LEN_MAX] = cfg_getint(cfg, "ga.chromosome_start_len_max");
    config_bool[CONFIG_NORMALIZED_FITNESS]      = cfg_getbool(cfg, "ga.normalized_fitness");
    config_int[CONFIG_FITNESS_TYPE]             = cfg_getint(cfg, "ga.fitness_type");
    config_float[CONFIG_MATING_RATE]            = cfg_getfloat(cfg, "ga.mating_rate");
    config_float[CONFIG_MATING_FRACTION]        = cfg_getfloat(cfg, "ga.mating_fraction");
    config_float[CONFIG_MUTATION_RATE]          = cfg_getfloat(cfg, "ga.mutation_rate");
    config_int[CONFIG_CUT_TYPE]                 = cfg_getint(cfg, "ga.cut_type");   
    config_int[CONFIG_MAX_RETEST]               = cfg_getint(cfg, "ga.max_retest");
    config_int[CONFIG_MAX_STALL]                = cfg_getint(cfg, "ga.max_stall");
    config_float[CONFIG_MUTATION_STALL_RATE]    = cfg_getfloat(cfg, "ga.mutation_stall_rate"); 
    config_bool[CONFIG_STOP_AT_100]             = cfg_getbool(cfg, "ga.stop_at_100"); 
    
    config_bool[CONFIG_MUTATION_LENGTH_GENE] = cfg_getbool(cfg, "mutation_length_gene");

    cfg_free(cfg);

    return true;
}

void config::post_init_config()
{
    config_int[CONFIG_CHROMOSOME_NUM] = read_n_inputs();

    if (!file_exists(config_string[CONFIG_LOG_PATH]))  // usare dir_exists
        mkdir(config_string[CONFIG_LOG_PATH].c_str(), 0777);

    if (config_string[CONFIG_LOAD_LOG_FILENAME] == "")
    {
        stringstream str;
        str << "rm -f " << config_string[CONFIG_LOG_PATH] << "/*.log";
        int ret = system(str.str().c_str());
    }

    check_config();
    init_log_profiles();
}

void config::init_log_profiles()
{
    log_profile *l_profile;

    if (config_bool[CONFIG_DEBUG])
        LOG_PTR->set_mask(L_DEBUG);

    if (config_bool[CONFIG_VERBOSE])
        LOG_PTR->set_mask(L_VERBOSE);

    if (config_bool[CONFIG_LOG])
        LOG_PTR->set_mask(L_ENABLE);

    l_profile = new log_profile("verbose", "");
    l_profile->set_opt(L_VERBOSE | L_COLOR);
    LOG_PTR->add_profile(l_profile);

    l_profile = new log_profile("debug", "");
    l_profile->set_opt(L_VERBOSE | L_DEBUG | L_COLOR);
    LOG_PTR->add_profile(l_profile);

    l_profile = new log_profile("events", config_string[CONFIG_LOG_PATH]);
    l_profile->set_opt(L_APPEND | L_CLOSE);
    LOG_PTR->add_profile(l_profile);
    
    l_profile = new log_profile("mating_events", config_string[CONFIG_LOG_PATH]);
    l_profile->set_opt(L_APPEND | L_CLOSE | L_DEBUG | L_INCREMENTAL);
    LOG_PTR->add_profile(l_profile);
    
    l_profile = new log_profile("generations", config_string[CONFIG_LOG_PATH]);
    l_profile->set_opt(L_INCREMENTAL | L_CLOSE);
    LOG_PTR->add_profile(l_profile);
}

void config::help_config()
{
    cout << "help config: " << endl;
    //TODO help config
}

int config::load_args(int argc, char **argv)
{
    int opt, tmp_i;
    opterr = 0;

    while ((opt = getopt (argc, argv, "c:hdp:t:s:l:")) != -1)
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
            case 'p':
                tmp_i = atoi(optarg);
                if (tmp_i == 0 || tmp_i == 1)
                    config_bool[CONFIG_PRINT_PROGRESS_BAR] = (bool) tmp_i;
                else
                    cout << "wrong -p parameter. you must use a bool value" << endl;
            break;
            case 't':
            {
                tmp_i = atoi(optarg);
                if (tmp_i > 0)
                    config_int[CONFIG_THREAD_SLOTS] = tmp_i;
                else
                    cout << "wrong -t parameter. you must use a value > 0" << endl;
            }
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
                    cout << "wrong -s parameter. you must use a valid generation log file" << endl;
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

    for (int index = optind; index < argc; index++)
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
    cout << "-p <0, 1>           :  print progress progress bar" << endl;
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

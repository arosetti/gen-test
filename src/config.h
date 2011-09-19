#include <cstdlib>
#include <string>
#include <confuse.h>

#ifndef CONFIG_H
#define CONFIG_H

#include "common/utility.h"
#include "common/logger/logger.h"

using namespace std;

static vector<pthread_t> threads_id;

enum e_cut_type
{
    CUT_DOUBLE_RANDOM = 1,
    CUT_SINGLE_RANDOM,
    CUT_HALF,
    CUT_TWO_POINTS_DOUBLE_RANDOM,
    CUT_TWO_POINTS_SINGLE_RANDOM,
    MAX_CUT_TYPE
};

enum config_bool
{
    CONFIG_DEBUG = 0,
    CONFIG_VERBOSE,
    CONFIG_LOG,
    CONFIG_GRAPHICS,
    CONFIG_PRINT_PROGRESS_BAR,
    CONFIG_PRINT_POPULATION_SIZE,
    CONFIG_PRINT_BEST,
    CONFIG_PRINT_WORST,
    CONFIG_PRINT_AVG_FITNESS,
    CONFIG_PRINT_AVG_FAULT_COVERAGE,
    CONFIG_PRINT_AVG_CHROMOSOME_LENGTH,
    CONFIG_LOG_MATING,
    CONFIG_LOG_MUTATIONS,
    CONFIG_LOG_SIMULATION,
    CONFIG_READ_FAULTS_FILE,
    CONFIG_NORMALIZED_FITNESS,
    CONFIG_STOP_AT_100,
    CONFIG_MUTATION_LENGTH_GENE,
    CONFIG_MAX_BOOL
};

enum config_int
{
    CONFIG_THREAD_SLOTS = 0,
    CONFIG_MAX_GENERATIONS,
    CONFIG_POPULATION_SIZE,
    CONFIG_FITNESS_TYPE,
    CONFIG_CUT_TYPE,
    CONFIG_MAX_STALL,
    CONFIG_MAX_RETEST,
    CONFIG_CHROMOSOME_NUM,
    CONFIG_CHROMOSOME_MAX_LENGTH,
    CONFIG_CHROMOSOME_START_LEN_MIN,
    CONFIG_CHROMOSOME_START_LEN_MAX,
    CONFIG_MAX_INT
};

enum config_string
{
    CONFIG_CONF_FILENAME = 0,
    CONFIG_LOAD_LOG_FILENAME,
    CONFIG_MAIN_PATH,
    CONFIG_LOG_PATH,
    CONFIG_SIMULATOR_PATH,
    CONFIG_SIMULATOR_BIN,
    CONFIG_SIMULATOR_ARGS,
    CONFIG_TEST_FILE_OUT,
    CONFIG_TEST_FILE_IN,
    CONFIG_THREAD_PREFIX,
    CONFIG_MAX_STRING
};

enum config_float
{
    CONFIG_MATING_FRACTION = 0,
    CONFIG_MATING_RATE,
    CONFIG_MUTATION_RATE,
    CONFIG_MUTATION_STALL_RATE,
    CONFIG_MAX_FLOAT,
};

//! Contiene i parametri di configurazione
/**
 *
 */
class config
{
    bool         config_bool[CONFIG_MAX_BOOL];
    int          config_int[CONFIG_MAX_INT];
    std::string  config_string[CONFIG_MAX_STRING];
    float        config_float[CONFIG_MAX_FLOAT];

    cfg_t* open_cfg();

    public: 
    config();
    //~config();

    //void set_bool_config(config_bool, bool);
    //void set_int_config(config_int, int);
    //void set_string_config(config_string, std::string);
    //void set_float_config(config_string, float);

    bool        get_bool_config(enum config_bool);
    int         get_int_config(enum config_int);
    std::string get_string_config(enum config_string);
    float       get_float_config(enum config_float);

    bool load_config();
    void post_init_config();
    void init_log_profiles();
    void help_config();
    void check_config();

    int  load_args(int argc, char **argv);
    void help_args();
};

#endif

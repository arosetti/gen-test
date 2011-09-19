#ifndef TESTS_H
#define TESTS_H

#include "../common/typedefs.h"
#include "../simulation/simulation.h"
#include <set>

using namespace std;

class general_tests;
class population;

struct thread_params
{
    population* pop;
    general_tests* g_test;
    uint32 sim_id;
};


void *SimulationThread(void *arg);

class tests
{
    public:
        tests();
        tests(const tests&);
        ~tests();

        uint32 GetTestNumber();
        uint32 GetDetectedNumber();

        uint32 GetFaultsSize();
        set<uint32> const& GetFaultsList() const;
        bool   DeleteFault(uint32 fault);
        bool   InsertFault(uint32 fault);
        bool   FindFault(uint32 fault);
        void   EmptyFaults(general_tests* g_test = NULL);

        bool   ExecuteTest(uint32 sim_id, general_tests* g_test = NULL);
        bool   GetFaultsFile(uint32 sim_id, general_tests* g_test = NULL);  // questa va in simulation

        virtual string get_dna() const = 0;
        virtual bool   is_tested() const = 0;
        virtual void   test() = 0;
        virtual void   untest() = 0;
        
    protected:

        uint32 detected;
        uint32 n_tests;

        simulation sim_test;

        set<uint32> m_faults_set;
};

#endif

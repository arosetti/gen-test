#ifndef TESTS_H
#define TESTS_H

#include "../common/typedefs.h"
#include "../simulation/simulation.h"
#include <set>

using namespace std;

class general_tests;

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
 
        void   ExecuteTest(general_tests* g_test = NULL); 
        bool   GetFaultsFile(general_tests* g_test);
 
        virtual string get_dna() const;
        virtual bool   IsEdited() const;
        virtual void   UnsetEdited() {};

    protected:
 
        uint32 detected;
        uint32 n_tests;

        simulation sim_test;

        set<uint32> m_faults_set;        
};

#endif

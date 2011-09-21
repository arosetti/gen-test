#ifndef GEN_TESTS_H
#define GEN_TESTS_H

#include <cstdlib>
#include <string>
#include <sstream>
#include <fstream>
#include <tr1/unordered_map>
#include <map>
#include "../common/typedefs.h"
#include "../ga/individual.h"

#define UNORDERED_MAP std::tr1::unordered_map
typedef UNORDERED_MAP<uint32, uint32> FaultsMap;
typedef std::map<uint32, individual*>  individual_map;

using namespace std;

class general_tests
{
    public:
        general_tests();
        ~general_tests();

        uint32 GetFaultsSize();

        FaultsMap const& GetFaultsMap() const;
        void   EmptyFaults();
        bool   DeleteFault(uint32 fault, uint32 numb = 0);
        bool   InsertFault(uint32 fault, uint32 numb = 1);
        uint32 FindFault(uint32 fault);

        bool   LoadFile(individual_map* pool); // Deprecato

        void  getlock_gen_test()
        {
            pthread_mutex_lock(&mutex_gen_test);
        }
        void  releaselock_gen_test()
        {
            pthread_mutex_unlock(&mutex_gen_test);
        }

    private:

        FaultsMap m_faults_map;
        pthread_mutex_t    mutex_gen_test;
};

#endif

#ifndef GEN_TESTS_H
#define GEN_TESTS_H

#include "../common/typedefs.h"
#include <set>
#include <tr1/unordered_map>
#include <map>

#define UNORDERED_MAP std::tr1::unordered_map

typedef UNORDERED_MAP<uint32, uint32> FaultsMap;

using namespace std;

class general_tests
{
    public:
        general_tests();
        ~general_tests();

        uint32 GetFaultsSize();

        FaultsMap const& GetFaultsMap() const;
        bool   DeleteFault(uint32 fault, uint32 numb = 0);
        bool   InsertFault(uint32 fault, uint32 numb = 1);
        uint32   FindFault(uint32 fault);

    private:

        FaultsMap m_faults_map;
};

#endif

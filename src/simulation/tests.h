#ifndef TESTS_H
#define TESTS_H

#include "../common/typedefs.h"
#include <set>

using namespace std;

class tests
{
    public:
        tests();
        ~tests();

        uint32 GetFaultsSize();

        set<uint32> const& GetFaultsList() const;
        bool   DeleteFault(uint32 fault);
        bool   InsertFault(uint32 fault);
        bool   FindFault(uint32 fault);
        void   EmptyFaults();

    private:

        set<uint32> m_faults_set;
};

#endif

#include "general_tests.h"

general_tests::general_tests()
{

}

general_tests::~general_tests()
{

}

uint32 general_tests::GetFaultsSize()
{
    return m_faults_map.size();
}

FaultsMap const& general_tests::GetFaultsMap() const  
{
    return m_faults_map;
}

bool general_tests::DeleteFault(uint32 fault, uint32 numb)
{
    FaultsMap::iterator itr = m_faults_map.find(fault);
    if (itr == m_faults_map.end())
        return false;

    if (m_faults_map[fault] <= numb || !numb)
        m_faults_map.erase(fault);
    else
        m_faults_map[fault] -= numb;

    return true;
}    

bool general_tests::InsertFault(uint32 fault, uint32 numb)
{
    FaultsMap::iterator itr = m_faults_map.find(fault);
    if (itr != m_faults_map.end())
        m_faults_map[fault] += numb;
    else
        m_faults_map[fault] = numb;

    return true;
}  

uint32 general_tests::FindFault(uint32 fault)
{
    FaultsMap::iterator itr = m_faults_map.find(fault);
    if (itr == m_faults_map.end())
        return 0;

    return m_faults_map[fault];
}    

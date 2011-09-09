#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include "typedefs.h"

class BarGoLink
{
    public: // constructors
        BarGoLink(int row_count);
        BarGoLink(uint32 row_count);
        BarGoLink();
        ~BarGoLink();

    public: // modifiers
        void step();
        static void SetOutputState(bool on);
        void init(int row_count);

    private:       

        static bool m_showOutput;
        static char const * const empty;
        static char const * const full;

        int rec_no;
        int rec_pos;
        int num_rec;
        int indic_len;
};
#endif

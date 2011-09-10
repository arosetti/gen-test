#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include "../typedefs.h"

class prog_bar
{
    public: // constructors
        prog_bar(int row_count);
        prog_bar(uint32 row_count);
        prog_bar();
        ~prog_bar();

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

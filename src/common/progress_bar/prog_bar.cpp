#include <stdio.h>
#include <errno.h>

#include "prog_bar.h"

bool prog_bar::m_showOutput = true;

char const* const prog_bar::empty = " ";
#ifdef _WIN32
char const* const prog_bar::full = "\x3D";
#else
char const* const prog_bar::full = "*";
#endif

prog_bar::prog_bar()
{

}

prog_bar::prog_bar(int row_count)
{
    init(row_count);
}

prog_bar::prog_bar(uint32 row_count)
{
    init((int)row_count);
}

prog_bar::~prog_bar()
{
    if (!m_showOutput)
        return;

    printf( "\n" );
    fflush(stdout);
}

void prog_bar::init(int row_count)
{
    rec_no = 0;
    rec_pos = 0;
    indic_len = 50;
    num_rec = row_count;

    if (!m_showOutput)
        return;

#ifdef _WIN32
    printf( "\x3D" );
#else
    printf( "[" );
#endif
    for ( int i = 0; i < indic_len; i++ ) printf( empty );
#ifdef _WIN32
    printf( "\x3D 0%%\r\x3D" );
#else
    printf( "] 0%%\r[" );
#endif
    fflush(stdout);
}

void prog_bar::step()
{
    if (!m_showOutput)
        return;

    int i, n;

    if ( num_rec == 0 ) return;
    ++rec_no;
    n = rec_no * indic_len / num_rec;
    if ( n != rec_pos )
    {
#ifdef _WIN32
        printf( "\r\x3D" );
#else
        printf( "\r[" );
#endif
        for ( i = 0; i < n; i++ ) printf( full );
        for ( ; i < indic_len; i++ ) printf( empty );
        float percent = (((float)n/(float)indic_len)*100);
#ifdef _WIN32
        printf( "\x3D %i%% \r\x3D", (int)percent);
#else
        printf( "] %i%% \r[", (int)percent);
#endif
        fflush(stdout);

        rec_pos = n;
    }
}

// avoid use inline version because linking problems with private static field
void prog_bar::SetOutputState(bool on)
{
    m_showOutput = on;
}

#include <pthread.h> // boost???
#include "utility.h"
#include "defines.h"

#ifndef GA_ENGINE_H
#define GA_ENGINE_H

class ga_engine
{
    population p;

    pubilic:
    ga_engine();
    ~ga_engine();

    void init();
    void evolve();  // avvia il thread o il ciclo di evoluzione.
}


#endif

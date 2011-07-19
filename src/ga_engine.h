#ifndef GA_ENGINE_H
#define GA_ENGINE_H

#include <pthread.h> // or boost (se usiamo boost per altre cose usiamo anche i thread già che ci siamo)

#include "utility.h"
#include "population.h"



class ga_engine
{
    population p;

    pubilic:
    ga_engine();
    ~ga_engine();

    void init();
    void evolve();  // avvia il thread o il ciclo di evoluzione.
};

#endif

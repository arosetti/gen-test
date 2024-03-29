#ifndef GA_ENGINE_H
#define GA_ENGINE_H

#include <cstdio>
#include <iostream>
#include <sstream>

#include "population.h"
#include "../common/timer/time.h"

using namespace std;

extern config *conf;

//! Coordina le operazioni base dell'algoritmo genetico.
/**
 *
 */

class ga_engine
{
    /*! il numero di generazione */
    uint32 generation;
    /*! puntatore alla popolazione */
    population *pop;

    public:

    //! Construttore - Inizializza il seed random con la funzione srand
    /**
    */
    ga_engine();
    //! Distruttore - elimina la popolazione
    /**
    */
    ~ga_engine();

    //! Crea la popolazione e inizializza gli individui casualmente
    /**
    */
    void init();
    //! Crea la popolazione e inizializza gli individui casualmente
    /**
    */
    void evolve();
};

#endif

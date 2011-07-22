#ifndef GA_ENGINE_H
#define GA_ENGINE_H

#include <cstdio>
#include <iostream>

#include "population.h"

using namespace std;

extern config *conf;

//! La classe ga_engine è il componente principale dell'algoritmo genetico.
/**
 *
 */

class ga_engine
{
    uint32 iteration;
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

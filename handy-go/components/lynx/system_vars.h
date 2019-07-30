#ifndef MY_SYSTEM_VARS_H
#define MY_SYSTEM_VARS_H

#include "myadd.h"

#ifdef MY_SUSIE_VARS
#include "susie_defs.h"
#ifdef SYSTEM_CPP
    #define VAR_PREFIX 
    #include "susie_vars.h"
    #undef VAR_PREFIX
#else
    #define VAR_PREFIX extern
    #include "susie_vars.h"
    #undef VAR_PREFIX
#endif
#endif

#endif

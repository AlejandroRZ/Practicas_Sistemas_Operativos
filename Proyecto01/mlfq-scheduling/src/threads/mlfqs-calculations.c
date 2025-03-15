#include "threads/thread.h"
#include "threads/mlfqs-calculations.h"
#include <stdlib.h>

//Calcula la carga total de todo el sistema
fixpoint_t
mlfqs_calculate_load_avg (fixpoint_t load_avg , int active_threads ) 
{
    //load_avg = (59/60)*load_avg + (1/60)*ready_threads
    load_avg = (load_avg * 59)/ 60 + FP_CONST (active_threads)/ 60;
}

//Mide que tanto tiempo un thread ha utilizado el CPU recientemente
fixpoint_t
mlfqs_calculate_recent_cpu (fixpoint_t recent_cpu , int nice , fixpoint_t load_avg ) 
{   
    // recent_cpu = (2*load_avg)/(2*load_avg + 1) * recent_cpu + nice
    fixpoint_t dividendo = ((fixpoint_t)(((int64_t)(load_avg * 2)) * recent_cpu >> FRACT_BITS));
    fixpoint_t divisor = load_avg* 2 + (1 << FRACT_BITS);
    recent_cpu = ((fixpoint_t)((((int64_t) dividendo) << FRACT_BITS) / divisor)) 
                + (nice << FRACT_BITS);
}

int 
mlfqs_calculate_priority (fixpoint_t recent_cpu , int nice ) 
{
    // priority = PRI_MAX - (recent_cpu / 4) - (nice * 2)
    int new_priority = (int) FP_ROUND (
                           FP_CONST ((PRI_MAX - ((nice) * 2)))-
						              (recent_cpu/ 4));
     if (new_priority > PRI_MAX)
        new_priority = PRI_MAX;
    else if (new_priority < PRI_MIN)
        new_priority = PRI_MIN;

    return new_priority;
}

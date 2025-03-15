#include "threads/thread.h"
#include "threads/mlfqs-calculations.h"
#include <stdlib.h>

fixpoint_t
mlfqs_calculate_load_avg (fixpoint_t load_avg UNUSED, int active_threads UNUSED) 
{
    /* Not yet implemented */
    return 0;
}

fixpoint_t
mlfqs_calculate_recent_cpu (fixpoint_t recent_cpu UNUSED, int nice UNUSED, fixpoint_t load_avg UNUSED) 
{
    /* Not yet implemented */
    return 0;
}

int 
mlfqs_calculate_priority (fixpoint_t recent_cpu UNUSED, int nice UNUSED) 
{
    /* Not yet implemented */
    return 0;
}

#ifndef THREADS_MLFQCALCULATIONS_H_
#define THREADS_MLFQCALCULATIONS_H_

#include "threads/fixpoint.h"

fixpoint_t mlfqs_calculate_load_avg (fixpoint_t load_avg, int active_threads);
fixpoint_t mlfqs_calculate_recent_cpu (fixpoint_t recent_cpu, int nice, fixpoint_t load_avg);
int mlfqs_calculate_priority (fixpoint_t recent_cpu, int nice);

#endif
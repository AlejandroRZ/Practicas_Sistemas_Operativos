#ifndef THREADS_MLFQCALCULATIONS_H_
#define THREADS_MLFQCALCULATIONS_H_

#include "threads/fixpoint.h"

//Calcula la carga total de todo el sistema
fixpoint_t mlfqs_calculate_load_avg (fixpoint_t load_avg, int active_threads);
//Medida que indica que tanto tiempo un thread ha utilizado el cpu recientemente
fixpoint_t mlfqs_calculate_recent_cpu (fixpoint_t recent_cpu, int nice, fixpoint_t load_avg);
//Calcula la prioridad de un thread
int mlfqs_calculate_priority (fixpoint_t recent_cpu, int nice);


#endif
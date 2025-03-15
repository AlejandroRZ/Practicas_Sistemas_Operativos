#ifndef __THREAD_FIXED_POINT_H
#define __THREAD_FIXED_POINT_H

#include <stdint.h>

/* Basic definitions of fixed point. */
typedef int fixpoint_t;
/* 15 LSB used for fractional part. */
#define FRACT_BITS 14
/* Convert a value to fixed-point value.*/ 
#define FP_CONST(A) ((fixpoint_t)(A << FRACT_BITS))
#define F_CONST 16384 // Equivale a 2**14

/*Convierte un entero simple a un numero equivalente en representacion de punto fijo*/
fixpoint_t int_to_fixpoint (int n);
/* Convierte un entero en un número equivalente en representación de punto fijo 17.14 */
fixpoint_t rational_to_fixpoint (int p, int q);
/* Convierte un número en representación de punto fijo 17.14 a un entero truncando su parte decimal */
int fixpoint_to_int (fixpoint_t x);
/* Redondeo */
int fixpoint_round (fixpoint_t x);
/* Multiplica dos números en representación de punto fijo 17.14 */
fixpoint_t fixpoint_mult (fixpoint_t x, fixpoint_t y);
/* Divide dos números en representación de punto fijo 17.14*/
fixpoint_t fixpoint_div (fixpoint_t x, fixpoint_t y);
/* Versión propia para redondear.*/
int FP_ROUND(fixpoint_t x);

#endif /* thread/fixpoint.h */

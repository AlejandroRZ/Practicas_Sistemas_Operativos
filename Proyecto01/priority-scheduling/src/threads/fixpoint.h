#ifndef THREADS_FIXPOINT_H_
#define THREADS_FIXPOINT_H_

#include <stdint.h>

typedef int32_t fixpoint_t;

// int_32 bits
// [0|1] (signo) | ENTERO... 17 bits  | | DECIMAL... 14 bits |

#define FRACT_BITS 14 // Número de bits reservados para la parte decimal
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

#endif

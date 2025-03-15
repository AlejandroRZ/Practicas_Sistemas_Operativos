#include "threads/fixpoint.h"

fixpoint_t 
int_to_fixpoint (int n) {
    return n * F_CONST;
}

fixpoint_t 
rational_to_fixpoint (int p, int q) {
  return int_to_fixpoint(p) / q;
}

int
fixpoint_to_int (fixpoint_t x) {
  return x / F_CONST;
}

int
fixpoint_round (fixpoint_t x) {
  fixpoint_t temp = (x > 0 ? 1 : -1) * (F_CONST / 2);
  return (x + temp) / F_CONST;
}

fixpoint_t
fixpoint_mult (fixpoint_t x, fixpoint_t y) {
  return ((int64_t) x) * y / F_CONST;
}

fixpoint_t 
fixpoint_div (fixpoint_t x, fixpoint_t y) {
  return ((int64_t) x) * F_CONST / y;
}

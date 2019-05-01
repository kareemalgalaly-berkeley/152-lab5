// See LICENSE for license details.

#include "stdlib.h"
#include "dataset.h"

//--------------------------------------------------------------------------
// vvadd function

void __attribute__((noinline)) vvadd(int coreid, int ncores, size_t n, const data_t* x, const data_t* y, data_t* z)
{
   size_t i;

   // force scalar
   ncores = 1;

   // interleave accesses
   for (i = coreid; i < n; i+=ncores)
   {
      z[i] = x[i] + y[i];
   }
}

void __attribute__((noinline)) vvadd_opt(int coreid, int ncores, size_t n, const data_t* x, const data_t* y, data_t* z)
{
  size_t i;
  float min = n * coreid/ncores;
  float max = n * (coreid + 1)/ncores;
  for (i = (int) min; i < (int) max; i++) {
      z[i] = x[i] + y[i];
  }
  // generalized unroll finisher
  //for (; i < max; i++) {
  //    z[i] = x[i] + y[i];
  //}
}

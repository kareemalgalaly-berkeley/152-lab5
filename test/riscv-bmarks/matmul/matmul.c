// See LICENSE for license details.

#include "dataset.h"
#include "util.h"
#include <stddef.h>

// static data_t;

void matmul(const size_t coreid, const size_t ncores, const size_t lda,  const data_t A[], const data_t B[], data_t C[])
{
  size_t i, j, k;

  for (i = 0; i < lda; i++) {
    for (j = coreid; j < lda; j += ncores) {
      data_t sum = 0;
      for (k = 0; k < lda; k++)
        sum += A[j*lda + k] * B[k*lda + i];
      C[i + j*lda] = sum;
    }
  }
}

void matmul_opt(const size_t coreid, const size_t ncores, const size_t lda,  const data_t A[], const data_t B[], data_t C[])
{
    size_t i, j, k;
    
    size_t start_j = lda / ncores * coreid;
    size_t end_j   = (coreid == ncores - 1) ? lda : lda / ncores * (coreid + 1);

//    memset(C, 0, lda*lda);

    for (j = start_j; j < end_j; j++) {
        for (i = 0; i < lda; i++) {
            data_t sum = 0;
            for (k = 0; k < lda; k++)
                sum += A[j*lda + k] * B[k*lda + i];
            C[i + j*lda] = sum;
        }
    }
}


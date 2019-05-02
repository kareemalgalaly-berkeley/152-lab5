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
        for (i = 0; i < lda; i+=8) {
            data_t sum_0 = 0;
            data_t sum_1 = 0;
            data_t sum_2 = 0;
            data_t sum_3 = 0;
            data_t sum_4 = 0;
            data_t sum_5 = 0;
            data_t sum_6 = 0;
            data_t sum_7 = 0;

            for (k = 0; k < lda; k++) {
                sum_0 += A[j*lda + k] * B[k*lda + i];
                sum_1 += A[j*lda + k] * B[k*lda + i+1];
                sum_2 += A[j*lda + k] * B[k*lda + i+2];
                sum_3 += A[j*lda + k] * B[k*lda + i+3];
                sum_4 += A[j*lda + k] * B[k*lda + i+4];
                sum_5 += A[j*lda + k] * B[k*lda + i+5];
                sum_6 += A[j*lda + k] * B[k*lda + i+6];
                sum_7 += A[j*lda + k] * B[k*lda + i+7];
            }
            C[i + j*lda] = sum_0;
            C[i + j*lda+1] = sum_1;
            C[i + j*lda+2] = sum_2;
            C[i + j*lda+3] = sum_3;
            C[i + j*lda+4] = sum_4;
            C[i + j*lda+5] = sum_5;
            C[i + j*lda+6] = sum_6;
            C[i + j*lda+7] = sum_7;
        }
    }
}


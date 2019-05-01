// See LICENSE for license details.

#include "dataset.h"
#include "util.h"
#include <stddef.h>

#define BLOCK_POWER 2

void matmul(const size_t coreid, size_t ncores, const size_t lda,  const data_t A[], const data_t B[], data_t C[])
{
  size_t i, j, k;

  // single thread
  ncores = 1;

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
    size_t num_blocks = 1 << BLOCK_POWER;
    size_t block;
    for (block = coreid; block < num_blocks; block += ncores) {
        matmul_block(BLOCK_POWER, block, lda, A, B, C);
    }
}

void matmul_block(size_t block_power, size_t block_id, const size_t lda,  const data_t A[], const data_t B[], data_t C[]) {
    size_t block_col = block_id % block_power;
    size_t block_row = block_id / block_power;

    // block_k has block_power instance
    size_t block_k = 0;
    size_t block_w = lda / block_power;
    for (; block_k < block_power; block_k++) {
        
        // compute bounds 
        // A shares block_row
        // B shares block_col
        // B eq = B + (block_col + lda * block_k  ) * block_w
        // A eq = A + (block_k   + lda * block_row) * block_w
        //
        // C eq = C + (block_col + lda * block_row) * block_w

        matmul_summing_active(lda, block_w, 
                A + (block_k   + lda * block_row) * block_w,
                B + (block_col + lda * block_k  ) * block_w,
                C + (block_col + lda * block_row) * block_w
                );
    }
}

void matmul_summing_active(const size_t lda, const size_t block_w, const data_t A[], const data_t B[], data_t C[])
{
  size_t i, j, k;

  for (i = 0; i < block_w; i++) {
    for (j = 0; j < block_w; j++) {
      data_t sum = 0;
      for (k = 0; k < block_w; k++)
        sum += A[j*lda + k] * B[k*lda + i];
      C[i + j*lda] += sum;
    }
  }
}

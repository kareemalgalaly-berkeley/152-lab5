// See LICENSE for license details.
////////////////////////////////////////// 

#include "dataset.h"
#include "util.h"
#include <stddef.h>

#define BLOCK_POWER 2
#define SCRATCH_SIZE 16*32 // ncores * subdiv_size 
// we'd do this dynamicaly but this implementation cant

//data_t scratch[SCRATCH_SIZE]; // static scratch since implementation won't let us be dynamic, so we can't have it automatically adjust for cores

void matmul(const size_t coreid, size_t ncores, const size_t lda,  const data_t A[], const data_t B[], data_t C[])
{
  size_t i, j, k;

  // single thread
  //if (coreid == 0) {
  //    ncores = 1;

      for (i = 0; i < lda; i++) {
        for (j = coreid; j < lda; j += ncores) {
          data_t sum = 0;
          for (k = 0; k < lda; k++)
            sum += A[j*lda + k] * B[k*lda + i];
          C[i + j*lda] = sum;
        }
      }
  //}
}
/*
void matmul_opt(const size_t coreid, const size_t ncores, const size_t lda,  const data_t A[], const data_t B[], data_t C[])
{
    size_t num_blocks = 1 << BLOCK_POWER;
    size_t block;
    size_t min = coreid * num_blocks / ncores;
    size_t max = (coreid != ncores-1) ? (coreid + 1) * num_blocks / ncores : num_blocks;
    for (block = min; block < max; block++) {
        matmul_block(BLOCK_POWER, coreid, ncores, block, lda, A, B, C);
    }
    //for (block = 0; block < num_blocks; block++) {
    //    matmul_block(BLOCK_POWER, coreid, ncores, block, lda, A, B, C);        
    //}                                                          
}

void matmul_block(size_t block_power, size_t coreid, size_t ncores, size_t block_id, const size_t lda,  const data_t A[], const data_t B[], data_t C[]) {
    size_t block_col = block_id % block_power;
    size_t block_row = block_id / block_power;

    // block_k has block_power instance
    size_t block_w = lda / block_power;
    
    size_t block_k = 0;
    for (; block_k < block_power; block_k++) {
    //for (size_t block_k = coreid; block_k < block_power; block_k += ncores) {
        
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

  for (j = 0; j < block_w; j++) {
    for (i = 0; i < block_w; i+=8) {
      data_t sum0 = 0;
      data_t sum1 = 0;
      data_t sum2 = 0;
      data_t sum3 = 0;
      data_t sum4 = 0;
      data_t sum5 = 0;
      data_t sum6 = 0;
      data_t sum7 = 0;
      for (k = 0; k < block_w; k++) {
        sum0 += A[j*lda + k] * B[k*lda + i];
        sum1 += A[j*lda + k] * B[k*lda + i+1];
        sum2 += A[j*lda + k] * B[k*lda + i+2];
        sum3 += A[j*lda + k] * B[k*lda + i+3];
        sum4 += A[j*lda + k] * B[k*lda + i+4];
        sum5 += A[j*lda + k] * B[k*lda + i+5];
        sum6 += A[j*lda + k] * B[k*lda + i+6];
        sum7 += A[j*lda + k] * B[k*lda + i+7];
      }
      C[i + j*lda] += sum0;
      C[i + j*lda + 1] += sum1;
      C[i + j*lda + 2] += sum2;
      C[i + j*lda + 3] += sum3;
      C[i + j*lda + 4] += sum4;
      C[i + j*lda + 5] += sum5;
      C[i + j*lda + 6] += sum6;
      C[i + j*lda + 7] += sum7;
    }
  }
  for (; i < block_w; i++) {
    for (j = 0; j < block_w; j++) {
      data_t sum = 0;
      for (k = 0; k < block_w; k++)
        sum += A[j*lda + k] * B[k*lda + i];
      C[i + j*lda] += sum;
    }
  }
}
*/


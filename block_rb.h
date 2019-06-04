/*
 * block_rb.h
 *
 *  Created on: 4 Jun 2019
 *      Author: stefan jaritz
 */

#ifndef BLOCK_RB_H_
#define BLOCK_RB_H_

#include <stdint.h>

// mode of operation:
// we write into the ring buffer sample wise and read block wise
//
// Organisation:
// we have a block of memory
// memory = M[0..M_Size]
// we split the memory into a number of blocks of equal size
// blocks = B[0..B_N]
// block_size = M_Size / B_N
// every block has an index (b_i) and this index can be translated into a memory position
// addr(B[b_i]) = addr(M[b_i*block_size])
// every block contains an fixed amount of samples s_N
// every sample has the same size s_size

typedef struct block_rb {
  const size_t m_size; // memory size in bytes
  const unsigned int b_N; // amount of blocks inside of the memory
  const size_t block_size; // size in bytes of each block
  const size_t s_size; // size of a sample
  const unsigned int s_N; // amounts of samples in a block
  const uint8_t * pM_start; // pointer to the start of the memory
  const uint8_t * pM_end; // pointer to the end of the memory

  // read variables
  struct readVars{
	  unsigned int b_i; // current block index were we read
	  unsigned int am; // amount of blocks ready to read
  } r;

  // write variables
  struct writeVars{
	  uint8_t * pD; // pointer to the next sample to write
	  unsigned int b_i; // current block index were we write
	  unsigned int s_i; // sample index
  } w;
} block_rb_t;

// declares a variable of block_rb_t
#define BLOCK_RB(_name, _mAddr, _m_size, _b_N, _s_size) \
block_rb_t _name = {				\
	.m_size = _m_size,				\
	.b_N = _b_N,					\
	.block_size = _m_size / _b_N,	\
	.s_size = _s_size,				\
	.s_N = _m_size / _b_N / _s_size,\
	.pM_start = _mAddr,				\
	.pM_end = _mAddr + _m_size,		\
	.r = {							\
	  .b_i = 0,						\
	  .am = 0,						\
	},								\
	.w = {							\
	  .pD = _mAddr,					\
	  .b_i = 0,						\
	  .s_i = 0,						\
	}								\
};

// gets the next sample write address
uint8_t * block_rb_getNextSampleWriteAddr(block_rb_t * r);

// gets the amount of blocks ready to read
unsigned int block_rb_getAmountOfBlocksToRead(block_rb_t * r);

// should be called after a sample is written to memory
void block_rb_writeDone(block_rb_t * r);

// reads a block back
uint8_t * block_rb_readBlock(block_rb_t * r);

// resets the ring buffer
void block_rb_reset(block_rb_t * r);

#endif /* BLOCK_RB_H_ */

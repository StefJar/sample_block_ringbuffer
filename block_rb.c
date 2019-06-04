/*
 * block.c
 *
 *  Created on: 4 Jun 2019
 *      Author: stefan jaritz
 */

#include <stddef.h>

#include "block_rb.h"

// gets the next sample write address
uint8_t * block_rb_getNextSampleWriteAddr(block_rb_t * r) {
	return r->w.pD;
}

// gets the amount of blocks ready to read
unsigned int block_rb_getAmountOfBlocksToRead(block_rb_t * r) {
	return r->r.am;
}

// should be called after a sample is written to memory
void block_rb_writeDone(block_rb_t * r) {
	// increment write pointer
	r->w.pD += r->s_size;
	// warp if needed
	if (r->w.pD >= r->pM_end) {
		r->w.pD = (uint8_t *) r->pM_start;
	}
	// increment the amount of samples stored
	r->w.s_i++;
	if (r->w.s_i >= r->s_N) {
		// block boundary check successful
		// we moved form one block to an other
		r->w.s_i = 0; // reset index
		r->r.am++; // tell that one block is ready to read
		r->w.b_i++; // remember that we are now in an other block doing writes
		if (r->w.b_i >= r->b_N) {
			r->w.b_i = 0;
		}
	}
}

// reads a block back
uint8_t * block_rb_readBlock(block_rb_t * r) {
	uint8_t * addr;

	if (!(r->r.am)) return NULL;

	// get address and save it
	addr = (uint8_t *) r->pM_start + (r->r.b_i * r->block_size);

	// advance to next block
	r->r.b_i++;
	if (r->r.b_i >= r->b_N) {
		r->r.b_i = 0;
	}
	// update amount
	r->r.am--;
	return addr;
}

// reset
void block_rb_reset(block_rb_t * r) {
	r->r.b_i = 0;
	r->r.am = 0;
	r->w.pD = r-> pM_start;
	r->w.b_i = 0;
	r->w.s_i = 0;
}

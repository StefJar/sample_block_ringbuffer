/*
 * simple test program that tests the block ring buffer code by writing 4G samples @ 3Bytes into
 * and reading it 83 sample blocks @ 3 Bytes (249 Bytes)
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "block_rb.h"

#define BLOCKS 8
#define ACC_SAMPLE_SIZE 3
#define ACC_SAMPLE_PER_FRAME ((255-4)/ACC_SAMPLE_SIZE)

uint8_t acc_memory[BLOCKS*ACC_SAMPLE_PER_FRAME*ACC_SAMPLE_SIZE];

BLOCK_RB(acc, acc_memory, sizeof(acc_memory), BLOCKS, ACC_SAMPLE_SIZE);

typedef struct uint24{
    uint8_t data[3];
} uint24_t;

uint32_t g_sampleC = 0;

#define UINT24_SET(_s, _v) \
	_s->data[0] = (_v ) & 0xFF; \
	_s->data[1] = (_v >> 8) & 0xFF; \
	_s->data[2] = (_v >> 16) & 0xFF

#define UINT24_GET(_s, _v) \
	_v = _s->data[0] | (_s->data[1] << 8) | (_s->data[2] << 16)

void getSample24bitUint(uint24_t * ps) {
	UINT24_SET(ps, g_sampleC);
	g_sampleC++;
	if (g_sampleC > 0xFFFFFF) {
		g_sampleC = 0;
	}
}


int main(int argc, char **argv) {
	uint24_t * rsb;
	uint32_t v, rv;
	unsigned int j;
	unsigned int msc = 0;

	v = 0;
	for (unsigned int i = 0; i < 0xFFFFFFFF; i++) {
		if (!(i % (1024*1024))) {
			msc++;
			printf("%uM samples processed\n", (unsigned int) msc);
		}
		getSample24bitUint((uint24_t *)block_rb_getNextSampleWriteAddr(&acc));
		block_rb_writeDone(&acc);
		if (block_rb_getAmountOfBlocksToRead(&acc)) {
			rsb = (uint24_t *) block_rb_readBlock(&acc);
			// check block
			for (j = 0; j < acc.s_N; j++) {
				UINT24_GET(rsb, rv);
				if (v != rv) {
					printf("error at %i", i);
					exit(-1);
				}
				rsb++;

				v++;
				if (v > 0xFFFFFF) {
					v = 0;
				}
			}
		}
	}
	return 0;
}

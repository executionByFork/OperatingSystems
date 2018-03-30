#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "bitmap.h"
#include "block_store.h"
// include more if you need

#define BITMAP_SIZE_BYTES 256        // 2^8 blocks.
#define BLOCK_STORE_NUM_BLOCKS 256   // 2^8 blocks.
#define BLOCK_STORE_AVAIL_BLOCKS 255 // Last block consumed by the FBM
#define BLOCK_STORE_NUM_BYTES 65536  // 2^8 blocks of 2^8 bytes.
#define BLOCK_SIZE_BITS 2048         // 2^8 BYTES per block
#define BLOCK_SIZE_BYTES 256         // 2^8 BYTES per block

typedef struct block_store {
	bitmap_t* FBM;
	unsigned char* blocks[BLOCK_STORE_AVAIL_BLOCKS];
} block_store_t;

block_store_t *block_store_create() {

	block_store_t* blockstore = (block_store_t*) malloc(sizeof(block_store_t));
	if (!blockstore) {
		return NULL;
	}

    blockstore->FBM = bitmap_create(BLOCK_STORE_AVAIL_BLOCKS);
    if (!blockstore->FBM) {
		free(blockstore);
		return NULL;
	}

	for (int i = 0; i < BLOCK_STORE_AVAIL_BLOCKS; ++i) {
		//check if the calloc was successful
		if ( !(blockstore->blocks[i] = calloc(1, BLOCK_SIZE_BYTES)) ) {
			for (int j = i-1; j >= 0; j--) {	//if at any point a calloc fails, free all previous calloc
				free(blockstore->blocks[j]);
			}
			//free FBM and blockstore before returning NULL
			free(blockstore->FBM);
			free(blockstore);
			return NULL;
		}
	}

    return blockstore;
}

void block_store_destroy(block_store_t *const bs) {
	if (!bs) {
		return;
	}
	//free every block in the store
	for (int j = 0; j < BLOCK_STORE_AVAIL_BLOCKS; j++) {
		free(bs->blocks[j]);
	}
	//free FBM and blockstore before returning
	bitmap_destroy(bs->FBM);
	free(bs);
	return;
}

size_t block_store_allocate(block_store_t *const bs) {
	if (!bs) {
		return SIZE_MAX;
	}
	//bitmap_set(bs->FBM, 5);
	size_t i;
	if ( (i = bitmap_ffz(bs->FBM)) == SIZE_MAX ) {
		return SIZE_MAX;
	}

	bitmap_set(bs->FBM, i);
	return i;
}

bool block_store_request(block_store_t *const bs, const size_t block_id) {
	if (!bs) {
		return false;
	}
	if (block_id > BLOCK_STORE_AVAIL_BLOCKS) {
		return false;
	}

	if (!bitmap_test(bs->FBM, block_id)) {
		bitmap_set(bs->FBM, block_id);
		return true;
	}

	return false;
}

void block_store_release(block_store_t *const bs, const size_t block_id) {
	if (!bs) {
		return;
	}
	if (block_id >= BLOCK_STORE_AVAIL_BLOCKS) {	//block id will be out of range
		return;
	}

	bitmap_reset(bs->FBM, block_id);
	return;
}

size_t block_store_get_used_blocks(const block_store_t *const bs) {
	if (!bs) {
		return SIZE_MAX;
	}

	return bitmap_total_set(bs->FBM);
}

size_t block_store_get_free_blocks(const block_store_t *const bs) {
	if (!bs) {
		return SIZE_MAX;
	}

	return (BLOCK_STORE_AVAIL_BLOCKS - bitmap_total_set(bs->FBM));
}

size_t block_store_get_total_blocks() {
	return BLOCK_STORE_AVAIL_BLOCKS;
}

size_t block_store_read(const block_store_t *const bs, const size_t block_id, void *buffer) {
	if (!bs) {
		return 0;
	}
	if (!buffer) {
		return 0;
	}
	if (block_id >= BLOCK_STORE_AVAIL_BLOCKS) {	//block id will be out of range
		return 0;
	}

	if (!bitmap_test(bs->FBM, block_id)) {
		return 0;
	}

	if (!memcpy(buffer, bs->blocks[block_id], BLOCK_SIZE_BYTES)) {
		return 0;
	}
	
	return BLOCK_SIZE_BYTES;
}

size_t block_store_write(block_store_t *const bs, const size_t block_id, const void *buffer) {
	if (!bs) {
		return 0;
	}
	if (!buffer) {
		return 0;
	}
	if (block_id >= BLOCK_STORE_AVAIL_BLOCKS) {	//block id will be out of range
		return 0;
	}

	if (!memcpy(bs->blocks[block_id], buffer, BLOCK_SIZE_BYTES);) {
		return 0;
	}
	return BLOCK_SIZE_BYTES;
}

/*
block_store_t *block_store_deserialize(const char *const filename) {

}

size_t block_store_serialize(const block_store_t *const bs, const char *const filename) {

}
*/

#include "../include/bitmap.h"

#include <stdio.h>

// data is an array of uint8_t and needs to be allocated in bitmap_create
//      and used in the remaining bitmap functions. You will use data for any bit operations and bit logic
// bit_count the number of requested bits, set in bitmap_create from n_bits
// byte_count the total number of bytes the data contains, set in bitmap_create


bitmap_t *bitmap_create(size_t n_bits) {
	if (n_bits <= 0) {	//check for invalid bit number
		return NULL;
	}

	// allocate memory space for the bitmap struct
	bitmap_t *bitmap = malloc(sizeof(bitmap_t));

	//calculate number of array indexes needed. Each index holds 8 bits so divide by 8
	int numElems = n_bits/8;
	if (n_bits%8 > 0) {	//If there is a remainder, we need another index to store those bits, so add 1
		numElems += 1;
	}
	bitmap->data = calloc(numElems, sizeof(uint8_t));	//allocate memory for the data array using calloc so memory is initilized to zero
	bitmap->bit_count = n_bits;
	bitmap->byte_count = numElems;

	return bitmap;
}

bool bitmap_set(bitmap_t *const bitmap, const size_t bit) {
	if (bitmap == NULL) {	//check for NULL pointer
		return false;
	}
	if (bit < 0) {	//check for negative bit value
		return false;
	}
	if (bitmap->bit_count < bit) {	//make sure bit value isn't over the bitmap capacity
		return false;
	}

	//calculate index and position of bit to set
	int i = bit/8;
	int pos = bit % 8;
	uint8_t flag = 1;

	//shift the flag over to the appropriate position so the | operator can set the correct bit in the array index
	flag = flag << pos;
	//running the bitwise OR of some data along with a binary value containing only 1 set bit
	//guarantees all positions without the set bit will not be flipped
	bitmap->data[i] |= flag; 

	return true;
}

bool bitmap_reset(bitmap_t *const bitmap, const size_t bit) {	
	if (bitmap == NULL) {	//check for NULL pointer
		return false;
	}
	if (bit < 0) {	//check for negative bit value
		return false;
	}
	if (bitmap->bit_count < bit) {	//make sure bit value isn't over the bitmap capacity
		return false;
	}

	//calculate index and position of bit to zero out
	int i = bit/8;
	int pos = bit % 8;
	uint8_t flag = 1;

	//shift the flag over to the appropriate position so the & operator can set the correct bit in the array index
	flag = flag << pos;
	//running the bitwise AND of some data along with a binary value containing only 1 unset bit
	//guarantees all positions without the set bit will not be flipped
	bitmap->data[i] &= ~flag;	//important that the bitwise not operator is applied to flag variable, so there's 1 unset bit instead of 1 set bit

	return true;
}

bool bitmap_test(const bitmap_t *const bitmap, const size_t bit) {
	if (bitmap == NULL) {	//check for NULL pointer
		return false;
	}
	if (bit < 0) {	//check for negative bit value
		return false;
	}
	if (bitmap->bit_count < bit) {	//make sure bit value isn't over the bitmap capacity
		return false;
	}

	//calculate index and position of bit to check
	int i = bit/8;
	int pos = bit % 8;
	uint8_t flag = 1;

	//shift the flag over to the appropriate position so the & operator can set the correct bit in the array index
	flag = flag << pos;

	//check the bit using the bitwise AND operator
	//using AND on some data along with a binary value containing only 1 unset bit
	//guarantees all positions without the set bit will not be tested
	if (bitmap->data[i] & flag) {	//if >0 then the test bit must have been set
		return true;
	}
	return false;	//if the block above doesnt execute a return, the bit must have been unset
}


/*************************************************************\
| I just wanted to note that is was never stated in the       |
| homework instructions as to which direction you wanted us   |
| to start searching from in the bitmap, when it came to      |
| finding the first set/unset bit. I had written both         |
| functions to search the bitmap starting with the most       |
| significant bit, and it took me a while to realize one of   |
| the tests for bitmap_ffz() was failing because you expected |
| us to search from the LSB end. I just wanted to make you    |
| aware of this so that future homeworks in this course might |
| have clearer instructions.                                  |
\*************************************************************/

size_t bitmap_ffs(const bitmap_t *const bitmap) {
	if (bitmap == NULL) {	//check for NULL pointer
		return SIZE_MAX;
	}

	uint8_t flag;
	int i, pos;
	for (i = 0; i < bitmap->byte_count; i++) {	//run this loop for as many indexes as there are in the bitmap
		flag = 1;
		for (pos = 0; pos < 8; ++pos) {	//runs 8 times, from index 0 to 7, checking each bit in each index of the bitmap
			if ( (8 * i) + pos >= bitmap->bit_count) {	//if trying to read past allocated boundary
				return SIZE_MAX;
			}

			if ( bitmap->data[i] & flag ) {	//if the current bit is found to be set
				//return the address of the first set bit
				//multiply the current index by 8, because there are 8 bits per uint8_t element (floor of i/8)
				//add the position you are testing in the current index (for the remainder in above division)
				return (8 * i) + pos;
			}
			flag = flag << 1;	//if bit isn't set, shift the flag variable and try again
		}
	}

	return SIZE_MAX;	//all allocated bits have been tested, no set bits found
}

size_t bitmap_ffz(const bitmap_t *const bitmap) {
	if (bitmap == NULL) {	//check for NULL pointer
		return SIZE_MAX;
	}

	uint8_t flag;
	int i, pos;
	for (i = 0; i < bitmap->byte_count; i++) {	//run this loop for as many indexes as there are in the bitmap
		flag = 1;
		for (pos = 0; pos < 8; ++pos) {	//runs 8 times, from index 0 to 7, checking each bit in each index of the bitmap
			if ( (8 * i) + pos >= bitmap->bit_count) {	//if trying to read past allocated boundary
				return SIZE_MAX;
			}

			if ( !(bitmap->data[i] & flag) ) {	//if the current bit is found to be unset
				//return the address of the first unset bit
				//multiply the current index by 8, because there are 8 bits per uint8_t element (floor of i/8)
				//add the position you are testing in the current index (for the remainder in above division)
				return (8 * i) + pos;
			}
			flag = flag << 1;	//if bit is set, shift the flag variable and try again
		}
	}

	return SIZE_MAX;	//all allocated bits have been tested, no unset bits found
}

bool bitmap_destroy(bitmap_t *bitmap) {
	if (bitmap == NULL) {	//check for NULL pointer
		return false;
	}

	//need to free data array before the container pointer (bitmap struct)
	//else we'd loose access to the data array pointer and cause a memory leak
	if (bitmap->data) {	//check for NULL pointer
		free(bitmap->data);
	} // should probably print warning if data doesn't exist in a real application

	free(bitmap);
	return true;
}

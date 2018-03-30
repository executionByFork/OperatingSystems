#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/arrays.h"

// LOOK INTO MEMCPY, MEMCMP, FREAD, and FWRITE

bool array_copy(const void *src, void *dst, const size_t elem_size, const size_t elem_count) {
	//check for NULL pointers
	if (src == NULL) {
		return false;
	}
	if (dst == NULL) {
		return false;
	}

	//check array size
	if (elem_size == 0) {
		return false;
	}

	//check array count
	if (elem_count == 0) {
		return false;
	}

	//copy memory from the src pointer into the dst pointer
	//specifically, copy (elem_size * elem_count) bytes
	memcpy(dst, src, elem_size * elem_count);
	return true;
}

bool array_is_equal(const void *data_one, void *data_two, const size_t elem_size, const size_t elem_count) {
	//check for NULL pointers
	if (data_one == NULL) {
		return false;
	}
	if (data_two == NULL) {
		return false;
	}

	//check array size
	if (elem_size == 0) {
		return false;
	}

	//check array count
	if (elem_count == 0) {
		return false;
	}

	//compare memory from data_one and data_two pointers
	//specifically, compare (elem_size * elem_count) bytes of data
	if ( memcmp(data_one, data_two, elem_size * elem_count) == 0 ) {
		return true;	//if memcmp() returns 0, it means the data matches
	}
	return false;	//if memcmp() returns !0 it means the data didn't match
}

ssize_t array_locate(const void *data, const void *target, const size_t elem_size, const size_t elem_count) {
	//check for NULL pointers
	if (data == NULL) {
		return -1;
	}
	if (target == NULL) {
		return -1;
	}

	//check array size
	if (elem_size == 0) {
		return -1;
	}

	//check array count
	if (elem_count == 0) {
		return -1;
	}

	//run loop once for each element, from 0 to (element_count - 1)
	int i;
	for (i = 0; i < elem_count; ++i) {
		//each iteration modifies the data pointer, shifting the address by another elem_size bytes each interation
		//memcmp checks to see if the target data matches the current element in the data array
		//third parameter is the number of bytes to compare
		if ( memcmp(data + (i*elem_size), target, elem_size) == 0 ) {
			return i;	//if the target matches the current element, return the current index
		}
	}
	return -1;	//if no matches
}

bool array_serialize(const void *src_data, const char *dst_file, const size_t elem_size, const size_t elem_count) {
	//check for NULL pointers
	if (src_data == NULL) {
		return false;
	}
	if (dst_file == NULL) {
		return false;
	}

	//check for valid filename
	if (strlen(dst_file) == 0) {	//checks for empty string
		return false;
	}
	int i;
	for (i = 0; i < elem_count; ++i) {	//checks for the occurance of \n in the string
		if (dst_file[i] == '\n') {
			return false;
		}
	}

	//check array size
	if (elem_size == 0) {
		return false;
	}

	//check array count
	if (elem_count == 0) {
		return false;
	}

	//open file with write access
	FILE *fp = fopen(dst_file, "w");
	//write to the file (fp) from the src_data buffer
	//elem_size is the size in bytes of each element
	//elem_count is the number of elements to be written
	size_t r = fwrite(src_data, elem_size, elem_count, fp);
	fclose(fp);	//close the file to avoid memory leaks

	//check r, the return of fwrite(), which shows how many elements were successfully written
	if ( r == elem_count ) {
		return true;	//if the number of elements written matches target number of elements to write
	}
	return false;	//if fwrite() wrote too many or too few elements
}

bool array_deserialize(const char *src_file, void *dst_data, const size_t elem_size, const size_t elem_count) {
	//check for NULL pointers
	if (dst_data == NULL) {
		return false;
	}
	if (src_file == NULL) {
		return false;
	}

	//check for valid filename
	if (strlen(src_file) == 0) {	//checks for empty string
		return false;
	}
	if (src_file[0] == '\n') {	//checks for the occurance of \n in the string
		return false;
	}

	//check array size
	if (elem_size == 0) {
		return false;
	}

	//check array count
	if (elem_count == 0) {
		return false;
	}

	//open file with read access
	FILE *fp = fopen(src_file, "r");
	//readfrom the file (fp) to the dst_data buffer
	//elem_size is the size in bytes of each element
	//elem_count is the number of elements to be read
	size_t r = fread(dst_data, elem_size, elem_count, fp);
	fclose(fp);	//close the file to avoid memory leaks

	//check r, the return of fread(), which shows how many elements were successfully read
	if ( r == elem_count ) {
		return true;	//if the number of elements read matches target number of elements to be read
	}
	return false;	//if fread() read too many or too few elements
}


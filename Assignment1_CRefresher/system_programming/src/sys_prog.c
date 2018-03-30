#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "../include/sys_prog.h"

// LOOK INTO OPEN, READ, WRITE, CLOSE, FSTAT/STAT, LSEEK
// GOOGLE FOR ENDIANESS HELP

bool bulk_read(const char *input_filename, void *dst, const size_t offset, const size_t dst_size) {
	//check for NULL pointers
	if (input_filename == NULL) {
		return false;
	}
	if (dst == NULL) {
		return false;
	}
	if (dst_size <= 0) {	//check for invalid size
		return false;
	}

	//check filename
	int len = strlen(input_filename);
	if (len == 0) {
		return false;	//check for empty string
	}
	int i;
	for (i = 0; i < len; ++i) {	//check all characters in the string for \n
		if (input_filename[i] == '\n') {
			return false;	//exit function if a is \n found
		}
	}

	//open input_filename with read access
	int handle = open(input_filename, O_RDONLY);
	if (handle < 0) {	//check for an opening error, for example if the file doesn't exist
		close(handle);
		return false;
	}

	//adjust the file pointer to the correct offset, using absolute terms
	if (lseek(handle, offset, SEEK_SET) < 0) {
		return false;
	}

	//read from handle, which is now the file pointer plus any offset, into dst
	//dst_size is the number of bytes to be read before truncating data
	int r = read(handle, dst, dst_size);
	close(handle);

	//check return of read() for runtime errors
	if (r > 0) {
		return true;
	}
	return false;
}

bool bulk_write(const void *src, const char *output_filename, const size_t offset, const size_t src_size) {
	//check for NULL pointers
	if (src == NULL) {
		return false;
	}
	if (output_filename == NULL) {
		return false;
	}
	if (src_size <= 0) {	//check for invalid size
		return false;
	}

	//check filename
	int len = strlen(output_filename);
	if (len == 0) {
		return false;	//check for empty string
	}
	int i;
	for (i = 0; i < len; ++i) {	//check all characters in the string for \n
		if (output_filename[i] == '\n') {
			return false;	//exit function if a is \n found
		}
	}

	//open input_filename with write access
	int handle = open(output_filename, O_WRONLY);
	if (handle < 0) {	//check for an opening error, for example if the file doesn't exist
		close(handle);
		return false;
	}

	//adjust the file pointer to the correct offset, using absolute terms
	if (lseek(handle, offset, SEEK_SET) < 0) {
		return false;
	}

	//write to handle, which is now the file pointer plus any offset, from src
	//src_size is the number of bytes to be written before truncating data
	int r = write(handle, src, src_size);
	close(handle);

	//check return of write() for runtime errors
	if ( r > 0 ) {
		return true;
	}
	return false;
}


bool file_stat(const char *query_filename, struct stat *metadata) {
	//check for NULL pointers
	if (query_filename == NULL) {
		return false;
	}
	if (metadata == NULL) {
		return false;
	}

	//run the stat function, which stores the query_filename's metatdata into the metadata variable
	//returns 0 on failure, so check for it
	if ( stat(query_filename, metadata) >= 0) {
		return true;
	}
	return false;
}

bool endianess_converter(uint32_t *src_data, uint32_t *dst_data, const size_t src_count) {
	//check for NULL pointers
	if (src_data == NULL) {
		return false;
	}
	if (dst_data == NULL) {
		return false;
	}
	if (src_count <= 0) {	//check for invalid size
		return false;
	}

	uint32_t data;
	//run for each element to be converted to the opposite endianess,from 0 to (src_count - 1)
	int i;
	for (i = 0; i < src_count; ++i) {
		//copy element from data array into the data variable for manipulation
		data = src_data[i];
		dst_data[i] = ((data >> 24) & 0x000000ff) |	//shifts data 24 bits to the right, and zeros all bits but the last byte
					  ((data >> 8) & 0x0000ff00) |	//shifts data 8 bits to the right, and zeros all bits but the third byte
					  ((data << 8) & 0x00ff0000) |	//shifts data 8 bits to the left, and zeros all bits but the second byte
					  ((data << 24) & 0xff000000);	//shifts data 24 bits to the left, and zeros all bits but the first byte
					  //all pieces are compbined with the bitwise OR operator

					  //Ex: AABBCCDD
					  //	(data >> 24) & 0x000000ff) = 000000AA
					  //	(data >> 8) & 0x0000ff00)  = 0000BB00
					  //	(data << 8) & 0x00ff0000)  = 00CC0000
					  //	(data << 24) & 0xff000000) = DD000000
					  // 000000AA | 0000BB00 | 00CC0000 | DD000000 = DDCCBBAA
	}
	return true;
}


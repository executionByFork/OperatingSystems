#include "../include/allocation.h"
#include <stdlib.h>
#include <stdio.h>


void* allocate_array(size_t member_size, size_t nmember, bool clear) {

	//check params
	if (member_size <= 0 || nmember <= 0) {
		return NULL;
	}

	void* addr;
	if (clear) {
		addr = calloc(nmember, member_size);
	} else {
		addr = malloc(nmember * member_size);
	}
	
	return addr;
}

void* reallocate_array(void* ptr, size_t size) {

	//check params
	if (!ptr) {
		return NULL;
	}
	if (size <= 0) {
		return NULL;
	}

	return realloc(ptr, size);
}

void deallocate_array(void** ptr) {

	if (!ptr || !*ptr) {
		return;
	}

	free(*ptr);
	*ptr = NULL;
}

char* read_line_to_buffer(char* filename) {

	if (!filename) {
		return NULL;
	}

	FILE* fp = fopen(filename, "r");

	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	rewind(fp);

	int mem_size = sizeof(char);
	int nmemb = size/mem_size;

	char* ptr = calloc(nmemb, mem_size);

	fread(ptr, mem_size, nmemb, fp);
	fclose(fp);

	return ptr;
}

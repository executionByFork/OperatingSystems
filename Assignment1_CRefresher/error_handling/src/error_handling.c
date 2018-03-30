#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>


#include "../include/error_handling.h"

int create_blank_records(Record_t **records, const size_t num_records) {

	if (records == NULL) {	//cant dereference if NULL so fail
		return -1;
	}
	if (*records != NULL) {	//records shouldnt be preallocated
		return -1;
	}
	if (num_records == 0) {
		return -1;
	}

	//calloc so memory space is initilized to zero
	*records = (Record_t*) calloc(num_records, sizeof(Record_t));
	if (*records == NULL) {	//if calloc fails
		return -2;
	}

	return 0;
}

int read_records(const char *input_filename, Record_t *records, const size_t num_records) {

	if (input_filename == NULL) {
		return -1;
	}
	if (records == NULL) {
		return -1;
	}
	if (num_records == 0) {
		return -1;
	}

	//open file read only mode
	int fd = open(input_filename, O_RDONLY);
	if (fd < 0) {	//if negitive there was an error
		return -2;
	}

	ssize_t data_read = 0;
	for (size_t i = 0; i < num_records; ++i) {
		data_read = read(fd, &records[i], sizeof(Record_t));
		if (data_read <= 0) {	//read returns negative on error and 0 for EOF
			return -3;	//if data_read <= 0 then there was a read error or too many elements trying to be read out
		}
	}
	return 0;
}

int create_record(Record_t **new_record, const char* name, int age) {

	if (*new_record != NULL) {
		return -1;
	}
	if (!name || strlen(name) >= 50) {
		return -1;
	}
	for (size_t i = 0; i < strlen(name); ++i) {	//checks for the occurance of \n in the string
		if (name[i] == '\n') {
			return -1;
		}
	}

	if (age < 1 || age > 200) {	//age must be between 1 & 200
		return -1;
	}

	//
	*new_record = (Record_t*) malloc(sizeof(Record_t));
	if (*new_record == NULL) {
		return -2;
	}
	
	memcpy((*new_record)->name, name, sizeof(char) * strlen(name));
	(*new_record)->name[MAX_NAME_LEN - 1] = 0;	
	(*new_record)->age = age;
	return 0;

}

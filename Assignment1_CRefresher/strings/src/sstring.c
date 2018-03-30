#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/sstring.h"

bool string_valid(const char *str, const size_t length) {
	if (str == NULL) {	//check for NULL pointer
		return false;
	}
	if (length <= 0) {	//check for invalid length
		return false;
	}

	// only checking for NUL terminator at the end of the string, because I'm
	// interpreting a NUL in the middle of the memory segment as an invalid string
	if (str[length - 1] == '\0') {
		return true;	//if NUL found, string valid
	}
	return false;	//no NUL found
}

char *string_duplicate(const char *str, const size_t length) {
	if (str == NULL) {	//check for NULL pointer
		return NULL;
	}
	if (length <= 0) {	//check for invalid length
		return NULL;
	}
	if (strlen(str) == 0) {	//check for empty string
		return NULL;
	}

	//check for NUL terminator
	if (str[length - 1] != '\0') {
		return NULL;	//no NUL found
	}

	//doublecheck length of string
	if (strlen(str) != length-1) {
		return NULL;
	}

	//malloc for memory which will hold the copied string
	char* dupe = malloc(sizeof(char) * length);
	//copy each character in the array to the dupe array
	int i;
	for (i = 0; i < length; ++i) {
		dupe[i] = str[i];
	}
	return dupe;
}

bool string_equal(const char *str_a, const char *str_b, const size_t length) {
	//check for NULL pointers
	if (str_a == NULL) {
		return false;
	}
	if (str_b == NULL) {
		return false;
	}
	if (length <= 0) {	//check for invalid length
		return false;
	}
	
	//compare the strings str_a and str_b
	//strcmp returns 0 if matching
	if (strcmp(str_a, str_b) == 0) {
		return true;	//if strings match
	}
	return false;	//if no match
}

int string_length(const char *str, const size_t length) {
	if (str == NULL) {	//check for NULL pointer
		return -1;
	}
	if (length <= 0) {	//check for invalid length
		return -1;
	}

	//strlen returns the length of a given string
	return strlen(str);
}

int string_tokenize(const char *str, const char *delims, const size_t str_length, char **tokens, const size_t max_token_length, const size_t requested_tokens) {
	//check for NULL pointers
	if (str == NULL) {
		return 0;
	}
	if (delims == NULL) {
		return 0;
	}
	if (tokens == NULL) {
		return 0;
	}
	if (str_length <= 0) {	//check for invalid length
		return 0;
	}
	if (max_token_length <= 0) {	//check for invalid length
		return 0;
	}
	if (requested_tokens <= 0) {	//check for invalid number of tokens
		return 0;
	}

	//malloc for a memory space to copy const str array into. strtok modifies the pointer it is sent, so you can't send it a const pointer
	char* string = malloc(sizeof(char) * str_length);
	//malloc for token variable, temporary storage before placing into **tokens
	char* tok = malloc(sizeof(char) * max_token_length);
	strcpy(string, str);	//copy the const string to a non const string

	int i;
	for (i = 0; i < requested_tokens; i++) {	//run requested_tokens times
		if (!tokens[i]) {	//if an element in tokens isn't allocated, end function with error
			return -1;
		}
		//strtok breaks a string into tokens by the delimiters sent to it
		//on first run, strtok needs a pointer to a string. On consecutive runs, sending NULL tells it to pick up where it left off
		tok = strtok(
			(i == 0) ? string : NULL,	//if first iteration, send the pointer, if not send NULL
			delims
		);
		if (tok == NULL) {
			break;	// no more tokens to extract
		}
		//save into the tokens array
		strcpy(tokens[i], tok);
	}

	return i;
}

bool string_to_int(const char *str, int *converted_value) {
	//check for NULL pointers
	if (str == NULL) {
		return false;
	}
	if (converted_value == NULL) {
		return false;
	}

	//check for a negative sign in the string
	bool neg = false;
	if (str[0] == '-') {
		neg = true;	//string has a negative representation
	}

	//converts the string to a value
	int val = atoi(str);

	//check for value overflow
	if (neg && val >= 0) {	//if value was meant to be negative but it comes out as positive, the number was over 32 bits
		return false;
	}
	if (!neg && val < 0) {	//if value was meant to be positive but it comes out as negative, the number was over 32 bits
		return false;
	}
	
	//store converted value
	*converted_value = val;
	return true;
}

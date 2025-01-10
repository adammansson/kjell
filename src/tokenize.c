#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>

#include "kjell.h"

static int read_string(token_t *token, char *start)
{
	char *current;

	printf("READ_STRING\n");

	current = start + 1;
	while (*current != '"') {
		if (*current == '\0')
			assert(false);

		current++;
	}

	token->type = TOKEN_STRING;
	token->start = start + 1;
	token->end = current - 1;

	return current - start + 1;
}

static bool isword(char c) {
	return isalpha(c) || c == '.' || c == '/';
}

static int read_word(token_t *token, char *start)
{
	char *current;

	printf("READ_WORD\n");

	current = start;
	while (isword(*current))
		current++;

	token->type = TOKEN_STRING;
	token->start = start;
	token->end = current - 1;

	return current - start;
}

#define KJELL_TOKENIZE_SIZE (4)
token_t *kjell_tokenize(char *input)
{
	size_t buffer_size;
	token_t *buffer;
	char *current;
	size_t offset;
	
	buffer_size = KJELL_TOKENIZE_SIZE;
	buffer = malloc(buffer_size * sizeof(token_t));
	offset = 0;

	current = input;
	while (true) {
		assert(*current >= 0);
		assert(*current < 128);

		if (offset >= buffer_size) {
			buffer_size += KJELL_TOKENIZE_SIZE;
			buffer = realloc(buffer, buffer_size * sizeof(token_t));
		}

		if (*current == '\0') {
			buffer[offset].type = TOKEN_END;
			buffer[offset].start = current;
			buffer[offset].end = current;

			return buffer;
		}

		if (*current == '|') {
			buffer[offset].type = TOKEN_PIPE;
			buffer[offset].start = current;
			buffer[offset].end = current;

			current++;
			offset++;
			continue;
		}

		if (*current == ' ') {
			current++;
			continue;
		}

		if (*current == '"') {
			current += read_string(buffer + offset, current);
			offset++;
			continue;
		}

		if (isword(*current)) {
			current += read_word(buffer + offset, current);
			offset++;
			continue;
		}
	}
}

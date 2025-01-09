#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>

#include "kjell.h"

enum state {
	STATE_NOTHING,
	STATE_WORD,
	STATE_STRING
};

#define KJELL_TOKENIZE_SIZE (4)
token_t *kjell_tokenize(char *input)
{
	int buffer_size;
	token_t *buffer;
	int pos;
	char *token_start;
	char *current;

	enum state state = STATE_NOTHING;

	buffer_size = KJELL_TOKENIZE_SIZE;
	buffer = malloc(buffer_size * sizeof(token_t));
	pos = 0;

	current = input;
	while (true) {
		printf("current = %c\n", *current);

		switch (state)
		{
		case STATE_NOTHING:
			printf("TOKENIZER: NOTHING\n");

			switch (*current)
			{
			case '\0':
				buffer[pos].type = TOKEN_END;
				buffer[pos].start = current;
				buffer[pos].end = current;
				return buffer;

			case ' ':
				break;

			case '|':
				buffer[pos].type = TOKEN_PIPE;
				buffer[pos].start = current;
				buffer[pos].end = current;
				pos++;
				break;

			case '"':
			 	token_start = current;
				state = STATE_STRING;
				break;

			default:
				if (isalpha(*current)) {
					token_start = current;
					state = STATE_WORD;
				}
			}
			
			current++;

			break;
		
		case STATE_WORD:
			printf("TOKENIZER: WORD\n");

			switch (*current)
			{
			case '\0':
			case ' ':
			case '|':
				buffer[pos].type = TOKEN_STRING;
				buffer[pos].start = token_start;
				buffer[pos].end = current - 1;
				pos++;
				
				state = STATE_NOTHING;
				break;
			default:
				current++;
			}

			break;

		case STATE_STRING:
			printf("TOKENIZER: STRING\n");

			switch (*current)
			{
			case '\0':
				assert(false);
				break;

			case '"':
				buffer[pos].type = TOKEN_STRING;
				buffer[pos].start = token_start + 1;
				buffer[pos].end = current - 1;
				pos++;

				state = STATE_NOTHING;
				break;
			}

			current++;
			
			break;

		default:
			assert(false);
		}

		if (pos >= buffer_size) {
			printf("REALLOCATE BUFFER\n");

			buffer_size += KJELL_TOKENIZE_SIZE;
			buffer = realloc(buffer, buffer_size * sizeof(token_t));
		}
	}
}

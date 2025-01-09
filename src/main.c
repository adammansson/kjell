#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include "kjell.h"

#define KJELL_SCAN_SIZE (64)
char *kjell_scan()
{
	int bufsize;
	char *buffer;
	int pos;
	int c;

	bufsize = KJELL_SCAN_SIZE;
	buffer = malloc(bufsize * sizeof(char));
	pos = 0;

	while (true) {
		c = getc(stdin);

		if (c == '\n' || c == EOF) {
			buffer[pos] = '\0';
			return buffer;
		}

		buffer[pos] = c;
		pos++;

		if (pos >= bufsize) {
			bufsize += KJELL_SCAN_SIZE;
			buffer = realloc(buffer, bufsize * sizeof(char));
		}
	}

	return buffer;
}

void print_tokens(token_t *token)
{
	char *c;

	printf("\nALL TOKENS FOUND:\n");
	while (true) {
		printf("token type = %d\n", token->type);

		if (token->type == TOKEN_END) {
			putchar('\n');
			return;
		}

		c = token->start;
		putchar('\'');
		while (c <= token->end) {
			putchar(*c);
			c++;
		}
		printf("'\n");

		token++;
	}
}

void kjell_loop()
{
	char *line;
	token_t *tokens;

	/*
	line = "echo \"quoted mump\" pog | ls";
	tokens = kjell_tokenize(line);
	print_tokens(tokens);
	*/

	while (true) {
		line = kjell_scan();
		printf("'%s'\n", line);
		tokens = kjell_tokenize(line);
		print_tokens(tokens);

		free(line);
		free(tokens);
	}
}

int main(void)
{
	kjell_loop();

	return 0;
}

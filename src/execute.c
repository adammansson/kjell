#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>

#include "kjell.h"

static char **create_args_from_tokens(token_t *start, token_t *end)
{
	size_t buffer_size;
	char **buffer;
	size_t arg_size;
	char *arg;
	int i;

	buffer_size = end - start + 2;
	buffer = malloc(buffer_size * sizeof(char *));
	buffer[buffer_size - 1] = NULL;

	for (i = 0; i < buffer_size - 1; i++) {
		arg_size = start[i].end - start[i].start + 2;
		arg = malloc(arg_size * sizeof(char *));
		arg[arg_size - 1] = '\0';
		memcpy(arg, start[i].start, arg_size - 1);

		buffer[i] = arg;
	}

	return buffer;
}

int kjell_execute(token_t *tokens)
{
	token_t *start;
	token_t *end;
	char *c;
	char **args;

	int ifd;
	int ofd;

	start = tokens;
	ifd = STDIN_FILENO;
	ofd = STDOUT_FILENO;

	while (true) {
		printf("TOKEN TYPE = %d\n", start->type);
		end = start;

		while (end->type == TOKEN_STRING)
			end++;

		args = create_args_from_tokens(start, end - 1);
		/*
		while (*args != NULL) {
			printf("arg = %s\n", *args);
			args++;
		}
		*/

		if (end->type == TOKEN_PIPE) {
			printf("EXECUTE PIPE\n");
		}

		if (end->type == TOKEN_END) {
			printf("EXECUTE END\n");

			ofd = STDOUT_FILENO;
		}

		printf("ifd = %d\n", ifd);
		printf("ofd = %d\n", ofd);

		printf("\nOUTPUT: \n");
		execvp(args[0], args);

		if (end->type == TOKEN_END)
			return 0;

		start = end + 1;
	}

	return 0;
}
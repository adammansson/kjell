#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

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

#define KJELL_PARSE_SIZE (4)
#define KJELL_PARSE_DELIM (" \t")
char **kjell_parse(char *line)
{
	int bufsize;
	char **buffer;
	int pos;
	char *token;

	bufsize = KJELL_PARSE_SIZE;
	buffer = malloc(bufsize * sizeof(char *));

	pos = 0;
	token = strtok(line, KJELL_PARSE_DELIM);
	while (token != NULL) {
		buffer[pos] = token;
		pos++;

		if (pos >= bufsize) {
			bufsize += KJELL_PARSE_SIZE;
			buffer = realloc(buffer, bufsize * sizeof(char *));
		}
		
		token = strtok(NULL, KJELL_PARSE_DELIM);
	}

	buffer[pos] = NULL;

	return buffer;
}

int kjell_launch(char **args)
{
	int cid;

	cid = fork();

	if (cid == 0) {
		if (execvp(*args, args) == -1)
			perror("kjell");

		exit(EXIT_FAILURE);
	} else
		wait(NULL);

	return 0;
}

int kjell_execute(char **args)
{
	if (args[0] == NULL)
		return 1;

	if (strcmp(args[0], "exit") == 0)
		return 1;

	if (strcmp(args[0], "cd") == 0) {
		if (args[1] == NULL) {
			if (chdir("/home/adam") == -1)
				perror("kjell");
		} else {
			if (chdir(args[1]) == -1)
				perror("kjell");
		}
		return 0;
	}

	return kjell_launch(args);
}

void kjell_loop()
{
	char *line;
	char **args;
	int status;

	status = 0;
	while (!status) {
		line = kjell_scan();
		args = kjell_parse(line);
		status = kjell_execute(args);

		free(line);
		free(args);
	}
}

int main(void)
{
	kjell_loop();

	return 0;
}

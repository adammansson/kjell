typedef enum token_type {
	TOKEN_END,
	TOKEN_STRING,
	TOKEN_PIPE,
	TOKEN_REDIRECT
} token_type_t;

typedef struct token {
	token_type_t type;
	char *start;
	char *end;
} token_t;

token_t *kjell_tokenize(char *input);

int kjell_execute(token_t *tokens);
#include "request_header.h"
#include "parse.h"
#include "y.tab.h"
#include "lex.yy.h"

/**
* Given a char buffer returns the parsed request headers
*/
Request *parse(char *buffer, int size, int socketFd)
{
	//Differant states in the state machine
	// fprintf(stdout, "parser1: length=%i\n", size);
	// print_detail2(buffer, size);
	enum
	{
		STATE_START = 0,
		STATE_CR,
		STATE_CRLF,
		STATE_CRLFCR,
		STATE_CRLFCRLF
	};

	int i = 0, state;
	size_t offset = 0;
	char ch;
	char buf[8192];
	memset(buf, 0, 8192);

	state = STATE_START;
	while (state != STATE_CRLFCRLF)
	{
		char expected = 0;

		if (i == size)
			break;

		ch = buffer[i++];
		buf[offset++] = ch;

		switch (state)
		{
		case STATE_START:
		case STATE_CRLF:
			expected = '\r';
			break;
		case STATE_CR:
		case STATE_CRLFCR:
			expected = '\n';
			break;
		default:
			state = STATE_START;
			continue;
		}

		if (ch == expected)
			state++;
		else
			state = STATE_START;
	}

	//Valid End State
	if (state == STATE_CRLFCRLF)
	{
		yyscan_t scanner;
		YY_BUFFER_STATE yystate;
		if (yylex_init(&scanner)) {
			return 0;
		}
		yystate = yy_scan_string(buf, scanner);
		fprintf(stdout, "parse: %s\n", buf);
		Request *request = (Request *)malloc(sizeof(Request));
		request->header_count = 0;
		//TODO You will need to handle resizing this in parser.y
		request->headers = (Request_header *)malloc(sizeof(Request_header) * 1);
		// set_parsing_options(buf, i, request);

		if (yyparse(request, scanner) == 0)
		{
			yy_delete_buffer(yystate, scanner);
			yylex_destroy(scanner);
			return request;
		} else {
			free(request->headers);
			request->headers = NULL;
			free(request);
			request = NULL;
			yy_delete_buffer(yystate, scanner);
			yylex_destroy(scanner);
		}
	}
	//TODO Handle Malformed Requests
	printf("Parsing Failed\n");
	return NULL;
}

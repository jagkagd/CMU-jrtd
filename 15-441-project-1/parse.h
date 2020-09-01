#ifndef __PARSE_H__
#define __PARSE_H__

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "request_header.h"
#include "y.tab.h"
#include "lex.yy.h"
#include "global.h"

//Header field

int yyparse(Request *request, yyscan_t scanner);

Request* parse(char *buffer, int size, int socketFd);

#endif
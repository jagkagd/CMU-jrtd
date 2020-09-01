CC=gcc
CFLAGS=-I.
DEPS = global.h log.h request_header.h lex.yy.h y.tab.h parse.h liso_server.h utils.h client_pool.h handle_HTTP_request.h
OBJ = y.tab.o lex.yy.o parse.o example.o
FLAGS = -g -Wall

default:all

all: example liso_server echo_client

lex.yy.c: lexer.l
	flex $^

y.tab.c: parser.y lex.yy.c
	bison -d parser.y

%.o: %.c $(DEPS)
	$(CC) $(FLAGS) -c -o $@ $< $(CFLAGS)

example: $(OBJ)
	$(CC) -g -o $@ $^ $(CFLAGS)

liso_server:
	$(CC) liso_server.c log.c utils.c client_pool.c parse.c y.tab.c lex.yy.c handle_HTTP_request.c -g -o liso_server ## -Wall -Werror

echo_client:
	$(CC) echo_client.c -o echo_client -Wall -Werror

clean:
	rm -f *~ *.o example lex.yy.c lex.yy.h y.tab.c y.tab.h liso_server echo_client

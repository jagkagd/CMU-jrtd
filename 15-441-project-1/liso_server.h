#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "parse.h"
#include "utils.h"
#include "client_pool.h"

#define LISO_PORT 9999
#define BUF_SIZE 4096

int close_socket(int sock);

int close_all_clients(int *clients);

int build_client_connect(int sock, int *clients);

int fd_set_init(fd_set *fd_read, int sock, int* clients);

int handle_client_recv(int client_sock);

int recv_trmn(char *buf, int len);
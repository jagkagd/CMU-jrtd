#ifndef __CLIENT_POOL_H__
#define __CLIENT_POOL_H__

#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/select.h>
#include "request_header.h"

#define MAX_CLIENTS 1024
#define NO_SOCKET -1
#define TRMN_REQUEST "\r\n\r\n"
#define BUF_SIZE 4096


int close_socket(int sock);

int close_all_clients(int *clients);

int print_clients(int *clients);

int init_clients(int *clients);

int build_client_connect(int sock, int *clients);

int fd_set_init(fd_set *fd_read, int sock, int *clients);

int handle_client_recv(int client_sock);

int recv_trmn(char *buf, int readret);

#endif
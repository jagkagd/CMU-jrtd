#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <time.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/select.h>

const char* logfile;
FILE *fp;

int init_log();

void get_time(char *time);

void get_ipadress(int sockfd, char *ip_addr, uint *port);

void dump_error(int sockfd, const char *msg, ...);

void dump_common(int sockfd, const char *request, int status_code, int size);

void close_log();

#endif
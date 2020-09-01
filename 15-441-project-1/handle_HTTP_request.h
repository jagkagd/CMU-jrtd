#ifndef __HANDLE_HTTP_REQUEST_H__
#define __HANDLE_HTTP_REQUEST_H__

#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <time.h>
#include "log.h"
#include "request_header.h"
#include "global.h"

#define HTTP_VERSION "HTTP/1.1"
#define SERVER_NAME "liso/1.0"

typedef struct {
    int code;
    char fp[MAX_BUF];
} Response_info;

int handle_HTTP_request(int sockfd, Request* request);

void get_head_status_code(Request* request, Response_info* info);
void get_get_status_code(Request* request, Response_info* info);
void get_post_status_code(Request* request, Response_info* info);

void get_status_code(Request* request, Response_info* info);

void get_head_status_code(Request* request, Response_info *info);

void get_get_status_code(Request* request, Response_info *info);

int check_source_match(Request* request);

int check_required_length(Request* request);

int check_http_version(Request* request);

void get_header_field(Request* request, const char* header, char* value);

void get_source_path(Request* request, char* fp);

int get_content_length(const char* fp);

#endif
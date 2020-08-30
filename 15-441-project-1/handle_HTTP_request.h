#include "log.h"
#include "request_header.h"
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define MAX_BUF 4096

void get_head_status_code(Request* request, struct response_info* info);
void get_get_status_code(Request* request, struct response_info* info);
void get_post_status_code(Request* request, struct response_info* info);

void get_status_code(Request* request, struct response_info* info);

void get_head_status_code(Request* request, struct response_info *info);

void get_get_status_code(Request* request, struct response_info *info);

int check_source_match(Request* request);

int check_required_length(Request* request);

int check_http_version(Request* request);

void get_header_field(Request* request, const char* header, char* value);

int get_souce_path(Request* request, char* fp);
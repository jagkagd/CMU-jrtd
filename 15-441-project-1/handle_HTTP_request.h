#include "log.h"
#include "request_header.h"
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_BUF 4096

int get_head_status_code(Request *request);
int get_get_status_code(Request *request);
int get_post_status_code(Request *request);

int get_status_code(Request *request)
{
    if (strcmp(request->http_method, "HEAD") == 0)
    {
        return get_head_status_code(request);
    }
    else if (strcmp(request->http_method, "GET") == 0)
    {
        return get_get_status_code(request);
    }
    else if (strcmp(request->http_method, "POST") == 0)
    {
        return get_post_status_code(request);
    }
    else
    {
        return 501;
    }
}

int get_head_status_code(Request *request){
    if(check_required_length(request) != 0){
        return 411;
    } else if(check_http_version(request) != 0){
        return 505;
    }
}

int get_get_status_code(Request *request){
    if(check_required_length(request) != 0){
        return 411;
    } else if(check_http_version(request) != 0){
        return 505;
    } else if(check_source_match())
}

int check_source_match(Request *request){
    char fp[MAX_BUF];
    get_source_path(request, fp);
    return access(fp, F_OK);
}

int check_required_length(Request *request){
    if(strcmp(get_header_field(request, "Content-Length"), "") == 0){
        return -1;
    } else {
        return 0;
    }
}

int check_http_version(Request *request){
    if(atof(request->http_version) > 1.1){
        return -1;
    } else {
        return 0;
    }
}

char* get_header_field(Request* request, const char* header){
    int i;
    for(i = 0; i < request->header_count; i++){
        if(strcmp(request->headers[i].header_name, header) == 0){
            return request->headers[i].header_value;
        }
    }
    if(i == request->header_count){
        return "";
    }
}

int get_souce_path(Request *request, char *fp){
    strcat(fp, request->http_uri);
    if(is_dir(fp)){
        return 0;
    } else {
        return -1;
    }
}
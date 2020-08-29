#include "handle_HTTP_request.h"

const char *http_version = "HTTP/1.1";

struct response_info{
    int code;
    char fp[MAX_BUF];
    int header_count;
    struct response_header response_headers[MAX_BUF];
};

struct response_header{
    char key[256];
    char value[MAX_BUF];
};

void get_status_code(Request *request, struct response_info* info)
{
    if (strcmp(request->http_method, "HEAD") == 0)
    {
        get_head_status_code(request, info);
    }
    else if (strcmp(request->http_method, "GET") == 0)
    {
        get_get_status_code(request, info);
    }
    else if (strcmp(request->http_method, "POST") == 0)
    {
        get_post_status_code(request, info);
    }
    else
    {
        info->code = 501;
    }
}

void get_head_status_code(Request *request, struct response_info* info)
{
    if (check_required_length(request) != 0)
    {
        info->code = 411;
    }
    else if (check_http_version(request) != 0)
    {
        info->code = 505;
    }
    else
    {
        info->code = 200;
    }
}

void get_get_status_code(Request *request, struct response_info* info)
{
    if (check_source_match(request) != 0)
    {
        info->code = 404;
    }
    else if (check_required_length(request) != 0)
    {
        info->code = 411;
    }
    else if (check_http_version(request) != 0)
    {
        info->code = 505;
    }
    else
    {
        info->code = 200;
        info->entity = ;
    }
}

void get_post_status_code(Request *request, struct response_info* info)
{
    if (check_required_length(request) != 0)
    {
        info->code = 411;
    }
    else if (check_http_version(request) != 0)
    {
        info->code = 505;
    }
    else if (check_source_match(request) != 0)
    {
        info->code = 404;
    }
    else
    {
        info->code = 200;
    }
}

int check_source_match(Request *request)
{
    char fp[MAX_BUF];
    get_source_path(request, fp);
    return access(fp);
}

int check_required_length(Request *request)
{
    return (strcmp(get_header_field(request, "Content-Length"), "") == 0);
}

int check_http_version(Request *request)
{
    return atof(request->http_version) > 1.1;
}

char *get_header_field(Request *request, const char *header)
{
    int i;
    for (i = 0; i < request->header_count; i++)
    {
        if (strcmp(request->headers[i].header_name, header) == 0)
        {
            return request->headers[i].header_value;
        }
    }
    if (i == request->header_count)
    {
        return "";
    }
}

int get_souce_path(Request *request, char *fp)
{
    strcat(fp, request->http_uri);
}

int handle_response_info(int sockfd, struct response_info* info){
    char response[1024];
    switch(info->code){
        case 200:
        
        break;
        case 404:
        fprintf(response, "%s 404 Bad request\r\n", http_version);
        break;
        case 411:
        fprintf(response, "%s 411 Length Required\r\n", http_version);
        break;
        case 501:
        fprintf(response, "%s 501 Not Implemented\r\n\r\n", http_version);
        break;
        case 505:
        fprintf(response, "%s 505 HTTP Version not supported\r\n", http_version);
    }
}
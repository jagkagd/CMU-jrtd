#include "handle_HTTP_request.h"

int handle_HTTP_request(int sockfd, Request *request)
{
    Response_info info;
    get_status_code(request, &info);
    handle_response_info(sockfd, request, &info);
}

void get_status_code(Request *request, Response_info *info)
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

void get_head_status_code(Request *request, Response_info *info)
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

void get_get_status_code(Request *request, Response_info *info)
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
    }
}

void get_post_status_code(Request *request, Response_info *info)
{
    if (!check_required_length(request))
    {
        info->code = 411;
    }
    else if (!check_http_version(request))
    {
        info->code = 505;
    }
    else if (!check_source_match(request))
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
    return access(fp, R_OK);
}

int check_required_length(Request *request)
{
    char *value[MAX_BUF];
    get_header_field(request, "Content-Length", value);
    return (strcmp(value, "") != 0);
}

int check_http_version(Request *request)
{
    return atof(request->http_version) > 1.1;
}

void get_header_field(Request *request, const char *header, char *value)
{
    int i;
    for (i = 0; i < request->header_count; i++)
    {
        if (strcmp(request->headers[i].header_name, header) == 0)
        {
            strcpy(value, request->headers[i].header_value);
        }
    }
    if (i == request->header_count)
    {
        strcpy(value, "");
    }
    return;
}

void get_source_path(Request *request, char *fp)
{
    strcat(fp, request->http_uri);
}

void handle_response_info(int sockfd, Response_info *info, Request *request)
{
    char response[MAX_BUF];
    switch (info->code)
    {
    case 200:
        if (strcmp(request->http_method, "POST") == 0)
        {
            fprintf(response, HTTP_VERSION" 200 OK\r\n");
        }
        else
        {
            char mime_type[64];
            get_mime_type(info->fp, mime_type);
            int content_length = get_content_length(info->fp);
            char curr_time[256], last_modified_time[256];
            get_curr_time(curr_time);
            get_last_modified_time(info->fp, last_modified_time);
            char connection_type[16];
            get_connection_type(request, connection_type);
            fprintf(response,
                    HTTP_VERSION
                    " 200 OK\r\n"
                    "Server: "SERVER_NAME"\r\n"
                    "Date: %s\r\n"
                    "Content-Length: %s\r\n"
                    "Content-type: %s\r\n"
                    "Last-modified: %s\r\n"
                    "Connection: %s\r\n"
                    "\r\n",
                    curr_time,
                    content_length,
                    mime_type,
                    last_modified_time,
                    connection_type);
            send_response(sockfd, response, strlen(response));
            if (strcmp(request->http_method, "GET") == 0)
            {
                send_file(sockfd, info->fp);
            }
        }
        break;
    case 404:
        fprintf(response, HTTP_VERSION" 404 Bad request\r\n");
        break;
    case 411:
        fprintf(response, HTTP_VERSION" 411 Length Required\r\n");
        break;
    case 501:
        fprintf(response, HTTP_VERSION" 501 Not Implemented\r\n\r\n");
        break;
    case 505:
        fprintf(response, HTTP_VERSION" 505 HTTP Version not supported\r\n");
    }
}

void get_mime_type(char *fp, char *mime_type)
{
    char *mime = strrchr(fp, '.');
    if (mime == NULL)
    {
        strcpy(mime_type, "application/octet-stream");
        return;
    }
    if (!strcmp(mime, "html"))
    {
        strcpy(mime_type, "text/html");
    }
    else if (!strcmp(mime, "css"))
    {
        strcpy(mime_type, "text/css");
    }
    else if (!strcmp(mime, "png"))
    {
        strcpy(mime_type, "image/png");
    }
    else if (!strcmp(mime, "jpeg"))
    {
        strcpy(mime_type, "image/jpeg");
    }
    else if (!strcmp(mime, "gif"))
    {
        strcpy(mime_type, "image/gif");
    }
    return;
}

void get_connection_type(Request *request, char *connection_type)
{
    strcpy(connection_type, request->http_method);
    return;
}

int send_response(int sockfd, char *response, int response_len)
{
    if (send(sockfd, response, response_len, 0) != response_len)
    {
        close_socket(sockfd);
        fprintf(stderr, "error sending to client.\n");
        return EXIT_FAILURE;
    }
}

int send_file(int sockfd, char *fp)
{
    int fd = open(fp, O_RDONLY);
    if (fd < 0)
    {
        dump_error(sockfd, "Error when opening file %s", fp);
        return -1;
    }
    size_t file_len = get_content_length(fp);
    ssize_t ret = sendfile(sockfd, fd, NULL, file_len);
    if (ret == -1)
    {
        dump_error(sockfd, "Error send file %s", fp);
    }

    return 0;
}

int get_content_length(const char *fp){
    struct stat st;
    stat(fp, &st);
    return st.st_size;
}

void get_curr_time(char *time_str){
    time_t rawtime;
    struct tm * timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    // Sun, 06 Nov 1994 08:49:37 GMT
    strftime(time_str, 256, "%a, %d %b %Y %T %Z", timeinfo);
}

void get_last_modified_time(const char*path, char *time_str) {
  struct stat st;
  struct tm *timeinfo;
  stat(path, &st);
  timeinfo = gmtime(&st.st_mtime);
  strftime(time_str, 256, "%a, %d %b %Y %T %Z", timeinfo);
}
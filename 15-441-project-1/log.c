#include "log.h"

int init_log(){
    if((fp = fopen(logfile, "a")) == NULL){
        fprintf(stderr, "[ERROR] log file can not be created!");
        return EXIT_FAILURE;
    }
    char *time_fmt;
    get_time(time_fmt);
    fprintf(fp, "Liso %s", time_fmt);
    return EXIT_SUCCESS;
}

void get_ipadress(int sockfd, char *ip_addr, uint *port){
    struct sockaddr_in cli_addr;
    socklen_t addr_len;
    getpeername(sockfd, (struct sockaddr*)&cli_addr, &addr_len);
    inet_ntop(AF_INET, &cli_addr.sin_addr, ip_addr, INET_ADDRSTRLEN*sizeof(char));
    *port = ntohs(cli_addr.sin_port);
}

void get_time(char *time_str) {
    time_t rawtime;
    struct tm * timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(time_str, 100, "%d/%b/%Y:%T %z", timeinfo);
}

void dump_error(int sockfd, const char *msg, ...){
    char *time_fmt;
    get_time(time_fmt);
    char ip_addr[INET_ADDRSTRLEN];
    uint port;
    get_ipadress(sockfd, ip_addr, &port);
    va_list(args);
    va_start(args, msg);
    char error_msg[4096];
    fprintf(error_msg, msg, args);
    va_end(args);
    fprintf(fp, "[%s] [error] [client %s:%d] %s", time_fmt, ip_addr, port, error_msg);
}

void dump_common(int sockfd, const char *request, int status_code, int size){
    char *time_fmt;
    get_time(time_fmt);
    char ip_addr[INET_ADDRSTRLEN];
    uint port;
    get_ipadress(sockfd, ip_addr, &port);
    fprintf(fp, "%s:%d - [%s] %s %d %d", ip_addr, port, time_fmt, request, status_code, size);
}

void close_log(){
    close(fp);
}
#include "client_pool.h"

const char *BAD_REQUEST_RESPONSE = "HTTP/1.1 400 Bad Request\r\n";

int close_socket(int sock)
{
    if (close(sock))
    {
        fprintf(stderr, "Failed closing socket.\n");
        return 1;
    }
    return 0;
}

int close_all_clients(int *clients)
{
    int i;
    for (i = 0; i < MAX_CLIENTS; i++)
    {
        if (clients[i] != NO_SOCKET)
        {
            if (close_socket(clients[i]))
            {
                return 1;
            }
        }
    }
    return 0;
}

int print_clients(int *clients)
{
    int i;
    for (i = 0; i < 5; i++)
    {
        fprintf(stdout, "clients[%i] = %i\n", i, clients[i]);
    }
    return 0;
}

int init_clients(int *clients)
{
    int i;
    for (i = 0; i < MAX_CLIENTS; i++)
    {
        clients[i] = NO_SOCKET;
    }
    return 0;
}

int build_client_connect(int sock, int *clients)
{
    int client_sock;
    struct sockaddr_in cli_addr;
    socklen_t cli_size = sizeof(cli_addr);
    if ((client_sock = accept(sock, (struct sockaddr *)&cli_addr, &cli_size)) == -1)
    {
        close_socket(sock);
        fprintf(stderr, "Error accept.\n");
        return EXIT_FAILURE;
    }
    fprintf(stdout, "Connect to %i.\n", client_sock);
    int i;
    for (i = 0; i < MAX_CLIENTS; i++)
    {
        if (clients[i] == NO_SOCKET)
        {
            clients[i] = client_sock;
            break;
        }
    }
    if (i >= MAX_CLIENTS)
    {
        fprintf(stderr, "Too many clients.\n");
        return EXIT_FAILURE;
    }
    return 0;
}

int fd_set_init(fd_set *fd_read, int sock, int *clients)
{
    int max_fd = sock;
    FD_ZERO(fd_read);
    FD_SET(sock, fd_read);
    int i;
    for (i = 0; i < MAX_CLIENTS; i++)
    {
        if (clients[i] != NO_SOCKET)
        {
            FD_SET(clients[i], fd_read);
            // fprintf(stdout, "set clients %i\n", clients[i]);
            if (clients[i] > max_fd)
            {
                max_fd = clients[i];
            }
        }
    }
    return max_fd;
}

int handle_client_recv(int client_sock)
{
    // fprintf(stdout, "[%i] handle.\n", client_sock);
    ssize_t readret = 0;
    char buf[BUF_SIZE];
    memset(buf, 0, BUF_SIZE);
    if ((readret = recv(client_sock, buf, BUF_SIZE, 0)) == -1)
    {
        // fprintf(stdout, "[%i] Received lenth = %li \nmsg: %s\n", client_sock, readret, buf);
        // if (recv_trmn(buf, readret))
        // {
        //     break;
        // }
        close_socket(client_sock);
        fprintf(stderr, "Error reading from client socket.\n");
        return EXIT_FAILURE;
    }
    if (readret == 0)
    {
        return 0;
    }
    fprintf(stdout, "[%i] Receive:\n", client_sock);
    print_detail(buf, readret);
    fprintf(stdout, "\n");
    Request *request = (Request *)parse(buf, readret, client_sock);
    if (request == NULL)
    {
        size_t len = strlen(BAD_REQUEST_RESPONSE);
        if (send(client_sock, BAD_REQUEST_RESPONSE, len, 0) != len)
        {
            close_socket(client_sock);
            fprintf(stderr, "Error sending to client.\n");
            return EXIT_FAILURE;
        }
        fprintf(stdout, "[%i] Send:\n", client_sock);
        print_detail(BAD_REQUEST_RESPONSE, len);
    }
    else
    {
        // buf[readret] = '\0';
        if (send(client_sock, buf, readret, 0) != readret)
        {
            close_socket(client_sock);
            fprintf(stderr, "error sending to client.\n");
            return EXIT_FAILURE;
        }
        fprintf(stdout, "[%i] Send:\n", client_sock);
        print_detail(buf, readret);
    }
    memset(buf, 0, BUF_SIZE);
    fprintf(stdout, "\n");

    return 0;
}

int recv_trmn(char *buf, int readret)
{
    int i;
    for (i = 0; i < 4; i++)
    {
        if (buf[readret - 4 + i] != TRMN_REQUEST[i])
        {
            return 0;
        }
    }
    return 1;
}
#include "liso_server.h"

int main(int argc, char *argv[])
{
    int sock;
    int clients[MAX_CLIENTS];
    struct sockaddr_in addr;
    fd_set fd_read;
    int max_fd, ret;

    fprintf(stdout, "----- Liso Server -----\n");

    init_clients(clients);

    /* all networked programs must create a socket */
    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        fprintf(stderr, "Failed creating socket.\n");
        return EXIT_FAILURE;
    }
    fprintf(stdout, "at socket: %i\n", sock);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(LISO_PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    /* servers bind sockets to ports---notify the OS they accept connections */
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)))
    {
        close_socket(sock);
        fprintf(stderr, "Failed binding socket.\n");
        return EXIT_FAILURE;
    }

    if (listen(sock, MAX_CLIENTS))
    {
        close_socket(sock);
        fprintf(stderr, "Error listening on socket.\n");
        return EXIT_FAILURE;
    }

    /* finally, loop waiting for input and then write it back */
    max_fd = sock;
    // print_clients(clients);
    while (1)
    {
        // print_clients(clients);
        max_fd = fd_set_init(&fd_read, sock, clients);
        // print_clients(clients);

        if ((ret = select(max_fd + 1, &fd_read, NULL, NULL, 0)) == -1)
        {
            close_socket(sock);
            fprintf(stderr, "Error select.\n");
            return EXIT_FAILURE;
        }

        if (ret == 0)
        {
            continue;
        }
        if (FD_ISSET(sock, &fd_read))
        {
            if (build_client_connect(sock, clients))
            {
                return EXIT_FAILURE;
            }
        }

        int i;
        for (i = 0; i < MAX_CLIENTS; i++)
        {
            if (clients[i] != NO_SOCKET && FD_ISSET(clients[i], &fd_read))
            {
                handle_client_recv(clients[i]);
                // fprintf(stdout, "finish handle [%i].\n", clients[i]);
            }
        }

        // if (close_socket(client_sock))
        // {
        //     close_socket(sock);
        //     fprintf(stderr, "Error closing client socket.\n");
        //     return EXIT_FAILURE;
        // }
    }

    // close_socket(sock);

    return EXIT_SUCCESS;
}

/* Toraliser */
#include "Toraliser.h"

Sent *Request(const struct sockaddr_in *app_socket)
{
    Sent *request = (Sent *)malloc(sizeof(Sent));
    request->vn = 4;
    request->cd = 1;
    request->dst_port = app_socket->sin_port;
    request->dst_ip = app_socket->sin_addr.s_addr;
    strcpy(request->username, Username);
    return request;
}

int connect(int app_socket, const struct sockaddr *app_address, socklen_t app_address_len)
{
    int soc = socket(AF_INET, SOCK_STREAM, 0);
    if (soc < 0)
    {
        fprintf(stderr, "Socket wasnt able to be created\n");
        perror("Reason ");
        close(soc);
        return -2;
    }

    int (*pointer_og_connect)(int, const struct sockaddr *, socklen_t);
    pointer_og_connect = dlsym(RTLD_NEXT, "connect");

    struct sockaddr_in socadd;
    socadd.sin_addr.s_addr = inet_addr(tor_proxy_addr);
    socadd.sin_port = htons(tor_proxy_port);
    socadd.sin_family = AF_INET;

    if ((pointer_og_connect(soc, (const struct sockaddr *)&socadd, sizeof(socadd))) == 0)
    {
        printf("Successful connection to TOR PROXY SERVER!\n");
    }
    else
    {
        fprintf(stderr, "Failed to connect the socket to the tor proxy server\n");
        perror("Reason ");
        return -3;
    }

    char buff_reciev[64];
    Sent *send_packet = Request((const struct sockaddr_in *)app_address);
    write(soc, send_packet, sizeof(Sent));
    memset(buff_reciev, 0, 64);
    read(soc, buff_reciev, sizeof(buff_reciev));
    Recieved *reciev = (Recieved *)buff_reciev;
    if (reciev->cd != 90)
    {
        fprintf(stderr, "Handshake failed!\n");
        free(send_packet);
        close(soc);
        return -4;
    }
    printf("Successfull handshake\n");
    free(send_packet);
    dup2(soc, app_socket);

    return 0;
}
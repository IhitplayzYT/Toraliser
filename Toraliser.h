/* Toraliser.h */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <ncurses.h>
#include <sys/random.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <dlfcn.h>

#define tor_proxy_addr "127.0.0.1"
#define tor_proxy_port 9050
#define default_port 80
#define Username "anonyms"

typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned int u32;
typedef unsigned long int u64;

struct sent_packet
{
    u8 vn;
    u8 cd;
    u16 dst_port;
    u32 dst_ip;
    char username[8];
};

struct received_packet
{
    u8 vn;
    u8 cd;
    u16 dst_port;
    u32 dst_ip;
};

typedef struct sent_packet Sent;
typedef struct received_packet Received;

Sent *Request(const struct sockaddr_in *);
int connect(int, const struct sockaddr *, socklen_t);
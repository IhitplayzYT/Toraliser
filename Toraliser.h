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

typedef unsigned char i8;
typedef unsigned short int i16;
typedef unsigned int i32;
typedef unsigned long int i64;

struct sent_packet
{
    i8 vn;
    i8 cd;
    i16 dst_port;
    i32 dst_ip;
    char username[8];
};

struct recieved_packet
{
    i8 vn;
    i8 cd;
    i16 dst_port;
    i32 dst_ip;
};

typedef struct sent_packet Sent;
typedef struct recieved_packet Recieved;

Sent *Request(const struct sockaddr_in *);
int connect(int, const struct sockaddr *, socklen_t);
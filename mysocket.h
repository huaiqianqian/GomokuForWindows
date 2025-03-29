#ifndef MYSOCKET_H
#define MYSOCKET_H

#include <winsock2.h>

#pragma comment(lib, "Ws2_32.lib")

// udp实际上并没有用到

#define MY_TCP 1
#define MY_UDP 2
typedef struct sockaddr* pSA;
typedef struct sockaddr_in SA;

void my_socketCli(SOCKET* fd_socket, int type, char* ip, int port, SA* my_addr);
void my_socketSer(SOCKET* fd_socket, int type, char* ip, int port);

void my_connect(SOCKET fd_socket, pSA server_addr, int len); //client to connect
void my_listen(SOCKET fd_socket, int listen_num); // server to listen

void my_accept(SOCKET* fd_client, SOCKET fd_socket, pSA client_addr, int* addrlen);

void my_send(int* sendlen, SOCKET  fd_socket, const char* msg, int len);
void my_recv(int* recvlen, SOCKET  fd_socket, char* msg, int len);

void my_close(SOCKET fd_socket);

void my_pre();
void my_defer();

#endif

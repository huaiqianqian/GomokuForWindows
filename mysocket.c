#include "mysocket.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <ws2tcpip.h>

void my_socketCli(SOCKET* fd_socket, int type, char* ip, int port, SA* my_addr) {
    // 检查协议类型合法性
    if (type != MY_TCP && type != MY_UDP) {
        fprintf(stderr, "[ERROR] invalid arg\n");
        my_defer();
        exit(EXIT_FAILURE);
    }

    // 创建 socket
    if (type == MY_TCP) {
        *fd_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    }
    else {
        *fd_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    }

    // 检查 socket 创建结果
    if (*fd_socket == INVALID_SOCKET) {
        int error = WSAGetLastError();
        fprintf(stderr, "[ERROR] socket init failed with code: %d\n", error);
        my_defer();
        exit(EXIT_FAILURE);
    }

    // 初始化地址结构
    memset(my_addr, 0, sizeof(SA));
    my_addr->sin_family = AF_INET;
    my_addr->sin_port = htons(port);
    my_addr->sin_addr.s_addr = inet_addr(ip);
}

void my_socketSer(SOCKET* fd_socket, int type, char* ip, int port) {
    // 检查协议类型合法性
    if (type != MY_TCP && type != MY_UDP) {
        fprintf(stderr, "[ERROR] invalid arg\n");
        my_defer();
        exit(EXIT_FAILURE);
    }

    // 创建套接字
    if (type == MY_TCP) {
        *fd_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    }
    else {
        *fd_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    }

    // 检查 socket 创建结果
    if (*fd_socket == INVALID_SOCKET) {
        int error = WSAGetLastError();
        fprintf(stderr, "[ERROR] socket init failed with code: %d\n", error);
        my_defer();
        exit(EXIT_FAILURE);
    }

    // 初始化地址结构
    struct sockaddr_in my_addr;
    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port);
    my_addr.sin_addr.s_addr = inet_addr(ip);

    // 检查 IP 地址有效性
    if (my_addr.sin_addr.s_addr == INADDR_NONE) {
        fprintf(stderr, "[ERROR] Invalid IP address: %s\n", ip);
        closesocket(*fd_socket);  // 关闭已创建的 socket
        my_defer();
        exit(EXIT_FAILURE);
    }

    // 绑定地址
    if (bind(*fd_socket, (struct sockaddr*)&my_addr, sizeof(my_addr)) == SOCKET_ERROR) {
        int error = WSAGetLastError();
        fprintf(stderr, "[ERROR] bind failed with code: %d\n", error);
        closesocket(*fd_socket);  // 关闭已创建的 socket
        my_defer();
        exit(EXIT_FAILURE);
    }

    // 确认绑定信息
    struct sockaddr_in bound_addr;
    int bound_addr_len = sizeof(bound_addr);
    if (getsockname(*fd_socket, (struct sockaddr*)&bound_addr, &bound_addr_len) == SOCKET_ERROR) {
        int error = WSAGetLastError();
        fprintf(stderr, "[WARN] getsockname error: %d\n", error);  // 警告而非致命错误
    }
    else {
        printf("Successfully bound to IP: %s, Port: %d\n",
            inet_ntoa(bound_addr.sin_addr), ntohs(bound_addr.sin_port));
    }
}

void my_listen(SOCKET fd_socket, int listen_num) {
    if (listen(fd_socket, listen_num) == SOCKET_ERROR) {
        int error = WSAGetLastError();
        fprintf(stderr, "[ERROR] listen failed with code: %d\n", error);
        closesocket(fd_socket);  // 关闭监听 socket
        my_defer();
        exit(EXIT_FAILURE);
    }
    printf("Listen successful.\n");
}

void my_accept(SOCKET* fd_client, SOCKET fd_socket, pSA client_addr, int* addrlen) {
    *fd_client = accept(fd_socket, client_addr, addrlen);
    if (*fd_client == INVALID_SOCKET) {
        int error = WSAGetLastError();
        fprintf(stderr, "[ERROR] accept failed with code: %d\n", error);
        closesocket(fd_socket);  // 关闭监听 socket
        my_defer();
        exit(EXIT_FAILURE);
    }
    printf("Client connected from IP: %s, Port: %d\n",
        inet_ntoa(((SA*)client_addr)->sin_addr),
        ntohs(((SA*)client_addr)->sin_port));
}

void my_send(int* sendlen, SOCKET fd_socket, const char* msg, int len) {
    *sendlen = send(fd_socket, msg, len, 0);
    if (*sendlen == SOCKET_ERROR) {
        int error = WSAGetLastError();
        fprintf(stderr, "[ERROR] send failed with code: %d\n", error);
        my_close(fd_socket);
        my_defer();
        exit(EXIT_FAILURE);
    }
}

void my_recv(int* recvlen, SOCKET fd_socket, char* msg, int len) {
    *recvlen = recv(fd_socket, msg, len, 0);
    if (*recvlen == SOCKET_ERROR) {
        int error = WSAGetLastError();
        fprintf(stderr, "[ERROR] recv failed with code: %d\n", error);
        my_close(fd_socket);
        my_defer();
        exit(EXIT_FAILURE);
    }
}

void my_close(SOCKET fd_socket) {
    int ret = closesocket(fd_socket);
    if (ret == SOCKET_ERROR) {
        int error = WSAGetLastError();
        fprintf(stderr, "[ERROR] close failed with code: %d\n", error);
        my_defer();
        exit(EXIT_FAILURE);
    }
}

void my_connect(SOCKET fd_socket, pSA server_addr, int len) {
    int ret = connect(fd_socket, (const struct sockaddr*)server_addr, len);
    if (ret == SOCKET_ERROR) {
        int error = WSAGetLastError();
        fprintf(stderr, "[ERROR] connect failed with code: %d\n", error);
        my_close(fd_socket);
        my_defer();
        exit(EXIT_FAILURE);
    }
}

void my_pre() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
}

void my_defer() {
    WSACleanup();
}
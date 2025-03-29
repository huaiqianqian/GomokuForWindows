#include "gomoku.h"
#include "mysocket.h"
#include <stdio.h>

#define IP "127.0.0.1"
#define PORT 13579

int main() {
	my_pre();

	char gomoku[MYSIZE][MYSIZE];
	SA server_addr;
	int msg[2];
	SOCKET fd_socket;
	int index, sendlen = 0, recvlen = 0;
	my_socketCli(&fd_socket, MY_TCP, IP, PORT, &server_addr);
	my_connect(fd_socket, (pSA)&server_addr, sizeof(SA));
	printf("client start\n");
	gomoku_init(gomoku);
	int x, y;
	while (1) {
			gomoku_step(gomoku,&x,&y,1);
			msg[0] = x;
			msg[1] = y;
			my_send(&sendlen, fd_socket, (char*)msg, 8);
			if (checkWin(x, y, gomoku, getchess(1))) {
				printf("Client win\n");
				break;
			}
			printf("waiting for server...\n");
			my_recv(&recvlen, fd_socket, (char*)msg, 1024);
			gomoku[msg[0]][msg[1]] = getchess(0);
			if (checkWin(msg[0], msg[1], gomoku, getchess(0))) {
				printf("Server win\n");
				break;
			}
			gomoku_show(gomoku);
	}
	my_close(fd_socket);

	my_defer();
}

#include "mysocket.h"
#include "gomoku.h"
#include <stdio.h>

#define IP "127.0.0.1"
#define PORT 13579

int main() {
	my_pre();

	int msg[2];
	char gomoku[MYSIZE][MYSIZE];
	SA client_addr;
	SOCKET fd_socket, fd_client;
	my_socketSer(&fd_socket, MY_TCP, IP, PORT);
	my_listen(fd_socket, 5);

	int sendlen = 0, recvlen = 0, len = sizeof(client_addr);
	int i, j, x, y;
	gomoku_init(gomoku);
	gomoku_show(gomoku);
	printf("waiting for client to connect...\n");
	my_accept(&fd_client, fd_socket, (pSA)&client_addr, &len);
	printf("game start\n");
	while (1) {
		printf("waiting for client...\n");
		my_recv(&recvlen, fd_client, (char*)msg, 1024);
		gomoku[msg[0]][msg[1]] = getchess(1);
		if (checkWin(msg[0], msg[1], gomoku, getchess(1))) {
			printf("Client win\n");
			break;
		}
		gomoku_show(gomoku);
		gomoku_step(gomoku, &x, &y, 0);
		msg[0] = x;
		msg[1] = y;
		my_send(&sendlen, fd_client, (char*)msg, recvlen);

		if (checkWin(x, y, gomoku, getchess(0))) {
			printf("Server win\n");
			break;
		}
	}
	my_close(fd_socket);
	my_close(fd_client);

	my_defer();
}




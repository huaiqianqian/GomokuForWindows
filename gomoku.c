#include"gomoku.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int checkWin(int x, int y, char gomoku[][MYSIZE], char flagWin) {
	// 定义四个检查方向及其反向：水平、垂直、主对角线、副对角线
	const int dirs[4][2][2] = {
		{{0, 1}, {0, -1}},   // 水平方向（右、左）
		{{1, 0}, {-1, 0}},   // 垂直方向（下、上）
		{{1, 1}, {-1, -1}},  // 主对角线（右下、左上）
		{{-1, 1}, {1, -1}}   // 副对角线（右上、左下）
	};

	for (int d = 0; d < 4; ++d) {  // 遍历四个方向
		int count = 0;  // 当前方向的连续棋子计数

		// 检查正方向（右、下、右下、右上）
		int dx = dirs[d][0][0], dy = dirs[d][0][1];
		for (int i = x + dx, j = y + dy;
			i > 0 && i < MYSIZE && j > 0 && j < MYSIZE;
			i += dx, j += dy) {
			if (gomoku[i][j] != flagWin) break;
			++count;
		}

		// 检查反方向（左、上、左上、左下）
		dx = dirs[d][1][0], dy = dirs[d][1][1];
		for (int i = x + dx, j = y + dy;
			i > 0 && i < MYSIZE && j > 0 && j < MYSIZE;
			i += dx, j += dy) {
			if (gomoku[i][j] != flagWin) break;
			++count;
		}

		// 若连续棋子数≥4（加上当前棋子共5个）
		if (count >= 4) return 1;
	}
	return 0;
}

void gomoku_show(char gomoku[][MYSIZE]) {
	int i, j;
	for (i = 0; i < MYSIZE; i++) {
		for (j = 0; j < MYSIZE; j++) {
			printf("%c ", gomoku[i][j]);
		}
		printf("\n");
	}
}

void gomoku_init(char gomoku[][MYSIZE]) {
	int i, j;
	for (i = 0; i < MYSIZE; i++) {
		for (j = 0; j < MYSIZE; j++) {
			gomoku[i][j] = '_';
		}
	}
	for (i = 0; i < MYSIZE; i++)
		gomoku[0][i] = gomoku[i][0] = i<=9? (i + '0'): (i-10 + 'a');
}


void gomoku_step(char gomoku[][MYSIZE], int* x, int* y, int is_c) {
	system("cls");
	gomoku_show(gomoku);
loop:
	{
#define player_name(is_c) (is_c?"client":"server")
		printf("%s's turn:\n", player_name(is_c));
		scanf("%d %d", x, y);
		if (*x < 1 || *x > MYSIZE - 1 || *y < 1 || *y > MYSIZE - 1) {
			printf("invalid input\n");
			goto loop;
		}

		if (gomoku[*x][*y] != '_') {
			printf("there has been one chess.\n");
			goto loop;
		}

		gomoku[*x][*y] = getchess(is_c);
#undef player_name
	}
	gomoku_show(gomoku);
}
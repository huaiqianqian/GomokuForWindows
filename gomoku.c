#include"gomoku.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int checkWin(int x, int y, char gomoku[][MYSIZE], char flagWin) {
	// �����ĸ���鷽���䷴��ˮƽ����ֱ�����Խ��ߡ����Խ���
	const int dirs[4][2][2] = {
		{{0, 1}, {0, -1}},   // ˮƽ�����ҡ���
		{{1, 0}, {-1, 0}},   // ��ֱ�����¡��ϣ�
		{{1, 1}, {-1, -1}},  // ���Խ��ߣ����¡����ϣ�
		{{-1, 1}, {1, -1}}   // ���Խ��ߣ����ϡ����£�
	};

	for (int d = 0; d < 4; ++d) {  // �����ĸ�����
		int count = 0;  // ��ǰ������������Ӽ���

		// ����������ҡ��¡����¡����ϣ�
		int dx = dirs[d][0][0], dy = dirs[d][0][1];
		for (int i = x + dx, j = y + dy;
			i > 0 && i < MYSIZE && j > 0 && j < MYSIZE;
			i += dx, j += dy) {
			if (gomoku[i][j] != flagWin) break;
			++count;
		}

		// ��鷴�������ϡ����ϡ����£�
		dx = dirs[d][1][0], dy = dirs[d][1][1];
		for (int i = x + dx, j = y + dy;
			i > 0 && i < MYSIZE && j > 0 && j < MYSIZE;
			i += dx, j += dy) {
			if (gomoku[i][j] != flagWin) break;
			++count;
		}

		// ��������������4�����ϵ�ǰ���ӹ�5����
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
	printf("hint : a->10 b->11 ...\n");
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

	// ѭ��ֱ����ȡ��Ч����
	while (1) {
		// ��ʾ��ǰ��ұ�ʶ
		printf("%s's turn (Input row column, range 1-%d):\n",
			is_c ? "Client" : "Server", MYSIZE - 1);

		// ��ȡ����������
		char input[32];
		if (fgets(input, sizeof(input), stdin) == NULL) {
			fprintf(stderr, "Input error: Read operation failed\n");
			exit(EXIT_FAILURE);
		}

		// �Ƴ����з�
		input[strcspn(input, "\n")] = '\0';

		// ��ʽ��֤�����������������
		if (sscanf(input, "%d %d", x, y) != 2) {
			puts("Error: Invalid format. Expected: [number][space][number]");
			continue;
		}

		// ��Χ��Ч����֤
		if (*x < 1 || *x >= MYSIZE || *y < 1 || *y >= MYSIZE) {
			printf("Error: Coordinates out of range. Valid values: 1-%d\n", MYSIZE - 1);
			continue;
		}

		// ����λ�ÿ�������֤
		if (gomoku[*x][*y] != '_') {
			printf("Error: Position (%d,%d) already occupied by '%c'\n",
				*x, *y, gomoku[*x][*y]);
			continue;
		}

		break;
	}

	// ���Ӳ�ˢ��������ʾ
	gomoku[*x][*y] = getchess(is_c);
	gomoku_show(gomoku);
}
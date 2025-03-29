#ifndef _5ZIQI_
#define _5ZIQI_

#define MYSIZE 16
// 0 is the border so the real size is MYSIZE - 1

#define getchess(is_c) (is_c?'O':'X')

int checkWin(int x, int y, char gomoku[MYSIZE][MYSIZE], char flagWin);
void gomoku_show(char gomoku[][MYSIZE]);
void gomoku_init(char gomoku[][MYSIZE]);
void gomoku_step(char gomoku[][MYSIZE], int* x, int* y, int is_c);
#endif

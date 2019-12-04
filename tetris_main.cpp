/*
 * TETRIS 
 * made by juil
 * update log: ver 0.1(2017.09.15)
 * update log: ver 0.2(2017.09.28) 스페이스바 기능 추가, 게임오버 추가, 점수 확인 추가, 화면 버벅거림 제거
 */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <iostream>
using namespace std;

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define ESC 27
#define BX 5
#define BY 1
#define BW 10
#define BH 20
#define delay(n) Sleep(n)                              // n/1000초만큼 시간 지연
#define randomize() srand((unsigned)time(NULL))         // 난수 발생기 초기화
#define random(n) (rand() % (n))                        //0~n까지의 난수 발생

enum { EMPTY, BRICK, WALL };
typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE;
char *Tile[3] = { "　","■","□" };

int type = 0;	//brick[type];난수로 한 턴마다 초기화시켜준다.
int rot = 0;	//0 or 1 or 2 or 3
int nx, ny;
int score = 0;

void DrawScreen();	
void DrawBoard();	
bool ProcessKey();	//키 눌렀으면 true 안눌렀으면 false반환
void DrawBrick(bool);	
int GetAround(int x, int y, int type, int rot);	 
bool MoveDown();	//부딪히면 true반환
void TestFull();	
void clrscr();
void gotoxy(int x, int y);
void setcursortype(CURSOR_TYPE c);

const char *brick[7] = //char *brick[type];
{
	"1111000000000000001000100010001011110000000000000010001000100010",	//ㅡ
	"1000111000000000011001000100000000001110001000000100010011000000",	//└
	"0010111000000000010001000110000000001110100000001100010001000000",	//┘
	"0110011000000000011001100000000001100110000000000110011000000000",	//ㅁ
	"0110110000000000010001100010000000000110110000001000110001000000",	//S
	"0100111000000000010001100100000000001110010000000100110001000000",	//ㅗ
	"1100011000000000001001100100000000001100011000000100110010000000"	//Z
};
const char *bptr;
char board[BH + 2][BW + 2];

void main()
{
	setcursortype(NOCURSOR);
	randomize();
	int nFrame, nStay;
	clrscr();
	for (int y = 0;y < BH + 2;y++)
	{
		for (int x = 0;x < BW + 2;x++)
		{
			board[y][x] = (x == 0 || x == BW + 1 || y == 0 || y == BH + 1) ? WALL : EMPTY;
		}
	}	  //초기 board세팅
	DrawScreen();
	nFrame = 5;
	for (;1;)
	{
		nx = BW / 2; ny = 1;
		rot = 0; type = random(6);
		
		if (GetAround(nx, ny, type, rot != EMPTY)) break;
		nStay = nFrame;
		for (;2;)
		{	
			DrawBrick(true);
			if (--nStay == 0)
			{
				nStay = nFrame;
				if (MoveDown()==true) break;
			}
			
			if (ProcessKey() == true)break;
			delay(50);			
		}
	}
	clrscr();
	gotoxy(20, 5); cout << "G A M E O V E R!";
	gotoxy(20, 6); cout << "S C O R E: " << score << endl;
}
void DrawScreen()
{
	DrawBoard();
	gotoxy(40, 2);
	cout << "TETRIS ver: 0.2";
	gotoxy(40, 4);
	cout << "Made by 63days";
	gotoxy(40, 6);
	cout << "좌우: 이동, 위: 회전, 아래: 내림, 스페이스바: 전부 내림";
}
void DrawBoard()
{
	for (int y = 0;y < BH + 2;y++)
	{
		for (int x = 0;x < BW + 2;x++)
		{
			if (board[y][x] == WALL)
			{
				gotoxy(BX+x * 2,BY+ y);
				cout << Tile[WALL];
			}
			else if(board[y][x]==EMPTY)
			{
				gotoxy(BX+x * 2,BY+ y);
				cout << Tile[EMPTY];
			}
			
			else if(board[y][x]==BRICK)
			{
				gotoxy(BX+x * 2,BY+ y);
				cout << Tile[BRICK];
			}
		}
		gotoxy(40, 8);
		cout << "SCORE: " << score;
	}
}
void DrawBrick(bool SHOW)
{
	if (SHOW == true)
	{
		for (int y = 0;y < 4;y++)
		{
			for (int x = 0;x < 4;x++)
			{
				if (brick[type][x + 4 * y + 16 * rot] == '1')
				{
					gotoxy(BX+(x + nx) * 2,BY+ y + ny);
					cout << Tile[BRICK];
				}
			}
		}
	}
	else
	{
		for (int y = 0;y < 4;y++)
		{
			for (int x = 0;x < 4;x++)
			{
				if (brick[type][x + 4 * y + 16 * rot] == '1')
				{
					gotoxy(BX+(x + nx) * 2,BY+ y + ny);
					cout << Tile[EMPTY];
				}
			}
		}
	}
}
int GetAround(int x, int y, int type, int rot)
{
	int bx, by;
	int k = EMPTY;
	for (int by = 0;by < 4;by++)
	{
		for (int bx = 0;bx < 4;bx++)
		{
			if (brick[type][bx + 4 * by + 16 * rot] == '1')
			{
				k = max(k, board[y + by][x + bx]);
			}
		}
	}
	
	return k;
}
bool MoveDown()
{
	if (GetAround(nx,ny+1,type,rot)==EMPTY)
	{
		DrawBrick(false);
		ny++;
		DrawBrick(true);
		TestFull();
		return false;
	}
	else
	{
		for (int by = 0;by < 4;by++)
		{
			for (int bx = 0;bx < 4;bx++)
			{
				if (brick[type][bx + 4 * by + 16 * rot] == '1')
				{
					board[ny + by][nx + bx] = BRICK;
				}
			}
		}
		TestFull();		
		return true;
	}
}
bool ProcessKey()
{
	int ch, trot;
	if (kbhit())
	{
		ch = getch();
		if (ch == 0xE0 || ch == 0)
		{
			ch = getch();
			switch (ch)
			{
			case LEFT:
				if (GetAround(nx - 1, ny, type, rot) == EMPTY)
				{
					DrawBrick(false);
					nx--;
					DrawBrick(true);
				}
				break;
			case RIGHT:
				if (GetAround(nx + 1, ny, type, rot) == EMPTY)
				{
					DrawBrick(false);
					nx++;
					DrawBrick(true);
				}
				break;
			case DOWN:
				if (GetAround(nx, ny + 1, type, rot) == EMPTY)
				{
					DrawBrick(false);
					ny++;
					DrawBrick(true);
				}
				break;
			case UP:
				if (rot < 3)
				{
					if (GetAround(nx, ny, type, rot + 1) == EMPTY)
					{
						DrawBrick(false);
						rot++;
						DrawBrick(true);
					}
				}
				else if (rot == 3)
				{
					if (GetAround(nx, ny, type, rot - 3) == EMPTY)
					{
						DrawBrick(false);
						rot -= 3;
						DrawBrick(false);
					}
				}
				break;
			}
		}
		else
		{
			switch (ch)
			{
			case ' ':
				while (GetAround(nx, ny + 1, type, rot) == EMPTY)
				{
					DrawBrick(false);
					ny++;
					DrawBrick(true);
				}
				break;
			}
		}
		return false;

	}
}
void TestFull()
{
	for (int by = BH;by >= 1;by--)
	{
		for (int bx = 1;bx <= BW;bx++)
		{
			if (board[by][bx] != BRICK)
				break;
			else if (bx == BW && board[by][bx] == BRICK)
			{
				for (int ty = by;ty > 1;ty--)
				{
					for (int tx = 1;tx <= BW;tx++)
					{
						board[ty][tx] = board[ty - 1][tx];
					}
				}
				score += 100;
				DrawBoard();
			}
		}
	}
}

void clrscr()
{
	system("cls");
}

// 커서를 x,y좌표로 이동시킨다.
void gotoxy(int x, int y)
{
	COORD Cur;
	Cur.X = x;
	Cur.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}
void setcursortype(CURSOR_TYPE c)
{
	CONSOLE_CURSOR_INFO CurInfo;

	switch (c) {
	case NOCURSOR:
		CurInfo.dwSize = 1;
		CurInfo.bVisible = FALSE;
		break;
	case SOLIDCURSOR:
		CurInfo.dwSize = 100;
		CurInfo.bVisible = TRUE;
		break;
	case NORMALCURSOR:
		CurInfo.dwSize = 20;
		CurInfo.bVisible = TRUE;
		break;
	}
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}
# Tetris:C implementation
![figure](./tetrisfigure.png)

## Code Explanation

```c
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
```
Each line corresponds to each block. Block has 4x4 space. 0 means blank space and 1 means block space. There are 7 blocks in total.

```c
char board[BH + 2][BW + 2];
/////
/////
for (int y = 0;y < BH + 2;y++)
	{
		for (int x = 0;x < BW + 2;x++)
		{
			board[y][x] = (x == 0 || x == BW + 1 || y == 0 || y == BH + 1) ? WALL : EMPTY;
		}
	}	  //초기 board세팅
```
board is a 2D-array to draw frame of game. The ends of array are walls and the rest are blank.

```c
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
```
This code is the main algorithm of the game.  
```c
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
```
MoveDown() function returns true if a current block could go down; otherwise returns false, and redraw a block that goes down one block. 
```c
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
```
int GetAround(x, y) function returns (x,y) position's space number.
- EMPTY: 0
- BRICK: 1
- WALL: 2
```c
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
```
Every frame, processKey() takes key value pressed by user and returns true if user pressed any of the operating keys; otherwise returns false.

## Reference site: [http://soen.kr/]
Used header file: Turbo.h (Download from the site above.)


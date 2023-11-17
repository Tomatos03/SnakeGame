#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <ctype.h>
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define ROW 30//地图宽
#define COL 40//地图长
#define InitMoveSpeed 80; //初始默认移动速度

//初始界面
void InitInterface(void);
//跳转光标
void CursorJump(int,int);
//影藏光标
void HideCursor(void);
//初始化蛇
void InitSnake(void);
//蛇的移动
void SnakeMove(int);
//食物生成
void GenerateFood(void);
//游戏框架
void GameFrame(void);
//游戏结束
bool GameEnd(void);
//绘制方块
void DrawSquare(int, int);

typedef struct { int x; int y;} SnakeBody,SnakeFood;
typedef struct { int x; int y; int bodyLen; } SnakeHead;
SnakeBody snakeBody[ROW * COL];
SnakeFood snakeFood;
SnakeHead snakeHead;
int currentMoveSpeed = InitMoveSpeed;//默认为85
int currentDirection = RIGHT;//默认蛇移动方向为右;

int main()
{
	do {
		system("title 贪吃蛇");//设置窗口名称
		system("mode 80,30");
		HideCursor();//影藏光标
		InitInterface();//初始化界面
		InitSnake();//初始化蛇
		GenerateFood();//食物生成
		GameFrame();//游戏主体框架
	} while (GameEnd());
	return 0;
}

void InitInterface(void)
{
	for (int y = 0; y < ROW; y++)
	{
		if (y != 0 && y != ROW - 1) {
			DrawSquare(0, y);
			DrawSquare((COL - 1) * 2, y);
			continue;
		}

		for (int x = 0; x < COL; x++)
		{
			DrawSquare(2 * x, y);
		}
	}
}

void CursorJump(int x,int y)
{
	//该结构包含坐标信息X，Y
	COORD coodInate;
	coodInate.X = x;
	coodInate.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coodInate);
}

void HideCursor(void)
{
	//此结构包含了光标的两个信息大小和是否可见
	CONSOLE_CURSOR_INFO setCursor ;
	setCursor.dwSize = 10;
	setCursor.bVisible = 0;
	HANDLE handle;
	//该函数返回相应的句柄
	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(handle,&setCursor);
}

void GameFrame(void)
{
	while (true)
	{
		Sleep(currentMoveSpeed);
		if (!_kbhit()) 
		{
			SnakeMove(currentDirection);
		}
		else 
		{
			int unUse = _getch();
			SnakeMove(_getch());
		}
		
		bool isFood = (snakeHead.x == snakeFood.x) && (snakeHead.y == snakeFood.y);
		//判断蛇是否吃到食物
		if (isFood)
		{
			GenerateFood();
			++snakeHead.bodyLen;
			currentMoveSpeed -= (currentMoveSpeed >= 25) ? 1 : 0;
			continue;
		}

		//判断是否撞到自己的尾巴
		//如果撞到自己的尾巴先退出内层循环在退出外层循环
		bool selfConsumption = false;
		for (int i = 0; i < snakeHead.bodyLen; i++)
		{
			if ((snakeHead.x == snakeBody[i].x) && (snakeHead.y == snakeBody[i].y))
			{
				selfConsumption = true;
				break;
			}
		}

		if (selfConsumption) 
		{
			break;
			continue;
		}

		//判断蛇是否撞墙
		bool isHittingWall = (snakeHead.y == 0) || (snakeHead.x == 0) || (snakeHead.x == 2 * (COL - 1)) || (snakeHead.y == ROW - 1);
		if (isHittingWall) 
		{
			break;
		}
	}
}

void GenerateFood(void)
{
	SnakeFood newFood;
	srand((int)time(0));
	bool isHeadCoordinateSame;
	bool foodInWall;
	do
	{
		newFood.x = rand() % (COL * 2 - 5) + 2;
		newFood.y = rand() % (ROW - 2) + 1;
		foodInWall = newFood.x % 2 != 0;
		isHeadCoordinateSame = (newFood.x == snakeHead.x && newFood.y == snakeHead.y);
	} while (isHeadCoordinateSame || foodInWall);
	snakeFood = newFood;
	DrawSquare(snakeFood.x, snakeFood.y);
}

void SnakeMove(int direction) {
	bool levelSame = (currentDirection == LEFT || currentDirection == RIGHT) && (direction == LEFT || direction == RIGHT);
	bool erectSame = (currentDirection == UP || currentDirection == DOWN) && (direction == UP || direction == DOWN);
	if (!levelSame || !erectSame) {
		currentDirection = direction;
	}
	
	SnakeHead preSnakeHead = snakeHead;
	//蛇头部移动
	switch (currentDirection) {
		case UP:
			--snakeHead.y;
			break;
		case DOWN:
			++snakeHead.y;
			break;
		case LEFT:
			snakeHead.x -= 2;
			break;
		case RIGHT:
			snakeHead.x += 2;
			break;
	}
	DrawSquare(snakeHead.x, snakeHead.y);
	SnakeBody* preSnakeBody = (SnakeBody*) malloc (snakeHead.bodyLen * sizeof(SnakeBody));
	for (int i = 0; i < snakeHead.bodyLen; i++){
		preSnakeBody[i] = snakeBody[i];
	}

	snakeBody[0].x = preSnakeHead.x;
	snakeBody[0].y = preSnakeHead.y;

	for (int i = 0; i < snakeHead.bodyLen; i++){
		snakeBody[i + 1] = preSnakeBody[i];
	}
	
	CursorJump(preSnakeBody[snakeHead.bodyLen - 1].x, preSnakeBody[snakeHead.bodyLen - 1].y);
	printf(" ");
	free(preSnakeBody);
}

bool GameEnd(void)
{
	system("cls");
	CursorJump(COL / 2, ROW / 2);
	printf("游戏结束是否再来一局(Y/N):");
	while (true) {
		char keyboardInput = tolower(_getch());
		if (keyboardInput == 'y')
		{
			system("cls");
			currentDirection = RIGHT;
			currentMoveSpeed = InitMoveSpeed;
			return true;
		}
		else if (keyboardInput == 'n')
		{
			return false;
		}
	}
}

void InitSnake(void)
{
	//设置蛇的初始坐标信息以及长度
	snakeHead.x = 10;
	snakeHead.y = ROW / 2;
	snakeHead.bodyLen = 2;
	DrawSquare(snakeHead.x, snakeHead.y);
	for (int i = 0; i < snakeHead.bodyLen; i++)
	{
		snakeBody[i].x = snakeHead.x - (i + 1) * 2;
		snakeBody[i].y = snakeHead.y;
		DrawSquare(snakeBody[i].x, snakeBody[i].y);
	}
}

void DrawSquare(int x, int y) {
	CursorJump(x, y);
	printf("■");
}
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
#define ROW 30//��ͼ��
#define COL 40//��ͼ��
#define InitMoveSpeed 80; //��ʼĬ���ƶ��ٶ�

//��ʼ����
void InitInterface(void);
//��ת���
void CursorJump(int,int);
//Ӱ�ع��
void HideCursor(void);
//��ʼ����
void InitSnake(void);
//�ߵ��ƶ�
void SnakeMove(int);
//ʳ������
void GenerateFood(void);
//��Ϸ���
void GameFrame(void);
//��Ϸ����
bool GameEnd(void);
//���Ʒ���
void DrawSquare(int, int);

typedef struct { int x; int y;} SnakeBody,SnakeFood;
typedef struct { int x; int y; int bodyLen; } SnakeHead;
SnakeBody snakeBody[ROW * COL];
SnakeFood snakeFood;
SnakeHead snakeHead;
int currentMoveSpeed = InitMoveSpeed;//Ĭ��Ϊ85
int currentDirection = RIGHT;//Ĭ�����ƶ�����Ϊ��;

int main()
{
	do {
		system("title ̰����");//���ô�������
		system("mode 80,30");
		HideCursor();//Ӱ�ع��
		InitInterface();//��ʼ������
		InitSnake();//��ʼ����
		GenerateFood();//ʳ������
		GameFrame();//��Ϸ������
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
	//�ýṹ����������ϢX��Y
	COORD coodInate;
	coodInate.X = x;
	coodInate.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coodInate);
}

void HideCursor(void)
{
	//�˽ṹ�����˹���������Ϣ��С���Ƿ�ɼ�
	CONSOLE_CURSOR_INFO setCursor ;
	setCursor.dwSize = 10;
	setCursor.bVisible = 0;
	HANDLE handle;
	//�ú���������Ӧ�ľ��
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
		//�ж����Ƿ�Ե�ʳ��
		if (isFood)
		{
			GenerateFood();
			++snakeHead.bodyLen;
			currentMoveSpeed -= (currentMoveSpeed >= 25) ? 1 : 0;
			continue;
		}

		//�ж��Ƿ�ײ���Լ���β��
		//���ײ���Լ���β�����˳��ڲ�ѭ�����˳����ѭ��
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

		//�ж����Ƿ�ײǽ
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
	//��ͷ���ƶ�
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
	printf("��Ϸ�����Ƿ�����һ��(Y/N):");
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
	//�����ߵĳ�ʼ������Ϣ�Լ�����
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
	printf("��");
}
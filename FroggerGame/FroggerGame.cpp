// FroggerGame.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iostream"
#include "time.h"
#include "conio.h"
#include "windows.h"

using namespace std;

void SetConsoleSize(int width, int height)
{
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, width, height, TRUE);
	//ShowWindow(console, SW_MAXIMIZE);
}
void removeCursor()
{
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}
void gotoxy(int x, int y)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	_COORD pos;
	pos.X = x;
	pos.Y = y;

	SetConsoleCursorPosition(hConsole, pos);
}
void TextColor(int x)
{
	HANDLE mau;
	mau = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(mau, x);
}

void WindowInit()
{
	SetConsoleSize(480, 640);
	removeCursor();
}

const int screenWidth = 60, screenHeight = 35;
enum Dir { STOP = 0, LEFT = 1, RIGHT = 2, UP = 3, DOWN = 4 };
struct Position
{
	int x;
	int y;
};

class Frog
{
private:
	Position originalPivotPos; Position pivotPos;
	Dir direction;

public:
	Position bodyPos[3][3];
	inline void CalcBodyPos()
	{	
		if (pivotPos.x <= 2)
			pivotPos.x = 2;
		if (pivotPos.x > screenWidth - 2)
			pivotPos.x = screenWidth - 2;

		//if (pivotPos.y <= 2)
			//pivotPos.y = 2;
		if (pivotPos.y > screenHeight - 2)
			pivotPos.y = screenHeight - 3;

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (i < 1)
				{
					bodyPos[i][j].y = pivotPos.y - 1;
				}
				else if (i > 1)
				{
					bodyPos[i][j].y = pivotPos.y + 1;
				}
				else
				{
					bodyPos[i][j].y = pivotPos.y;
				}

				if (j < 1)
				{
					bodyPos[i][j].x = pivotPos.x - 1;
				}
				else if (j > 1)
				{
					bodyPos[i][j].x = pivotPos.x + 1;
				}
				else
				{
					bodyPos[i][j].x = pivotPos.x;
				}
			}
		}
	}

	Frog(int posX, int posY)
	{
		originalPivotPos.x = posX;
		originalPivotPos.y = posY;
		pivotPos = originalPivotPos;
		CalcBodyPos();
		direction = STOP;
	}
	
	void Reset()
	{
		pivotPos = originalPivotPos;
		CalcBodyPos();
		direction = STOP;
	}

	void Move()
	{
		switch (direction)
		{
			case STOP: 
				break;

			case LEFT:
				pivotPos.x -= 3;
				break;

			case RIGHT:
				pivotPos.x += 3;
				break;
				
			case UP:
				pivotPos.y -= 3;
				break;

			case DOWN:
				pivotPos.y += 3;
				break;

			default:
				break;
		}
		CalcBodyPos();
	}

	inline int getPivotPosX() { return pivotPos.x; }
	inline int getPivotPosY() { return pivotPos.y; }
	inline Dir getDirection() { return direction; }
	inline void setDirection(Dir dir) { direction = dir; }
};

class Car
{
private:
	int speed; char bodyChar;
	Position originalPivotPos; Position pivotPos;
	Dir direction;

public:
	Position bodyPos[3][3];
	inline void CalcBodyPos()
	{
		if (pivotPos.x > screenWidth)
		{
			pivotPos.x = -1;
			speed = rand() % 2 + 1;
		}

		if (pivotPos.x < -1)
		{
			pivotPos.x = screenWidth - 1;
			speed = rand() % 2 + 1;
		}

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (i < 1)
				{
					bodyPos[i][j].y = pivotPos.y - 1;
				}
				else if (i > 1)
				{
					bodyPos[i][j].y = pivotPos.y + 1;
				}
				else
				{
					bodyPos[i][j].y = pivotPos.y;
				}

				if (j < 1)
				{
					bodyPos[i][j].x = pivotPos.x - 1;
				}
				else if (j > 1)
				{
					bodyPos[i][j].x = pivotPos.x + 1;
				}
				else
				{
					bodyPos[i][j].x = pivotPos.x;
				}
			}
		}
	}

	Car(int posX, int posY, Dir dir, int spd, char ch)
	{
		originalPivotPos.x = posX;
		originalPivotPos.y = posY;
		pivotPos = originalPivotPos;
		CalcBodyPos();

		speed = spd;
		direction = dir;
		bodyChar = ch;
	}

	void Reset()
	{
		pivotPos = originalPivotPos;
		CalcBodyPos();
	}
	void Move()
	{
		switch(direction)
		{
			case STOP:
				break;

			case LEFT:
				pivotPos.x -= speed;
				break;

			case RIGHT:
				pivotPos.x += speed;
				break;

			default:
				break;
		}
		CalcBodyPos();
	}

	inline int getPivotPosX() { return pivotPos.x; }
	inline int getPivotPosY() { return pivotPos.y; }
	inline char getBodyChar() { return bodyChar; }
	inline Dir getDirection() { return direction; }
};

class GameManager
{
private:
	int score;
	int width; int height;
	bool quit; bool restart;
	bool gameOver;

	Frog *player;
	Car *enemy[5];

public:
	GameManager(int h, int w)
	{
		height = h;
		width = w;
		score = 0;
		gameOver = false;

		player = new Frog(width / 2 - 1, height - 3);
		for (int i = 0; i < 5; i++)
		{
			//srand(time(NULL));
			int randomDir = rand() % 2 + 1;
			int randomSpeed = rand() % 2 + 1;
			if (randomDir == 1)
			{
				enemy[i] = new Car(2, i * 6 + 2, RIGHT, randomSpeed, '>');
			}
			else if (randomDir == 2)
			{
				enemy[i] = new Car(width - 2, i * 6 + 2, LEFT, randomSpeed, '<');
			}
		}
	}

	~GameManager()
	{
		delete player, enemy;
	}

	void Reset()
	{
		score = 0;
		gameOver = false;
		player->Reset();
		for (int t = 0; t < 5; t++)
			enemy[t]->Reset();
	}

	void CheckInput()
	{
		if (_kbhit())
		{
			char current = _getch();

			if (current == 'w' || current == 'W')
			{
				//if (player->getPivotPosY() > 2)
					player->setDirection(UP);
			}
			else if (current == 's' || current == 'S')
			{
				if (player->getPivotPosY() < height - 3)
					player->setDirection(DOWN);
			}
			else if (current == 'a' || current == 'A')
			{
				player->setDirection(LEFT);
			}
			else if (current == 'd' || current == 'D')
			{
				player->setDirection(RIGHT);
			}
		}
		else
			player->setDirection(STOP);
	}

	void scoreUp()
	{
		score += 100;
		TextColor(14);
		gotoxy(width / 2 - 8, height / 2);
		cout << "YOU GOT 100 POINTS";
		Beep(1200, 100);
		Sleep(1000);
		player->Reset();
	}

	void Logic()
	{
		player->Move();
		for (int t = 0; t < 5; t++)
			enemy[t]->Move();
		if (player->getPivotPosY() < 2)
		{
			scoreUp();
		}
	}

	void Draw()
	{
		gotoxy(0, 0);
		for (int y = 0; y < height; y++) 
		{
			for (int x = 0; x < width; x++)
			{
				bool isPrinted = false;
				bool playerPrinted = false;

				if (y == 0 || y == height - 1)
				{
					TextColor(119);
					cout << '#';
					isPrinted = true;
					TextColor(0);
				}

				if (!isPrinted)
				{
					for (int i = 0; i < 3; i++)
					{
						for (int j = 0; j < 3; j++)
						{
							if (y == player->bodyPos[i][j].y && x == player->bodyPos[i][j].x)
							{
								TextColor(14);
								cout << '^';
								isPrinted = true;
								playerPrinted = true;
								TextColor(0);
							}
						}
					}
				}

				//if (!isPrinted)
				//{
					for (int t = 0; t < 5; t++)
					{
						for (int u = 0; u < 3; u++)
						{
							for (int v = 0; v < 3; v++)
							{
								if (y == enemy[t]->bodyPos[u][v].y && x == enemy[t]->bodyPos[u][v].x)
								{
									if (!playerPrinted)
									{
										if (enemy[t]->getBodyChar() == '>')
										{
											TextColor(11);
										}
										else
										{
											TextColor(10);
										}
										cout << enemy[t]->getBodyChar();
										isPrinted = true;
										TextColor(0);
									}
									else
										gameOver = true;
								}
							}
						}
					}
				//}

				if (!isPrinted)
				{
					if (y >= 5 && (y + 1) % 6 == 0)
					{
						TextColor(7);
						cout << '-';
						isPrinted = true;
						TextColor(0);
					}
				}
				
				if (!isPrinted)
					cout << ' ';
			}
			cout << endl;
		}
		TextColor(15);
		gotoxy(width / 2 - 8, height);
		cout << "Your score: " << score;
		TextColor(0);
		TextColor(10);
		gotoxy(width / 2 - 16, height + 1);
		cout << "C++ FROGGER - Author: Hao Phan";
	}
	void Run()
	{
		play:
		while (!gameOver)
		{
			//srand(time(NULL));
			CheckInput();
			Logic();
			Draw();
		}
		GameOver();
		goto play;
	}
	void GameOver()
	{
		gotoxy(width / 2 - 4, height / 2);
		TextColor(12);
		cout << "GAME OVER" << endl;
		Beep(300, 500);

		gotoxy(width / 2 - 18, height / 2 + 1);
		cout << "Would you like to restart game (y/n)? ";
		while (true)
		{
			char current = _getch();
			if (current == 'y')
			{
				Reset();
				break;
			}
			else if (current == 'n')
			{
				exit(0);
			}
		}
	}
};

int main()
{
	WindowInit();
	GameManager GM(screenHeight, screenWidth);
	GM.Run();
	return 0;
}
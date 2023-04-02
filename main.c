#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#include "Data.h"
#include "Screen.h"
#include "Run.h"
#include "Setup.h"

//#define New_Mode

//#define HoldGame 3

void SETUP();

void minigames(int, DATA*, char*);

int main(void)
{
#ifdef New_Mode

	Status Menu = Initial("��_��.txt");

	SETUP(); Sleep(777);
	
	Memorize(&Menu, "��");
	
#endif

#ifndef New_Mode
	DATA base, type, Menu;

	int choose[2]; int game[game_amount], index_ = -1;
	char* filename = NULL;

	SETUP(); Sleep(777);

	for (int i = 0; i < game_amount; i++)
	{
		game[i] = rand() % game_amount;
		for (int j = 0; j < i; j++)
			if (game[i] == game[j])
				i--;
	}

	while (1)
	{
		do
		{
			choose[0] = BootScreen();

			switch (choose[0])
			{
			case 1: //�����ϱ�
				break;
			case 2: //����
				SettingScreen();
				ApplySetting();
				SaveSetting();
				break;
			case 3: //Contribute
				ContributeScreen();
				break;
			case 4: //Exit
				ClearScreen(); return 0;
			}
		} while (choose[0] != 1);

		base = Initial("Region.txt");
		type = Initial("FoodType.txt");

		do
		{
			choose[0] = StartScreen();

			switch (choose[0])
			{
			case -1: //���ư���
				choose[0] = 100;
				break;
			case 1: //���� ����
				Menu = AllMenu(&base, &type);
				minigames(game[(++index_ >= game_amount) ? (index_ = 0) : index_], &Menu, "���� ����");
				break;

			case 2: //���� ����
				choose[0] = RegionSelectScreen(&base);
				if (choose[0] == -1) {
					choose[0] = 200; continue;
				}
				Menu = RegionMenu(base.name[choose[0] - 1], &type);
				minigames(game[(++index_ >= game_amount) ? (index_ = 0) : index_], &Menu, base.name[choose[0] - 1]);
				break;

			case 3: //���� ����
				do {
					choose[1] = FoodTypeSelectScreen(&type);
					if (choose[1] == -1) {
						choose[0] = 200; continue;
					}
					choose[0] = RegionSelectScreen(&base);
					if (choose[0] != -1) {
						break;
					}
				} while (choose[0] != 200);
				if (choose[0] == 200) continue;

				filename = GetFileName(base.name[choose[0] - 1], type.name[choose[1] - 1]);
				Menu = Initial(filename); free(filename);
				minigames(game[(++index_ >= game_amount) ? (index_ = 0) : index_], &Menu, type.name[choose[1] - 1]);
				break;
			}

			if (choose[0] == 100)
				continue;
			Destruct(&Menu);
			ClearScreen();
		} while (choose[0] != 100);

		Destruct(&base); Destruct(&type);
	}
	
#endif
}

void SETUP()
{
	system("mode con cols=30 lines=20");

	{
	HANDLE hConsole; CONSOLE_CURSOR_INFO ConsoleCursor;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	ConsoleCursor.bVisible = 0; ConsoleCursor.dwSize = 1;
	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
	}//Ŀ�� ����

	srand((unsigned int)time(NULL));

	GetSetting();
}

void minigames(int type, DATA* Menu, char* str)
{
	if (hold_ != -1)
		type = hold_;

#ifdef HoldGame
	type = HoldGame;
#endif

	switch (type)
	{
	case 0: //����
		Roll(Menu, str);
		break;
	case 1: //���
		Pull(Menu, str);
		break;
	case 2: //ã��
		Puzzle(Menu, str);
		break;
	case 3: //����
		Dart(Menu, str);
		break;
	case 4: //�ܿ�
		Memorize(Menu, str);
		break;
	}
}

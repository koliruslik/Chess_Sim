// Chess_sim.cpp: определяет точку входа для приложения.
//
#include "raylib.h"
#include "Chess_sim.h"


#include "MainMenu.h"
#include "Button.h"
#include "time.h"
using namespace std;


enum GameState
{
	MAIN_MENU,
	SETTINGS,
	GAMEPLAY
};


string GetCurrentTime()
{
	// Получаем текущее время
	time_t now = time(0);

	// Преобразуем в локальное время
	struct tm* timeInfo = localtime(&now);

	// Форматируем время в строку
	char buffer[80];
	strftime(buffer, sizeof(buffer), "%H:%M:%S", timeInfo); // Формат: ЧЧ:ММ:СС
	return string(buffer);
}

int main()
{
	const int screenWidth = 1920;
	const int screenHeight = 1080;
	const char* title = "Chess_sim";
	InitWindow(screenWidth, screenHeight, title);

	GameState gameState = MAIN_MENU;
	MainMenu mainMenu(screenWidth, screenHeight);
	Button startGameButton(screenWidth / 2, screenHeight / 2 - 25, 150, 50, "Start Game");
	Button exitGameButton(screenWidth / 2, screenHeight / 2 + 25, 150, 50, "Exit");

	SetTargetFPS(60);
	int i = 0;
	string currentTime = GetCurrentTime();
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);
		currentTime = GetCurrentTime();
		ClearBackground(BLACK);
		switch (gameState)
		{
		case MAIN_MENU:
			
			mainMenu.DrawMenu();
			mainMenu.Update();
			
			DrawText(currentTime.c_str(), 10,10,20,WHITE );
			if (mainMenu.shouldExitGame())
			{
				CloseWindow();
			}
			
			break;
		case SETTINGS:
			break;
		case GAMEPLAY:
			break;
		}
		EndDrawing();
	}
	cout << GetScreenWidth() << endl;
	
	
	//return 0;
}

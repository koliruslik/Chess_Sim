// Chess_sim.cpp: определяет точку входа для приложения.
//
#include "raylib.h"
#include "Chess_sim.h"


#include "MainMenu.h"
#include "Button.h"
#include "Timer.h"
#include "Pieces.h"
using namespace std;


enum GameState
{
	MAIN_MENU,
	SETTINGS,
	GAMEPLAY
};




int main()
{
	const int screenWidth = 1920;
	const int screenHeight = 1080;
	const char* title = "Chess_sim";
	InitWindow(screenWidth, screenHeight, title);
	Timer timer;
	GameState gameState = MAIN_MENU;
	MainMenu mainMenu(screenWidth, screenHeight);
	Button startGameButton(screenWidth / 2, screenHeight / 2 - 25, 150, 50, "Start Game");
	Button exitGameButton(screenWidth / 2, screenHeight / 2 + 25, 150, 50, "Exit");
	Pieces pieces("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");

	pieces.PrintPieces();

	//SetTargetFPS(60);
	//int i = 0;
	//while (!WindowShouldClose())
	//{
	//	BeginDrawing();
	//	ClearBackground(BLACK);
	//	switch (gameState)
	//	{
	//	case MAIN_MENU:
	//		
	//		mainMenu.DrawMenu();
	//		mainMenu.Update();
	//		
	//		DrawText(timer.GetCurrentTime().c_str(), 10, 10, 20, WHITE);
	//		if (mainMenu.shouldExitGame())
	//		{
	//			CloseWindow();
	//		}
	//		else if (mainMenu.shouldStartGame())
	//		{
	//			gameState = GAMEPLAY;
	//		}
	//		else if (mainMenu.isSettingsMenu())
	//		{
	//			gameState = SETTINGS;
	//		}
	//		
	//		break;
	//	case SETTINGS:
	//		DrawText("SETTINGS", screenWidth / 2 - MeasureText("SETTINGS", 20), 0, 20, WHITE);//temporary
	//		break;
	//	case GAMEPLAY:
	//		DrawText("CHESS", screenWidth / 2 - MeasureText("CHESS", 20), 0, 20, WHITE);//temporary
	//		break;
	//	default:
	//		break;
	//	}
	//	EndDrawing();
	//}
	//cout << GetScreenWidth() << endl;
	return 0;
	
	//return 0;
}

// Chess_sim.cpp: определяет точку входа для приложения.
//
#include "raylib.h"
#include "Chess_sim.h"


#include "MainMenu.h"
#include "Button.h"
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

	GameState gameState = MAIN_MENU;
	MainMenu mainMenu(screenWidth, screenHeight);
	Button button(screenWidth/2, screenHeight/2, 200, 50, "START GAME");

	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		button.Draw();

	}
	cout << GetScreenWidth() << endl;
	
	CloseWindow();
	//return 0;
}

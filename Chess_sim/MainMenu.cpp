#include "MainMenu.h"

MainMenu::MainMenu(int screenWidth, int screenHeight) 
	: Menu(screenWidth, screenHeight), startGame(false), exitGame(false){}

void MainMenu::Update()
{

}

void MainMenu::DrawMenu()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

	drawButton("Start Game", screenWidth / 2, screenHeight / 2 - 50);
    
    EndDrawing();
}
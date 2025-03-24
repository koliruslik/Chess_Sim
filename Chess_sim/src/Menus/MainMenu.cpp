#include "MainMenu.h"


MainMenu::MainMenu(int screenWidth, int screenHeight)
	: Menu(screenWidth, screenHeight) {
	AddButton("START", Button(screenWidth / 2, screenHeight / 2, 200, 50, "START GAME"));
	AddButton("SETTINGS", Button(screenWidth / 2, screenHeight / 2 + 60, 200, 50, "SETTINGS"));
	AddButton("EXIT", Button(screenWidth / 2, screenHeight / 2 + 120, 200, 50, "EXIT"));
	mainMenu = true;
	settings = false;
	startGame = false;
	exitGame = false;
}

void MainMenu::ButtonAction(Button& button)
{
	if (button.text == "START GAME")
	{
		startGame = true;
		mainMenu = false;
		printf("Start Game Pressed\n");

	}
	else if (button.text == "SETTINGS")
	{
		settings = true;
		mainMenu = false;
		printf("Settings Pressed\n");
	}
	else if (button.text == "EXIT")
	{
		exitGame = true;
		mainMenu = false;
		printf("Exit Pressed\n");
	}
}
bool MainMenu::shouldStartGame() const
{
	return startGame;
}
bool MainMenu::shouldExitGame() const
{
	return exitGame;
}
bool MainMenu::isSettingsMenu() const
{
	return settings;
}
bool MainMenu::isMainMenu() const
{
	return mainMenu;
}
#include "MainMenu.h"


MainMenu::MainMenu(int screenWidth, int screenHeight)
	: Menu(screenWidth, screenHeight) {
	AddButton("VS FRIEND", Button(screenWidth / 2, screenHeight / 2 - 60, 200, 50, "VS FRIEND"));
	AddButton("PLAY WHITE", Button(screenWidth / 2 + 110, screenHeight / 2, 200, 50, "PLAY WHITE"));
	AddButton("PLAY BLACK", Button(screenWidth / 2 - 110, screenHeight / 2, 200, 50, "PLAY BLACK"));
	AddButton("SETTINGS", Button(screenWidth / 2, screenHeight / 2 + 60, 200, 50, "SETTINGS"));
	AddButton("EXIT", Button(screenWidth / 2, screenHeight / 2 + 120, 200, 50, "EXIT"));
	mainMenu = true;
	settings = false;
	startGame = false;
	exitGame = false;
}

void MainMenu::ButtonAction(Button& button)
{
	if (button.text == "VS FRIEND")
	{
		startGame = true;
		mainMenu = false;
		printf("Start Game Pressed\n");
		aiSideToPlay = SIDE::None;
	}
	else if (button.text == "PLAY WHITE" || button.text == "PLAY BLACK")
	{
		startGame = true;
		mainMenu = false;
		printf("Start Game Pressed\n");
		aiSideToPlay = (button.text == "PLAY WHITE") ? SIDE::Black : SIDE::White;

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
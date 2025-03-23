#include "MainMenu.h"


MainMenu::MainMenu(int screenWidth, int screenHeight)
	: Menu(screenWidth, screenHeight), startGame(false), exitGame(false) {
	AddButton("START", Button(screenWidth / 2, screenHeight / 2, 200, 50, "START GAME"));
	AddButton("SETTINGS", Button(screenWidth / 2, screenHeight / 2 + 60, 200, 50, "SETTINGS"));
	AddButton("EXIT", Button(screenWidth / 2, screenHeight / 2 + 120, 200, 50, "EXIT"));
}

void MainMenu::Update()
{
    for (Button& button : buttons) 
    {
        if (button.isClicked())
        {
            if (button.text == "START GAME") 
            {
				startGame = true;
                printf("Start Game Pressed\n");
            }
            else if (button.text == "SETTINGS") 
            {
                printf("Settings Pressed\n");
            }
            else if (button.text == "EXIT")
            {
				exitGame = true;
                printf("Exit Pressed\n");
            }
        }
    }
}

void MainMenu::ButtonAction(Button& button)
{
	if (button.text == "Start Game")
	{
		startGame = true;
	}
	else if (button.text == "Exit")
	{
		exitGame = true;
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
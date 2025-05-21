#include "ESCMenu.h"

ESCMenu::ESCMenu(int screenWidth, int screenHeight, std::shared_ptr<BoardRenderer>& renderer)
	: Menu(screenWidth, screenHeight, renderer)
{
	AddMainButtons();
}

void ESCMenu::ButtonAction(Button& button)
{
	std::string text = button.text;

	if (text == "RESUME")
	{
		result = ESCResult::Resume;
		LogButtonPress("Resuming game");
	}
	else if (text == "SAVE/LOAD")
	{
		LogButtonPress("Opening save menu");
		buttons.clear();
		AddSaveButtons();
	}
	else if (text == "SAVE GAME")
	{
		result = ESCResult::Save;
		LogButtonPress("Saving game");
	}
	else if (text == "LOAD GAME")
	{
		result = ESCResult::Load;
		LogButtonPress("Loading game");
	}
	else if (text == "GAME1")
	{
		filePath = Save1Path;
		LogButtonPress("Selected GAME1 slot");
	}
	else if (text == "GAME2")
	{
		filePath = Save2Path;
		LogButtonPress("Selected GAME2 slot");
	}
	else if (text == "GAME3")
	{
		filePath = Save3Path;
		LogButtonPress("Selected GAME3 slot");
	}
	else if (text == "BACK")
	{
		buttons.clear();
		AddMainButtons();
		LogButtonPress("Returning to main menu in ESC");
	}
	else if (text == "MENU")
	{
		result = ESCResult::ToMainMenu;
		LogButtonPress("Returning to main menu from ESC");
	}
}

void ESCMenu::AddMainButtons()
{
	AddButton("RESUME", Button(centerPosX, centerPosY - buttonOffsetY, buttonSizeX, buttonSizeY, "RESUME"));
	AddButton("SAVE/LOAD", Button(centerPosX, centerPosY, buttonSizeX, buttonSizeY, "SAVE/LOAD"));
	AddButton("MENU", Button(centerPosX, centerPosY + buttonOffsetY * 2, buttonSizeX, buttonSizeY, "MENU"));
}

void ESCMenu::AddSaveButtons()
{
	AddButton("GAME1", Button(centerPosX - buttonOffsetX, centerPosY, buttonSizeX, buttonSizeY, "GAME1"));
	AddButton("GAME2", Button(centerPosX, centerPosY, buttonSizeX, buttonSizeY, "GAME2"));
	AddButton("GAME3", Button(centerPosX + buttonOffsetX, centerPosY, buttonSizeX, buttonSizeY, "GAME3"));
	AddButton("SAVE GAME", Button(centerPosX - buttonOffsetX / 2, centerPosY + buttonOffsetY * 2, buttonSizeX, buttonSizeY, "SAVE GAME"));
	AddButton("LOAD GAME", Button(centerPosX + buttonOffsetX / 2, centerPosY + buttonOffsetY * 2, buttonSizeX, buttonSizeY, "LOAD GAME"));
	AddButton("BACK", Button(centerPosX, centerPosY + buttonOffsetY * 3, buttonSizeX, buttonSizeY, "BACK"));
}


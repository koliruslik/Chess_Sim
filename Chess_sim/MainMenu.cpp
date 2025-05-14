#include "MainMenu.h"


MainMenu::MainMenu(int screenWidth, int screenHeight)
	: Menu(screenWidth, screenHeight),
	aiSideToPlay(SIDE::None),
	vsAi(false)
{
	AddMainButtons();
	LoadIconTexture(Icon1Path, theme1Icon);
	LoadIconTexture(Icon2Path, theme2Icon);
	LoadIconTexture(Icon3Path, theme3Icon);
	LoadIconTexture(squarePath, squareTexture);
}
void MainMenu::ButtonAction(Button& button)
{
	const std::string& text = button.name;
	if (state == MenuState::Main)
	{
		if (text == "VS FRIEND") {
			result = MenuResult::StartGame;
			aiSideToPlay = SIDE::None;
			vsAi = false;
			LogButtonPress("Start Game (VS FRIEND)");
		}
		else if (text == "VS AI") {
			state = MenuState::AiSideSelection;
			buttons.clear();
			AddAiSelectionButtons();
			LogButtonPress("Switched to AI side selection");
		}
		else if (text == "SETTINGS") {
			//result = MenuResult::OpenSettings;
			state = MenuState::Settings;
			buttons.clear();
			AddSettingsButtons();
			LogButtonPress("Settings");
		}
		else if (text == "EXIT") {
			result = MenuResult::Exit;
			LogButtonPress("Exit");
		}
	}
	else if (state == MenuState::AiSideSelection) 
	{
		if (text == "WHITE") {
			result = MenuResult::StartGame;
			aiSideToPlay = SIDE::Black;
			vsAi = true;
			LogButtonPress("WHITE (Player) selected");
		}
		else if (text == "BLACK") {
			result = MenuResult::StartGame;
			aiSideToPlay = SIDE::White;
			vsAi = true;
			LogButtonPress("BLACK (Player) selected");
		}
		else if (text == "BACK") {
			state = MenuState::Main;
			buttons.clear();
			AddMainButtons();
			LogButtonPress("Returned to Main Menu");
		}
	}
	else if (state == MenuState::Settings)
	{
		if (text == "THEME1")
		{
			theme = Theme::Theme1;
			LogButtonPress("Theme1 selected");
		}
		else if (text == "THEME2")
		{
			theme = Theme::Theme2;
			LogButtonPress("Theme2 selected");
		}
		else if (text == "THEME3")
		{
			theme = Theme::Theme3;
			LogButtonPress("Theme3 selected");
		}
		else if (text == "BACK")
		{
			result = MenuResult::None;
			state = MenuState::Main;
			buttons.clear();
			AddMainButtons();
			LogButtonPress("Returned to Main Menu");
		}
	}
}

void MainMenu::AddMainButtons()
{
	AddButton("VS FRIEND", Button(screenWidth / 2, screenHeight / 2 - 60, 200, 50, "VS FRIEND"));
	AddButton("VS AI", Button(screenWidth / 2, screenHeight / 2, 200, 50, "VS AI"));
	AddButton("SETTINGS", Button(screenWidth / 2, screenHeight / 2 + 60, 200, 50, "SETTINGS"));
	AddButton("EXIT", Button(screenWidth / 2, screenHeight / 2 + 120, 200, 50, "EXIT"));
}

void MainMenu::AddAiSelectionButtons()
{
	AddButton("WHITE", Button(screenWidth / 2 - 110, screenHeight / 2, 200, 50, "WHITE"));
	AddButton("BLACK", Button(screenWidth / 2 + 110, screenHeight / 2, 200, 50, "BLACK"));
	AddButton("BACK", Button(screenWidth / 2, screenHeight / 2 + 80, 200, 50, "BACK"));
}

void MainMenu::AddSettingsButtons()
{
	Vector2 cellPos1 = { screenWidth / 2 - 220, screenHeight / 2 };
	Vector2 cellPos2 = { screenWidth / 2, screenHeight / 2 };
	Vector2 cellPos3 = { screenWidth / 2 + 220, screenHeight / 2 };
	AddButton("THEME1", Button(screenWidth / 2 - 220, screenHeight / 2, 100, 100, "", squareTexture));
	AddButton("THEME2", Button(screenWidth / 2, screenHeight / 2, 100, 100, "", squareTexture));
	AddButton("THEME3", Button(screenWidth / 2 + 220, screenHeight / 2, 100, 100, "", squareTexture));
	AddButton("ICON", Button(screenWidth / 2 - 220, screenHeight / 2, 100, 100, "", theme1Icon));
	AddButton("ICON", Button(screenWidth / 2, screenHeight / 2, 100, 100, "", theme2Icon));
	AddButton("ICON", Button(screenWidth / 2 + 220, screenHeight / 2, 100, 100, "", theme3Icon));
	


	AddButton("BACK", Button(screenWidth / 2, screenHeight / 2 + 150, 200, 50, "BACK"));
}

MenuResult MainMenu::getResult() const {
	return result;
}

SIDE MainMenu::getSideToPlay() const {
	return aiSideToPlay;
}

bool MainMenu::isVsAi() const {
	return vsAi;
}

Theme MainMenu::getTheme() const
{
	return theme;
}

void MainMenu::LoadIconTexture(std::string filePath, Texture2D &texture)
{
		texture = LoadTexture(filePath.c_str());
		if (texture.id == 0) {
			std::cerr << "Failed to load white piece texture: " << filePath << std::endl;
		}	
}

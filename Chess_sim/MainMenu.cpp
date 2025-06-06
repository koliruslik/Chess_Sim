#include "MainMenu.h"


MainMenu::MainMenu(int screenWidth, int screenHeight, std::shared_ptr<BoardRenderer>& renderer)
	: Menu(screenWidth, screenHeight, renderer),
	aiSideToPlay(SIDE::None),
	pawnTheme1(Init::getPawnIcon(Theme::Theme1, true)),
	pawnTheme2(Init::getPawnIcon(Theme::Theme2, true)),
	pawnTheme3(Init::getPawnIcon(Theme::Theme3, true)),
	squareBTex(Init::getSquareBlack()),
	squareWTex(Init::getSquareWhite()),
	vsAi(false)
{
	boardRenderTexture = LoadRenderTexture(screenWidth, screenHeight);
	AddMainButtons();
}

MainMenu::~MainMenu()
{
	UnloadRenderTexture(boardRenderTexture);
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
		}
		else if (text == "SETTINGS") {

			state = MenuState::Settings;
			drawIconText = true;
			buttons.clear();
			AddSettingsButtons();
			LogButtonPress("Settings");
		}
		else if (text == "CONSTRUCT POS")
		{
			result = MenuResult::ConstructPosition;
			LogButtonPress("Construct position");
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
			selectedThemeText = "Selected Theme: 1";
			LogButtonPress("Theme1 selected");
		}
		else if (text == "THEME2")
		{
			theme = Theme::Theme2;
			selectedThemeText = "Selected Theme: 2";
			LogButtonPress("Theme2 selected");
		}
		else if (text == "THEME3")
		{
			theme = Theme::Theme3;
			selectedThemeText = "Selected Theme: 3";
			LogButtonPress("Theme3 selected");
		}
		else if (text == "BACK")
		{
			state = MenuState::Main;
			drawIconText = false;
			buttons.clear();
			AddMainButtons();
			LogButtonPress("Returned to Main Menu");
		}
	}
}

void MainMenu::AddMainButtons()
{
	AddButton("VS FRIEND", Button(centerPosX + buttonOffsetX / 2, centerPosY, buttonSizeX, buttonSizeY, "VS FRIEND"));
	AddButton("VS AI", Button(centerPosX - buttonOffsetX / 2, centerPosY, buttonSizeX, buttonSizeY, "VS AI"));
	AddButton("CONSTRUCT POS", Button(centerPosX, centerPosY - buttonOffsetY, buttonSizeX*1.25, buttonSizeY, "CONSTRUCT POS"));
	AddButton("SETTINGS", Button(centerPosX, centerPosY + buttonOffsetY, buttonSizeX, buttonSizeY, "SETTINGS"));
	AddButton("EXIT", Button(centerPosX, centerPosY + buttonOffsetY*2, buttonSizeX, buttonSizeY, "EXIT"));
}

void MainMenu::AddAiSelectionButtons()
{
	AddButton("WHITE", Button(centerPosX - buttonOffsetX / 2, centerPosY, buttonSizeX, buttonSizeY, "WHITE"));
	AddButton("BLACK", Button(centerPosX + buttonOffsetX / 2, centerPosY, buttonSizeX, buttonSizeY, "BLACK"));
	AddButton("BACK", Button(centerPosX, centerPosY + buttonOffsetY*3, buttonSizeX, buttonSizeY, "BACK"));
}

void MainMenu::AddSettingsButtons()
{

	AddButton("THEME1", Button(centerPosX - buttonOffsetX, centerPosY, buttonSizeX/2, buttonSizeX/2, "", squareBTex));
	AddButton("THEME2", Button(centerPosX, centerPosY, buttonSizeX / 2, buttonSizeX / 2, "", squareBTex));
	AddButton("THEME3", Button(centerPosX + buttonOffsetX, centerPosY, buttonSizeX / 2, buttonSizeX / 2, "", squareBTex));
	AddButton("ICON", Button(centerPosX - buttonOffsetX, centerPosY, buttonSizeX / 2, buttonSizeX / 2, "", pawnTheme1));
	AddButton("ICON", Button(centerPosX, centerPosY, buttonSizeX / 2, buttonSizeX / 2, "", pawnTheme2));
	AddButton("ICON", Button(centerPosX + buttonOffsetX, centerPosY, buttonSizeX / 2, buttonSizeX / 2, "", pawnTheme3));
	


	AddButton("BACK", Button(centerPosX, centerPosY + buttonOffsetY*3, buttonSizeX, buttonSizeY, "BACK"));
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

void MainMenu::resetResult() {
	result = MenuResult::None;
}


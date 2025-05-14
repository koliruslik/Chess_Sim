#pragma once

#ifndef MAINMENU_H
#define MAINMENU_H

#include <map>

#include "Menu.h"
#include "Pieces.hpp"
#include "MenuResult.h"
#include "raylib.h"

enum class MenuState {
	Main,
	AiSideSelection,
	Settings
};

enum class Theme {
	Theme1,
	Theme2,
	Theme3
};

class MainMenu : public Menu
{
public:
	MainMenu(int screenWidth, int screenHeight);

	void ButtonAction(Button& button) override;

	MenuResult getResult() const;
	SIDE getSideToPlay() const;
	Theme getTheme() const;
	bool isVsAi() const;

	
private:
	MenuResult result = MenuResult::None;
	MenuState state = MenuState::Main;
	Theme theme = Theme::Theme1;
	SIDE aiSideToPlay = SIDE::None;
	const std::string Icon1Path = "..\\..\\..\\..\\recources\\ChessPieces\\White\\pawnW.png";
	const std::string Icon2Path = "..\\..\\..\\..\\recources\\ChessPieces\\White\\pawnW2.png";
	const std::string Icon3Path = "..\\..\\..\\..\\recources\\ChessPieces\\White\\pawnW3.png";
	const std::string squarePath = "..\\..\\..\\..\\recources\\boards\\squareB.png";
	Texture2D theme1Icon;
	Texture2D theme2Icon;
	Texture2D theme3Icon;
	Texture2D squareTexture;
	bool vsAi = false;
	void AddMainButtons();
	void AddAiSelectionButtons();
	void AddSettingsButtons();
	void LoadIconTexture(std::string filePath, Texture2D &texture);
};
#endif // MAINMENU_H
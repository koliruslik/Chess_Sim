#pragma once

#ifndef MAINMENU_H
#define MAINMENU_H

#include <map>

#include "Menu.h"
#include "Pieces.hpp"
#include "raylib.h"
#include "Init.h"
#include "GameEnums.h"
#include "Timer.h"
#include "BoardRenderer.h"

class MainMenu : public Menu
{
public:
	MainMenu(int screenWidth, int screenHeight, std::shared_ptr<BoardRenderer>& renderer);
	~MainMenu();
	void ButtonAction(Button& button) override;

	MenuResult getResult() const;
	SIDE getSideToPlay() const;
	Theme getTheme() const;
	bool isVsAi() const;
	void resetResult();
	 
	
private:
	RenderTexture2D boardRenderTexture;

	const Texture2D& pawnTheme1;
	const Texture2D& pawnTheme2;
	const Texture2D& pawnTheme3;
	const Texture2D& squareBTex;
	const Texture2D& squareWTex;

	MenuResult result = MenuResult::None;
	MenuState state = MenuState::Main;
	Theme theme = Theme::Theme1;
	SIDE aiSideToPlay = SIDE::None;
	bool vsAi = false;

	void AddMainButtons();
	void AddAiSelectionButtons();
	void AddSettingsButtons();
};
#endif // MAINMENU_H
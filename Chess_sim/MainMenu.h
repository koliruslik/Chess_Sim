#pragma once

#ifndef MAINMENU_H
#define MAINMENU_H


#include "Menu.h"
#include "Pieces.hpp"

class MainMenu : public Menu
{


public:
	MainMenu(int screenWidth, int screenHeight);

	bool shouldStartGame() const;
	bool shouldExitGame() const;
	bool isSettingsMenu() const;
	bool isMainMenu() const;
	void ButtonAction(Button& button) override;

	SIDE getSideToPlay() const { return aiSideToPlay; }
	bool isVsAi() const { return vsAi; }
private:
	SIDE aiSideToPlay;
	bool vsAi;
};

#endif // MAINMENU_H
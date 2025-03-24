#pragma once

#ifndef MAINMENU_H
#define MAINMENU_H


#include "Menu.h"


class MainMenu : public Menu
{


public:
	MainMenu(int screenWidth, int screenHeight);

	bool shouldStartGame() const;
	bool shouldExitGame() const;
	bool isSettingsMenu() const;
	bool isMainMenu() const;

	void ButtonAction(Button& button) override;

};


#endif // MAINMENU_H
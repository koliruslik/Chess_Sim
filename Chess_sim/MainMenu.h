#pragma once

#ifndef MAINMENU_H
#define MAINMENU_H


#include "Menu.h"


class MainMenu : public Menu
{
private:
	bool startGame = false;
	bool exitGame = false;

public:
	MainMenu(int screenWidth, int screenHeight);

	void ButtonAction(Button& button) override;
	void Update() override;
	bool shouldStartGame() const;
	bool shouldExitGame() const;

};


#endif // MAINMENU_H
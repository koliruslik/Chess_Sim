#pragma once

#ifndef MENU_H
#define MENU_H

#include "raylib.h"
#include "Button.h"

#include <vector>
#include <string>


class Menu
{
protected:
	int screenWidth;
	int screenHeight;
	const int fontSize = 20;
	std::vector<Button> buttons;

	bool mainMenu = false;
	bool startGame = false;
	bool exitGame = false;
	bool settings = false;

public:
	Menu(int screenWidth, int screenHeight);
	virtual ~Menu() = default;
	

	virtual void Update();
	virtual void DrawMenu();
	

	void AddButton(const std::string& name, Button button);
	virtual void ButtonAction(Button& button) = 0;
};

#endif // MENU_H	
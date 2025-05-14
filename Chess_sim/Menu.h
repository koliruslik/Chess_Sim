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

	void LogButtonPress(const char* message) const;
	void AddButton(const std::string& name, Button button);
public:
	Menu(int screenWidth, int screenHeight);
	virtual ~Menu() = default;
	
	virtual void Update();
	virtual void DrawMenu();
	
	virtual void ButtonAction(Button& button) = 0;
};

#endif // MENU_H	
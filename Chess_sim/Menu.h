#pragma once

#ifndef MENU_H
#define MENU_H

#include "raylib.h"
#include "Button.h"
#include "BoardRenderer.h"

#include <vector>
#include <string>
#include <memory>

class Menu
{
protected:
	std::shared_ptr<BoardRenderer> renderer;
	int screenWidth;
	int screenHeight;
	const int fontSize = 20;
	std::vector<Button> buttons;
	bool drawIconText = false;
	std::string selectedThemeText = "Selected Theme: 1";
	void LogButtonPress(const char* message) const;
	void AddButton(const std::string& name, Button button);

	const float buttonSizeX = 200;
	const float buttonSizeY = 50;
	const float buttonOffsetX = buttonSizeX + 10;
	const float buttonOffsetY = buttonSizeY + 10;

	const float centerPosX = screenWidth / 2;
	const float centerPosY = screenHeight / 2;
	
public:
	Menu(int screenWidth, int screenHeight, std::shared_ptr<BoardRenderer>& renderer);
	virtual ~Menu() = default;
	
	virtual void Update();
	virtual void DrawMenu();
	
	virtual void ButtonAction(Button& button) = 0;
};

#endif // MENU_H	
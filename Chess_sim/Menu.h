#pragma once

#ifndef MENU_H
#define MENU_H

#include "raylib.h"

class Menu
{
protected:
	int screenWidth;
	int screenHeight;
	const int fontSize = 20;

public:
	Menu(int screenWidth, int screenHeight);
	virtual ~Menu() = default;

	virtual void Update() = 0;
	virtual void DrawMenu() = 0;
	void drawButton(const char* text, int posX, int posY) const;

	
};

#endif // MENU_H	
#pragma once

#ifndef BUTTON_H
#define BUTTON_H

#include "raylib.h"
#include <functional>
#include <string>
class Button
{
public:
	Rectangle bounds;
	Color color;
	const char* text;
	std::string name;

	Texture2D texture;
	bool hasTexture = false;

	Button(float x, float y, float width, float heigth, const char* text);
	Button(float x, float y, float width, float height, const char* text, Texture2D texture);
	void Draw();
	bool IsMouseOver();
	bool IsClicked();

	void setName(std::string name) { this->name = name; }
private:
	const int fontSize = 20;
	bool hovered = false;
	bool wasPressedInside = false;
};

#endif // BUTTON_H
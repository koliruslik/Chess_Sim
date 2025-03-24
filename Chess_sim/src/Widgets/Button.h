#pragma once

#ifndef BUTTON_H
#define BUTTON_H

#include "raylib.h"
#include <functional>
class Button
{
public:
	Rectangle bounds;
	Color color;
	const char* text;

	Button(float x, float y, float width, float heigth, const char* text);

	void Draw();
	bool IsMouseOver();
	bool isClicked() const;

private:
	const int fontSize = 20;
};

#endif // BUTTON_H
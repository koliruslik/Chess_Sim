#include "Menu.h"

Menu::Menu(int screenWidth, int screenHeight)
{
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
}
void Menu::drawButton(const char* text, int posX, int posY) const
{
	int textWidth = MeasureText(text, fontSize);
	int textHeight = fontSize;
	posX -= textWidth;
	
	DrawRectangle(posX - 5, posY - 5, textWidth + 10, textHeight + 10, LIGHTGRAY);
	DrawRectangleLines(posX - 5, posY - 5, textWidth + 10, textHeight + 10, GRAY);
	DrawText(text, posX, posY, fontSize, BLACK);
}
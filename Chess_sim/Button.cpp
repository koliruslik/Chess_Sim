#include "Button.h"

Button::Button(float x, float y, float width, float height, const char* text)
	: bounds({ x - width/2, y - height/2, width, height }), color(LIGHTGRAY), text(text) {}



void Button::Draw()
{
	DrawRectangle(bounds.x, bounds.y, bounds.width, bounds.height, color);
	//DrawRectangleLinesEx(bounds, 2, GRAY);
	int textWidth = MeasureText(text, fontSize);
	DrawText(text, bounds.x + bounds.width / 2 - textWidth / 2, bounds.y + 8, fontSize, BLACK);
}
bool Button::IsMouseOver() 
{
    Vector2 mousePos = GetMousePosition();
    if (mousePos.x > bounds.x && mousePos.x < bounds.x + bounds.width &&
        mousePos.y > bounds.y && mousePos.y < bounds.y + bounds.height)
    {
        color = GRAY;
        return true;
    }
    else
    {
        color = LIGHTGRAY;
        return false;
    }
}
bool Button::isClicked() const {
	return CheckCollisionPointRec(GetMousePosition(), bounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

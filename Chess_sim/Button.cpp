#include "Button.h"

Button::Button(float x, float y, float width, float height, const char* text)
    : bounds{ x - width / 2, y - height / 2, width, height },
    color(LIGHTGRAY), text(text), hasTexture(false) , name(""){
}

Button::Button(float x, float y, float width, float height, const char* text, Texture2D texture)
    : bounds{ x - width / 2, y - height / 2, width, height },
    color(WHITE), text(text), texture(texture), hasTexture(true), name("") {
}

void Button::Draw()
{
    if (hasTexture)
    {
        DrawTexturePro(
            texture,
            { 0, 0, (float)texture.width, (float)texture.height },
            bounds,
            { 0, 0 },
            0.0f,
            WHITE
        );
    }
    else
    {
        DrawRectangle(bounds.x, bounds.y, bounds.width, bounds.height, color);
    }

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

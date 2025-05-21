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
    Vector2 mousePos = GetMousePosition();
    bool hovered = CheckCollisionPointRec(mousePos, bounds);
    bool pressed = wasPressedInside;
    Rectangle drawBounds = bounds;

    if (pressed)
    {
        drawBounds.x += 2;
        drawBounds.y += 2;
    }

    if (pressed)
        color = DARKGRAY;
    else if (hovered)
        color = GRAY;
    else
        color = LIGHTGRAY;
    if (hasTexture)
    {
        DrawTexturePro(
            texture,
            { 0, 0, (float)texture.width, (float)texture.height },
            drawBounds,
            { 0, 0 },
            0.0f,
            WHITE
        );
    }
    else
    {
        DrawRectangle(drawBounds.x, drawBounds.y, drawBounds.width, drawBounds.height, color);
    }

    int textWidth = MeasureText(text, fontSize);
    DrawText(text, drawBounds.x + drawBounds.width / 2 - textWidth / 2, drawBounds.y + 8, fontSize, BLACK);
}

bool Button::IsClicked() {
    Vector2 mousePos = GetMousePosition();
    bool isInside = CheckCollisionPointRec(mousePos, bounds);

    if (isInside && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        wasPressedInside = true;
    }

    if (wasPressedInside && isInside && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        wasPressedInside = false;
        return true;
    }

    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        wasPressedInside = false;
    }

    return false;
}

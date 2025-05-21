#include "Menu.h"

Menu::Menu(int screenWidth, int screenHeight, std::shared_ptr<BoardRenderer>& renderer)
	:screenWidth(screenWidth), screenHeight(screenHeight),
	renderer(renderer)
{
}

void Menu::AddButton(const std::string& name, Button button)
{
	button.setName(name);
	buttons.push_back(button);
}

void Menu::DrawMenu() {
	renderer->drawBoardBlurredAndDarkened();
	for (int i = 0; i < buttons.size(); i++) {
		buttons[i].Draw();


	}
	if (drawIconText)
	{
		int textWidth = MeasureText(selectedThemeText.c_str(), fontSize);
		DrawText(selectedThemeText.c_str(), centerPosX - textWidth / 2, centerPosY + buttonOffsetY * 2, fontSize, WHITE);
	}
}
void Menu::Update()
{
	for (int i = 0; i < buttons.size(); i++)
	{
		if (buttons[i].IsClicked())
		{
			ButtonAction(buttons[i]);
		}
	}
}


void Menu::LogButtonPress(const char* message) const
{
	printf("%s Pressed\n", message);
}
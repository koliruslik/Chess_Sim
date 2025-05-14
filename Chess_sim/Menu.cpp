#include "Menu.h"

Menu::Menu(int screenWidth, int screenHeight)
	: screenWidth(screenWidth), screenHeight(screenHeight) {
}

void Menu::AddButton(const std::string& name, Button button)
{
	button.setName(name);
	buttons.push_back(button);
}

void Menu::DrawMenu() {

    for (int i = 0; i < buttons.size(); i++)
    {
		buttons[i].Draw();
    }

}
void Menu::Update()
{
	for (int i = 0; i < buttons.size(); i++)
	{
		if (buttons[i].isClicked())
		{
			ButtonAction(buttons[i]);
		}
	}
}


void Menu::LogButtonPress(const char* message) const
{
	printf("%s Pressed\n", message);
}
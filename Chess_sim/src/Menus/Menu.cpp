#include "Menu.h"

Menu::Menu(int screenWidth, int screenHeight)
	: screenWidth(screenWidth), screenHeight(screenHeight) {
}

void Menu::AddButton(const std::string& name, Button button)
{
	buttons.push_back(button);
}

void Menu::DrawMenu() {

    for (int i = 0; i < buttons.size(); i++)
    {
        Button& button = buttons[i]; 
        button.Draw();
    }

}
void Menu::Update()
{
	for (int i = 0; i < buttons.size(); i++)
	{
		Button& button = buttons[i];
		if (button.isClicked())
		{
			ButtonAction(button);
		}
	}
}

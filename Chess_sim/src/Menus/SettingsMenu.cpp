
#include "SettingsMenu.h"


SettingsMenu::SettingsMenu(int screenWidth, int screenHeight)
	: Menu(screenWidth, screenHeight)
{
	AddButton("BACK", Button(screenWidth / 2, screenHeight / 2 + 120, 200, 50, "BACK"));
}
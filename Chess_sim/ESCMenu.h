#pragma once


#include "Menu.h"
class ESCMenu : public Menu
{
public:
	ESCMenu(int screenWidth, int screenHeight, std::shared_ptr<BoardRenderer>& renderer);

	enum Action {
		SAVE,
		LOAD,
		NONE
	};

	void ButtonAction(Button& button) override;
	ESCResult getResult() const { return result; }
	Action getAction() const { return action; }
	std::string getFilePath() const { return filePath; }
	void resetResult() { result = ESCResult::None; }
	void resetAction() { action = Action::NONE; }
	

private:
	ESCResult result = ESCResult::None;

	Color WhiteTransparent = { 255,255,255, static_cast<unsigned char>(alpha * 255) };
	int screenWidth;
	int screenHeight;
	float alpha = 0.9f;

	std::string filePath;
	std::string Save1Path = "saves/SAVE1.txt";
	std::string Save2Path = "saves/SAVE2.txt";
	std::string Save3Path = "saves/SAVE3.txt";
	
	Action action = Action::NONE;

	void AddMainButtons();
	void AddSaveButtons();
};
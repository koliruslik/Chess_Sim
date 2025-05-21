#pragma once

#include "Position.hpp"
#include "BoardRenderer.h"
#include "raylib.h"
#include "BTrans.h"

#include <cstdlib>
#include <memory>
class PosConstructor
{
public:
	PosConstructor(Position position, std::shared_ptr<BoardRenderer>& renderer);
	void update();
	MenuResult getResult() { return result; }
	Position getPosition() { return position; }
	int getKingCounter() { return wKingCounter + bKingCounter; }

	void setTheme(Theme theme) { this->theme = theme; }
private:
	Position position;
	std::shared_ptr<BoardRenderer> renderer;
	MenuResult result;
	Theme theme = Theme::Theme1;
	int screenWidth = GetScreenWidth();
	int screenHeight = GetScreenHeight();
	int squareSize = 80;
	int boardSize = 8;
	int wKingCounter = 0;
	int bKingCounter = 0;
	float scale = 1.5;
	const float MAXSCALE = 1.6f;
	const float MINSCALE = 1.4f;
	const float DIFF = 0.05f;
	Vector2 mousePos;
	bool showInfo = true;
	struct SelectedPiece {
		int8_t type = PIECE::NONE;
		int8_t side = SIDE::White;
		std::string strType = "NONE";
		bool isWhite = true;
	};

	SelectedPiece selected;
	 

	void SelectSquare(bool isPlacing);
	void SelectPiece();
	bool isKingAdjacent(uint8_t squareIndex, uint8_t side);
	bool canPlacePiece(uint8_t type, uint8_t side, int row, uint8_t squareIndex);
	int getPieceCap(uint8_t type, uint8_t side);
};